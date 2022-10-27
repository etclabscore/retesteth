#include "BlockMining.h"
#include "Verification.h"
#include <Options.h>
#include <retesteth/TestHelper.h>
#include <testStructures/Common.h>

using namespace dev;
using namespace std;
using namespace test;
using namespace test::debug;
using namespace teststruct;
using namespace dataobject;
namespace fs = boost::filesystem;

namespace  {
void correctHeaderByToolResponse(BlockHeader& _header, ToolResponse const& _res)
{
    // Update a block header with information that we have and what we get from t8ntool
    _header.setStateRoot(_res.stateRoot());
    _header.setGasUsed(_res.totalGasUsed());
    _header.setTransactionHash(_res.txRoot());
    _header.setTrReceiptsHash(_res.receiptRoot());
    _header.setLogsBloom(_res.logsBloom());
    _header.setStateRoot(_res.stateRoot());
}
}

namespace toolimpl
{
ToolChain::ToolChain(
    EthereumBlockState const& _genesis, spSetChainParamsArgs const& _config, fs::path const& _toolPath, fs::path const& _tmpDir)
  : m_initialParams(_config),
    m_engine(_config->sealEngine()),
    m_fork(new FORK(_config->params().atKey("fork"))),
    m_toolPath(_toolPath),
    m_tmpDir(_tmpDir)
{
    m_toolParams = GCP_SPointer<ToolParams>(new ToolParams(_config->params()));

    // Practically tool here does not have any idea of what are the fork names.
    // Because any tool can have custom name implementation. Since this is a mock for tool functions
    // We know what are the fork names. Because devs refuse to expose core logic for tests
    // The protocol logic is implemented here to catch test errors.
    auto const& forks = Options::getCurrentConfig().cfgFile().forks();
    if (inArray(forks, m_fork.getCContent()))
    {
        if (compareFork(m_fork, CMP::lt, FORK("London"))
            && _genesis.header()->type() == BlockType::BlockHeader1559)
            throw test::UpwardsException("Constructing 1559 genesis on network which is lower London!");
        if (compareFork(m_fork, CMP::ge, FORK("London"))
            && _genesis.header()->type() != BlockType::BlockHeader1559
            && _genesis.header()->type() != BlockType::BlockHeaderMerge)
            throw test::UpwardsException("Constructing legacy genesis on network which is higher London!");
    }

    // We yet don't know the state root of genesis. Ask the tool to calculate it
    ToolResponse const res = mineBlockOnTool(_genesis, _genesis, SealEngine::NoReward);

    EthereumBlockState genesisFixed(_genesis.header(), _genesis.state(), FH32::zero());
    genesisFixed.headerUnsafe().getContent().setStateRoot(res.stateRoot());
    genesisFixed.headerUnsafe().getContent().recalculateHash();
    genesisFixed.setTotalDifficulty(genesisFixed.header()->difficulty());
    m_blocks.push_back(genesisFixed);
}

spSetChainParamsArgs genT9NChainParams(FORK const& _net)
{
    spDataObject difficultyParams;
    // Dummy chain params, not used in t9n but we need to setup chainID
    (*difficultyParams).atKeyPointer("accounts") = spDataObject(new DataObject(DataType::Object));
    (*difficultyParams)["genesis"]["author"] = "0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba";
    (*difficultyParams)["genesis"]["difficulty"] = "0x00";
    (*difficultyParams)["genesis"]["gasLimit"] = "0xff112233445566";
    (*difficultyParams)["genesis"]["baseFeePerGas"] = "0x0b";
    (*difficultyParams)["genesis"]["extraData"] = "0x00";
    (*difficultyParams)["genesis"]["timestamp"] = "0x00";
    (*difficultyParams)["genesis"]["nonce"] = "0x0000000000000000";
    (*difficultyParams)["genesis"]["mixHash"] = "0x0000000000000000000000000000000000000000000000000000000000020000";
    (*difficultyParams)["sealEngine"] = "NoProof";

    auto const& forkChainIDmap = Options::getCurrentConfig().getGenesisTemplateChainID();
    if (forkChainIDmap.count(_net))
        (*difficultyParams)["params"]["chainID"] = forkChainIDmap.at(_net)->asString();
    else
        (*difficultyParams)["params"]["chainID"] = VALUE(Options::getCurrentConfig().cfgFile().defaultChainID()).asString();
    return spSetChainParamsArgs(new SetChainParamsArgs(difficultyParams));
}

ToolChain::ToolChain(
    EthereumBlockState const& _parentBlock, EthereumBlockState const& _currentBlock,
    FORK const& _fork, fs::path const& _toolPath, fs::path const& _tmpDir)
   :m_initialParams(genT9NChainParams(_fork)),
    m_engine(SealEngine::NoProof),
    m_fork(new FORK(_fork.asString())),
    m_toolPath(_toolPath),
    m_tmpDir(_tmpDir)
{
    // Calculate the difficutly of _currentBlock given _parentBlock
    ToolResponse res = mineBlockOnTool(_currentBlock, _parentBlock, SealEngine::NoReward);
    m_blocks.push_back(_currentBlock);
    m_blocks.back().headerUnsafe().getContent().setDifficulty(res.currentDifficulty());
}


spDataObject const ToolChain::mineBlock(EthereumBlockState const& _pendingBlock, EthereumBlockState const& _parentBlock, Mining _req)
{
    // Ask the tool to calculate post state and block header
    // With current chain information, txs from pending block
    ToolResponse const res = mineBlockOnTool(_pendingBlock, _parentBlock, m_engine);
    ETH_DC_MESSAGE(
        DC::TESTLOG, "ToolChain::mineBlock of new block: " + BlockHeader::BlockTypeToString(_pendingBlock.header()->type()));

    // Pending fixed is pending header corrected by the information returned by tool
    // The tool can reject transactions changing the stateHash, TxRoot, TxReceipts, HeaderHash, GasUsed
    EthereumBlockState pendingFixed(_pendingBlock.header(), res.state(), res.logsHash());

    BlockHeader& pendingFixedHeader = pendingFixed.headerUnsafe().getContent();
    pendingFixedHeader.setNumber(m_blocks.size());
    correctHeaderByToolResponse(pendingFixedHeader, res);
    if (_pendingBlock.header()->type() != BlockType::BlockHeaderMerge)
    {
        pendingFixedHeader.setDifficulty(res.currentDifficulty());
        checkDifficultyAgainstRetesteth(res.currentDifficulty(), pendingFixed.header());
    }
    calculateAndSetBaseFee(pendingFixed.headerUnsafe(), lastBlock().header());

    spDataObject miningResult;
    miningResult = coorectTransactionsByToolResponse(res, pendingFixed, _pendingBlock, _req);
    correctUncleHeaders(pendingFixed, _pendingBlock);

    // Calculate header hash from header fields (does not recalc tx, un hashes)
    pendingFixed.headerUnsafe().getContent().recalculateHash();

    // Blockchain rules
    verifyEthereumBlockHeader(pendingFixed.header(), *this);
    additionalHeaderVerification(res, pendingFixed, _pendingBlock, _req);

    calculateAndSetTotalDifficulty(pendingFixed);

    pendingFixed.setTrsTrace(res.debugTrace());
    m_blocks.push_back(pendingFixed);
    return miningResult;
}

ToolResponse ToolChain::mineBlockOnTool(
    EthereumBlockState const& _currentBlock, EthereumBlockState const& _parentBlock, SealEngine _engine)
{
    BlockMining toolMiner(*this, _currentBlock, _parentBlock, _engine);
    toolMiner.prepareEnvFile();
    toolMiner.prepareAllocFile();
    toolMiner.prepareTxnFile();
    toolMiner.executeTransition();
    return toolMiner.readResult();
}

void ToolChain::rewindToBlock(size_t _number)
{
    while (m_blocks.size() > _number + 1)
        m_blocks.pop_back();
}

// Helper functions
void ToolChain::checkDifficultyAgainstRetesteth(VALUE const& _toolDifficulty, spBlockHeader const& _pendingHeader)
{
    // Calculate difficulty for tool (tool does not calculate difficulty)
    ChainOperationParams params = ChainOperationParams::defaultParams(toolParams());
    VALUE retestethDifficulty = calculateEthashDifficulty(params, _pendingHeader, lastBlock().header());
    if (_toolDifficulty != retestethDifficulty)
        ETH_ERROR_MESSAGE("tool vs retesteth difficulty disagree: " + _toolDifficulty.asDecString() + " vs " +
                          retestethDifficulty.asDecString());
}

void ToolChain::calculateAndSetBaseFee(spBlockHeader& _pendingHeader, spBlockHeader const& _parentHeader)
{
    bool isOn1559 = _pendingHeader.getCContent().type() == BlockType::BlockHeader1559 && _parentHeader->type() == BlockType::BlockHeader1559;
    bool isOn1559ToMerge = _pendingHeader.getCContent().type() == BlockType::BlockHeaderMerge && _parentHeader->type() == BlockType::BlockHeader1559;
    bool isOnMerge = _pendingHeader.getCContent().type() == BlockType::BlockHeaderMerge && _parentHeader->type() == BlockType::BlockHeaderMerge;

    // Calculate new baseFee
    if (isOn1559 || isOn1559ToMerge || isOnMerge)
    {
        ChainOperationParams params = ChainOperationParams::defaultParams(toolParams());
        BlockHeader1559& pendingFixed1559Header = BlockHeader1559::castFrom(_pendingHeader.getContent());
        VALUE baseFee = calculateEIP1559BaseFee(params, _pendingHeader, _parentHeader);
        pendingFixed1559Header.setBaseFee(baseFee);
    }
}

spDataObject ToolChain::coorectTransactionsByToolResponse(
    ToolResponse const& _res, EthereumBlockState& _pendingFixed, EthereumBlockState const& _pendingBlock, Mining _miningReq)
{
    // Add only those transactions which tool returned a receipt for
    // Some transactions are expected to fail. That should be detected by tests
    size_t index = 0;
    spDataObject miningResult;
    (*miningResult)["result"] = true;

    for (auto const& tr : _pendingBlock.transactions())
    {
        bool found = false;
        FH32 const trHash = tr->hash();
        for (auto const& trReceipt : _res.receipts())
        {
            if (trReceipt.trHash() == trHash)
            {
                found = true;
                _pendingFixed.addTransaction(tr);
                break;
            }
        }
        if (!found)
        {
            string const message = "t8ntool didn't return a transaction with hash: " + trHash.asString();

            // Find the rejected transaction information
            bool rejectedInfoFound = false;
            for (auto const& el : _res.rejected())
            {
                if (el.index() == index)
                {
                    rejectedInfoFound = true;
                    spDataObject rejectInfo;
                    (*rejectInfo)["hash"] = trHash.asString();
                    (*rejectInfo)["error"] = el.error();
                    (*miningResult)["rejectedTransactions"].addArrayObject(rejectInfo);
                    break;
                }
            }
            if (!rejectedInfoFound)
                ETH_ERROR_MESSAGE("tool didn't provide information about rejected transaction");
            if (_miningReq == Mining::AllowFailTransactions)
            {
                ETH_DC_MESSAGE(DC::LOWLOG, message);
            }
            else
                throw test::UpwardsException(message);
        }
        index++;
    }
    return miningResult;
}

void ToolChain::correctUncleHeaders(EthereumBlockState& _pendingFixed, EthereumBlockState const& _pendingBlock)
{
    // Treat all uncles as valid as t8ntool does not calculate uncles
    // Uncle header validity as well as RLP logic is checked before
    for (auto const& un : _pendingBlock.uncles())
    {
        verifyEthereumBlockHeader(un, *this);
        _pendingFixed.addUncle(un);
    }
    _pendingFixed.recalculateUncleHash();  // Rely that only uncle hash is recalculated (simulate t8ntool unclehash)
}

void ToolChain::additionalHeaderVerification(
    ToolResponse const& _res, EthereumBlockState& _pendingFixed, EthereumBlockState const& _pendingBlock, Mining _miningReq)
{
    // Require number from pending block to be equal to actual block number that is imported
    if (_pendingBlock.header()->number() != _pendingFixed.header()->number().asBigInt())
        throw test::UpwardsException(string("Block Number from pending block != actual chain height! (") +
                                     _pendingBlock.header()->number().asString() +
                                     " != " + _pendingFixed.header()->number().asString() + ")");

    if (_miningReq == Mining::RequireValid)  // called on rawRLP import
    {
        if (m_fork.getContent().asString() == "HomesteadToDaoAt5" && _pendingFixed.header()->number() > 4 &&
            _pendingFixed.header()->number() < 19 &&
            _pendingFixed.header()->extraData().asString() != "0x64616f2d686172642d666f726b")
            throw test::UpwardsException("Dao Extra Data required!");

        spDataObject const pendingH = _pendingBlock.header()->asDataObject();
        spDataObject const pendingFixedH = _pendingFixed.header()->asDataObject();
        if (pendingH->asJson(0, false) != pendingFixedH->asJson(0, false))
        {
            string errField;
            string const compare = compareBlockHeaders(pendingH, pendingFixedH, errField);
            throw test::UpwardsException(string("Block from pending block != t8ntool constructed block!\n") +
                                         "Error in field: " + errField + "\n" +
                                         "rawRLP/Pending header  vs  t8ntool header \n" + compare);
        }
    }

    if (_pendingFixed.header()->transactionRoot() != _res.txRoot())
    {
        ETH_ERROR_MESSAGE(string("ToolChain::mineBlock txRootHash is different to one ruturned by tool \n") +
                          "constructedBlockHash: " + _pendingFixed.header()->transactionRoot().asString() +
                          "\n toolTransactionRoot: " + _res.txRoot().asString());
    }
}

void ToolChain::calculateAndSetTotalDifficulty(EthereumBlockState& _pendingFixed)
{
    VALUE totalDifficulty(0);
    if (m_blocks.size() > 0)
        totalDifficulty = m_blocks.at(m_blocks.size() - 1).totalDifficulty();
    _pendingFixed.setTotalDifficulty(totalDifficulty + _pendingFixed.header()->difficulty());

    ETH_DC_MESSAGE(DC::TESTLOG, "New block N: " + to_string(m_blocks.size()));
    ETH_DC_MESSAGE(DC::TESTLOG, "New block TD: " + totalDifficulty.asDecString() + " + " +
                                    _pendingFixed.header()->difficulty().asDecString() + " = " +
                                    _pendingFixed.totalDifficulty().asDecString());
}
}  // namespace toolimpl
