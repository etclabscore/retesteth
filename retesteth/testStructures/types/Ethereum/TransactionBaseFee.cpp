#include "TransactionBaseFee.h"

#include <libdevcore/Address.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <libdevcrypto/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
TransactionBaseFee::TransactionBaseFee(DataObject const& _data, string const& _dataRawPreview, string const& _dataLabel)
  : TransactionAccessList()
{
    fromDataObject(_data);
    m_dataRawPreview = _dataRawPreview;
    m_dataLabel = _dataLabel;
}

void TransactionBaseFee::fromDataObject(DataObject const& _data)
{
    m_secretKey = spVALUE(new VALUE(0));
    try
    {
        m_accessList = spAccessList(new AccessList(_data.atKey("accessList")));
        m_maxFeePerGas = spVALUE(new VALUE(_data.atKey("maxFeePerGas")));
        m_maxInclusionFeePerGas = spVALUE(new VALUE(_data.atKey("maxInclusionFeePerGas")));

        m_data = spBYTES(new BYTES(_data.atKey("data")));
        m_gasLimit = spVALUE(new VALUE(_data.atKey("gasLimit")));
        m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
        m_value = spVALUE(new VALUE(_data.atKey("value")));

        if (_data.atKey("to").type() == DataType::Null || _data.atKey("to").asString().empty())
            m_creation = true;
        else
        {
            m_creation = false;
            m_to = spFH20(new FH20(_data.atKey("to")));
        }

        if (_data.count("secretKey"))
            buildVRS(_data.atKey("secretKey"));
        else
        {
            m_v = spVALUE(new VALUE(_data.atKey("v")));
            if (m_v.getCContent() > dev::u256("0xff"))
                throw test::UpwardsException("Incorrect transaction `v` value: " + m_v.getCContent().asString());
            m_r = spVALUE(new VALUE(_data.atKey("r")));
            m_s = spVALUE(new VALUE(_data.atKey("s")));
            rebuildRLP();
        }
        requireJsonFields(_data, "TransactionBaseFee " + _data.getKey(),
            {
                {"data", {{DataType::String}, jsonField::Required}},
                {"gasLimit", {{DataType::String}, jsonField::Required}},
                {"nonce", {{DataType::String}, jsonField::Required}},
                {"value", {{DataType::String}, jsonField::Required}},
                {"to", {{DataType::String, DataType::Null}, jsonField::Required}},
                {"secretKey", {{DataType::String}, jsonField::Optional}},
                {"v", {{DataType::String}, jsonField::Optional}},
                {"r", {{DataType::String}, jsonField::Optional}},
                {"s", {{DataType::String}, jsonField::Optional}},

                // Transaction type 1
                {"type", {{DataType::String}, jsonField::Optional}},
                {"chainId", {{DataType::String, DataType::Null}, jsonField::Optional}},
                {"accessList", {{DataType::Array}, jsonField::Required}},

                // Transaction type 2
                {"maxFeePerGas", {{DataType::String}, jsonField::Required}},
                {"maxInclusionFeePerGas", {{DataType::String, DataType::Null}, jsonField::Required}},

                {"publicKey", {{DataType::String}, jsonField::Optional}},  // Besu EthGetBlockBy transaction
                {"raw", {{DataType::String}, jsonField::Optional}},        // Besu EthGetBlockBy transaction

                {"blockHash", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy transaction
                {"blockNumber", {{DataType::String}, jsonField::Optional}},       // EthGetBlockBy transaction
                {"from", {{DataType::String}, jsonField::Optional}},              // EthGetBlockBy transaction
                {"hash", {{DataType::String}, jsonField::Optional}},              // EthGetBlockBy transaction
                {"transactionIndex", {{DataType::String}, jsonField::Optional}},  // EthGetBlockBy transaction
                {"invalid", {{DataType::String}, jsonField::Optional}},           // BlockchainTest filling
            });
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("TransactionBaseFee convertion error: ") + _ex.what() + _data.asJson());
    }
}

TransactionBaseFee::TransactionBaseFee(dev::RLP const& _rlp)
{
    m_secretKey = spVALUE(new VALUE(0));
    fromRLP(_rlp);
}

TransactionBaseFee::TransactionBaseFee(BYTES const& _rlp)
{
    m_secretKey = spVALUE(new VALUE(0));
    dev::bytes decodeRLP = sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
    fromRLP(rlp);
}

