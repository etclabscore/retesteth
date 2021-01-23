#pragma once
#include "../../../basetypes.h"
#include "TransactionInGeneralSection.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
// Base logic for State Test Transaction section
struct StateTestTransactionBase : GCP_SPointerBase
{
    DataObject const asDataObject() const;
    std::vector<TransactionInGeneralSection> buildTransactions() const;

    struct Databox
    {
        Databox(BYTES const& _data, string const& _label, string const& _rawPreview)
          : m_data(_data), m_dataLabel(_label), m_dataRawPreview(_rawPreview)
        {}
        BYTES m_data;
        string m_dataLabel;
        string m_dataRawPreview;  // The source code preview before code compilation
    };

    std::vector<Databox> const& databoxVector() const { return m_databox; };

protected:
    StateTestTransactionBase(){};
    std::vector<Databox> m_databox;
    std::vector<VALUE> m_gasLimit;
    std::vector<VALUE> m_value;
    spVALUE m_gasPrice;
    spVALUE m_nonce;
    spFH32 m_secretKey;
    spFH20 m_to;
    bool m_creation;
};


}  // namespace teststruct
}  // namespace test
