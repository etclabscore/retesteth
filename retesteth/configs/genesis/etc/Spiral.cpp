#include <retesteth/configs/Options.h>
using namespace std;
using namespace dataobject;

namespace retesteth::options
{
genSpiralCfgETC::genSpiralCfgETC()
{

const string rpc_Spiral_config = R"({
    "params" : {
        "homesteadForkBlock" : "0x00",
        "EIP150ForkBlock" : "0x00",
        "EIP158ForkBlock" : "0x00",
        "atlantisBlock" : "0x00",
        "aghartaBlock" : "0x00",
        "phoenixBlock" : "0x00",
        "magnetoBlock" : "0x00",
        "mystiqueBlock" : "0x00",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

const string etc_Spiral_config = R"({
    "params" : {
        "fork" : "ETC_Mystique",
        "chainID" : "0x3d"
    },
    "accounts" : {
    }
})";

    spDataObject obj;
    (*obj)["path"] = "besuetc/genesis/ETC_Spiral.json";
    (*obj)["content"] = rpc_Spiral_config;
    map_configs.addArrayObject(obj);

    spDataObject obj2;
    (*obj2)["path"] = "etc/genesis/ETC_Spiral.json";
    (*obj2)["content"] = etc_Spiral_config;
    map_configs.addArrayObject(obj2);
}
}  // namespace retesteth::options