void TransactionBaseFee::fromRLP(dev::RLP const& _rlp)
{
    DataObject trData;
    size_t i = 0;
    rlpToString(_rlp[i++]);  // chainID
    trData["nonce"] = rlpToString(_rlp[i++]);

    trData["gasPrice"] = "0x10";  // fake gasPrice
    trData["maxInclusionFeePerGas"] = rlpToString(_rlp[i++]);
    trData["maxFeePerGas"] = rlpToString(_rlp[i++]);
    m_maxInclusionFeePerGas = spVALUE(new VALUE(trData["maxInclusionFeePerGas"]));
    m_maxFeePerGas = spVALUE(new VALUE(trData["maxFeePerGas"]));

    trData["gasLimit"] = rlpToString(_rlp[i++]);
    string const to = rlpToString(_rlp[i++], 0);
    trData["to"] = to == "0x" ? "" : to;
    trData["value"] = rlpToString(_rlp[i++]);
    trData["data"] = rlpToString(_rlp[i++], 0);

    // read access list
    spAccessList list = spAccessList(new AccessList(_rlp[i++]));
    trData["accessList"] = list.getContent().asDataObject();
    m_accessList = list;

    trData["v"] = rlpToString(_rlp[i++]);
    trData["r"] = rlpToString(_rlp[i++]);
    trData["s"] = rlpToString(_rlp[i++]);
    fromDataObject(trData);
}


void TransactionBaseFee::buildVRS(VALUE const& _secret)
{
    setSecret(_secret);

    dev::RLPStream stream;
    stream.appendList(9);
    streamHeader(stream);

    // Alter output with prefixed 02 byte + tr.rlp
    dev::bytes outa = stream.out();
    outa.insert(outa.begin(), dev::byte(2));  // txType

    dev::h256 hash(dev::sha3(outa));
    dev::Signature sig = dev::sign(dev::Secret(_secret.asString()), hash);
    dev::SignatureStruct sigStruct = *(dev::SignatureStruct const*)&sig;
    ETH_FAIL_REQUIRE_MESSAGE(
        sigStruct.isValid(), TestOutputHelper::get().testName() + " Could not construct transaction signature!");

    DataObject v = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.v), 1));
    DataObject r = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.r)));
    DataObject s = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.s)));
    assignV(spVALUE(new VALUE(v)));
    assignR(spVALUE(new VALUE(r)));
    assignS(spVALUE(new VALUE(s)));
    rebuildRLP();
}

void TransactionBaseFee::streamHeader(dev::RLPStream& _s) const
{
    // rlp([chainId, nonce, maxInclusionFeePerGas, maxFeePerGas, gasLimit, to, value, data, access_list, signatureYParity,
    // signatureR, signatureS])
    _s << VALUE(1).asU256();
    _s << nonce().asU256();

    _s << maxInclusionFeePerGas().asU256();
    _s << maxFeePerGas().asU256();

    _s << gasLimit().asU256();
    if (Transaction::isCreation())
        _s << "";
    else
        _s << dev::Address(to().asString());
    _s << value().asU256();
    _s << test::sfromHex(data().asString());

    // Access Listist
    dev::RLPStream accessList(m_accessList.getCContent().list().size());
    for (auto const& el : m_accessList.getCContent().list())
        accessList.appendRaw(el.getCContent().asRLPStream().out());

    _s.appendRaw(accessList.out());
}

DataObject const TransactionBaseFee::asDataObject(ExportOrder _order) const
{
    DataObject out = Transaction::asDataObject(_order);

    out["chainId"] = "0x01";
    out["type"] = "0x02";
    out["maxFeePerGas"] = m_maxFeePerGas.getCContent().asString();
    out["maxInclusionFeePerGas"] = m_maxInclusionFeePerGas.getCContent().asString();
    if (_order == ExportOrder::ToolStyle)
    {
        out["chainId"] = "0x1";
        out["type"] = "0x2";

        DataObject t8ntoolFields;
        t8ntoolFields["maxFeePerGas"] = m_maxFeePerGas.getCContent().asString();
        t8ntoolFields["maxInclusionFeePerGas"] = m_maxInclusionFeePerGas.getCContent().asString();
        t8ntoolFields.performModifier(mod_removeLeadingZerosFromHexValues);
        out["maxFeePerGas"] = t8ntoolFields["maxFeePerGas"];
        out["maxInclusionFeePerGas"] = t8ntoolFields["maxInclusionFeePerGas"];

        if (!m_secretKey.isEmpty() && m_secretKey.getCContent() != 0)
            out["secretKey"] = m_secretKey.getCContent().asString();
    }
    return out;
}

void TransactionBaseFee::rebuildRLP()
{
    // RLP(02 + tr.rlp)
    dev::RLPStream wrapper;
    dev::RLPStream out;
    out.appendList(12);
    streamHeader(out);
    out << v().asU256().convert_to<dev::byte>();
    out << r().asU256();
    out << s().asU256();

    // Alter output with prefixed 02 byte + tr.rlp
    dev::bytes outa = out.out();
    outa.insert(outa.begin(), dev::byte(2));

    // Encode bytearray into rlp
    wrapper << outa;
    m_outRlpStream = wrapper;
    m_rawRLPdata = spBYTES(new BYTES(dev::toHexPrefixed(outa)));
    m_hash = spFH32(new FH32("0x" + dev::toString(dev::sha3(outa))));
}


}  // namespace teststruct
}  // namespace test
