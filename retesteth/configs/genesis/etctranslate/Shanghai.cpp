#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genShanghaiCfgETCTR::genShanghaiCfgETCTR()
{
const string t8ntool_Shanghai_config = R"({
    "params" : {
        "fork" : "ETC_Spiral",
        "terminalTotalDifficulty" : "0x00",
        "constantinopleForkBlock" : "0x00",
        "byzantiumForkBlock" : "0x00",
        "homesteadForkBlock" : "0x00",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

    spDataObject obj2;
    (*obj2)["path"] = "etctranslate/genesis/Shanghai.json";
    (*obj2)["content"] = t8ntool_Shanghai_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
