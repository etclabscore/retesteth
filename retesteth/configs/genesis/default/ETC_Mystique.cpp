#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string default_ETC_Mystique_config = R"({
    "params" : {
       "homesteadForkBlock" : "0x00",
       "EIP150ForkBlock" : "0x00",
       "EIP158ForkBlock" : "0x00",
       "byzantiumForkBlock" : "0x00",
       "constantinopleForkBlock" : "0x00",
       "constantinopleFixForkBlock" : "0x00",
       "istanbulForkBlock" : "0x00",
       "berlinForkBlock" : "0x00",
       "londonForkBlock" : "0x00",
       "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

const string t8ntool_ETC_Mystique_config = R"({
    "params" : {
        "fork" : "ETC_Mystique",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },
    "accounts" : {
    }
})";

genETC_MystiqueCfg::genETC_MystiqueCfg()
{
    spDataObject obj;
    (*obj)["path"] = "besu/genesis/ETC_Mystique.json";
    (*obj)["content"] = default_ETC_Mystique_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "default/genesis/ETC_Mystique.json";
    (*obj2)["content"] = t8ntool_ETC_Mystique_config;
    map_configs.addArrayObject(obj2);
}
