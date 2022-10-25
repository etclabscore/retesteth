#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string t8ntooletc_ETC_Mystique_config = R"({
    "params" : {
        "fork" : "ETC_Mystique",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00"
    },

    "accounts" : {
    }
})";

t8ntooletc_genETC_MystiqueCfg::t8ntooletc_genETC_MystiqueCfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "t8ntooletc/genesis/ETC_Mystique.json";
    (*obj2)["content"] = t8ntooletc_ETC_Mystique_config;
    map_configs.addArrayObject(obj2);
}
