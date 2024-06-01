#include <retesteth/configs/Options.h>
#include <string>
using namespace std;
using namespace dataobject;

const string t8ntooletc_ETC_Phoenix_config = R"({
    "params" : {
        "fork" : "ETC_Phoenix",
        "chainID" : "0x3d",
        "homesteadForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "constantinopleForkBlock" : "0x00"
    },

    "accounts" : {
    }
})";

t8ntooletc_genETC_PhoenixCfg::t8ntooletc_genETC_PhoenixCfg()
{
    spDataObject obj2;
    (*obj2)["path"] = "t8ntooletc/genesis/ETC_Phoenix.json";
    (*obj2)["content"] = t8ntooletc_ETC_Phoenix_config;
    map_configs.addArrayObject(obj2);
}
