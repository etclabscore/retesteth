#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string t8ntooletc_ETC_Atlantis_config = R"({
    "params" : {
        "fork" : "ETC_Atlantis",
        "homesteadForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00"
    },

    "accounts" : {
    }
})";

t8ntooletc_genETC_AtlantisCfg::t8ntooletc_genETC_AtlantisCfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "t8ntooletc/genesis/ETC_Atlantis.json";
    (*obj2)["content"] = t8ntooletc_ETC_Atlantis_config;
    map_configs.addArrayObject(obj2);
}
