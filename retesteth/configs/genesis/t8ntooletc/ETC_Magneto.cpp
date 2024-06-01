#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string t8ntooletc_ETC_Magneto_config = R"({
    "params" : {
        "fork" : "ETC_Magneto",
        "chainID" : "0x3d",
        "homesteadForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "constantinopleForkBlock" : "0x00"
    },

    "accounts" : {
    }
})";

t8ntooletc_genETC_MagnetoCfg::t8ntooletc_genETC_MagnetoCfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "t8ntooletc/genesis/ETC_Magneto.json";
    (*obj2)["content"] = t8ntooletc_ETC_Magneto_config;
    map_configs.addArrayObject(obj2);
}
