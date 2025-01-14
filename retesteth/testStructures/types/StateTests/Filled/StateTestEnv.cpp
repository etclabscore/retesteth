#include "StateTestEnv.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;

namespace
{
void requireStateTestEnvScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestEnv " + _data.getKey(),
        {{"currentCoinbase", {{DataType::String}, jsonField::Required}},
            {"currentDifficulty", {{DataType::String}, jsonField::Required}},
            {"currentBaseFee", {{DataType::String}, jsonField::Optional}},
            {"currentRandom", {{DataType::String}, jsonField::Optional}},
            {"currentGasLimit", {{DataType::String}, jsonField::Required}},
            {"currentNumber", {{DataType::String}, jsonField::Required}},
            {"currentTimestamp", {{DataType::String}, jsonField::Required}},
            {"previousHash", {{DataType::String}, jsonField::Required}}});
}

}  // namespace


namespace test::teststruct
{
void StateTestEnv::initializeFields(DataObject const& _data)
{
    m_currentCoinbase = sFH20(_data.atKey("currentCoinbase"));
    m_currentNumber = sVALUE(_data.atKey("currentNumber"));
    m_currentGasLimit = sVALUE(_data.atKey("currentGasLimit"));

    // Indicates first block timestamp in StateTests
    m_currentTimestamp = sVALUE(_data.atKey("currentTimestamp"));
    // Indicates zero block timestamp in StateTests
    m_genesisTimestamp = sVALUE(0);
    m_previousHash = sFH32(_data.atKey("previousHash"));

    DataObject tmpD;
    tmpD = "0x00";  // State Tests extra data is 0x00
    m_currentExtraData = sBYTES(tmpD);
    m_currentNonce = spFH8(FH8::zero().copy());
    m_currentMixHash = spFH32(FH32::zero().copy());

    // 1559
    m_currentDifficulty = sVALUE(DataObject("0x00"));
    m_currentBaseFee = sVALUE(DataObject("0x0a"));

    if (_data.count("currentBaseFee"))
        m_currentBaseFee = sVALUE(_data.atKey("currentBaseFee"));
    if (_data.count("currentDifficulty"))
        m_currentDifficulty = sVALUE(_data.atKey("currentDifficulty"));

    // Merge
    m_currentRandom = spFH32(FH32::zero().copy());
    if (_data.count("currentRandom"))
        m_currentRandom = sFH32(_data.atKey("currentRandom"));

    m_currentWithdrawalsRoot = sFH32(DataObject(C_WITHDRAWALS_EMPTY_ROOT));
    m_currentExcessDataGas = sVALUE(0);
}


StateTestEnv::StateTestEnv(DataObject const& _data)
{
    try
    {
        requireStateTestEnvScheme(_data);
        initializeFields(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestEnv parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
