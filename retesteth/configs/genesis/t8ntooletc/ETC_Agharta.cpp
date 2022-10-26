#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string t8ntooletc_ETC_Agharta_config = R"({
    "params" : {
        "fork" : "ETC_Agharta",
        "chainID" : "0x3d",
        "homesteadForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "constantinopleForkBlock" : "0x00"
    },

    "accounts" : {
    }
})";

t8ntooletc_genETC_AghartaCfg::t8ntooletc_genETC_AghartaCfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "t8ntooletc/genesis/ETC_Agharta.json";
    (*obj2)["content"] = t8ntooletc_ETC_Agharta_config;
    map_configs.addArrayObject(obj2);
}
