#pragma once
#include <libdataobj/DataObject.h>

namespace retesteth::options
{
extern dataobject::DataObject map_configs;

// Attached clients config
class alethcfg { public: alethcfg(); };
class alethIpcDebugcfg { public: alethIpcDebugcfg(); };
class besucfg { public: besucfg(); };
class t8ntoolcfg { public: t8ntoolcfg(); };
class t8ntooleipcfg { public: t8ntooleipcfg(); };
class t8ntooletccfg { public: t8ntooletccfg(); };
class oewrapcfg { public: oewrapcfg(); };
class ethereumjscfg { public: ethereumjscfg(); };

// Genesis configs for clients
class genRewardsCfg { public: genRewardsCfg(); };
class genFrontierCfg { public: genFrontierCfg(); };
class genHomesteadCfg { public: genHomesteadCfg(); };
class genEIP150Cfg { public: genEIP150Cfg(); };
class genEIP158Cfg { public: genEIP158Cfg(); };
class genByzantiumCfg { public: genByzantiumCfg(); };
class genConstantinopleCfg { public: genConstantinopleCfg(); };
class genConstantinopleFixCfg { public: genConstantinopleFixCfg(); };
class genIstanbulCfg { public: genIstanbulCfg(); };
class genBerlinCfg { public: genBerlinCfg(); };
class genLondonCfg { public: genLondonCfg(); };
class genArrowGlacierCfg { public: genArrowGlacierCfg(); };
class genGrayGlacierCfg { public: genGrayGlacierCfg(); };
class genMergeCfg { public: genMergeCfg(); };
class genArrowGlacierToMergeAtDiffC0000Cfg { public: genArrowGlacierToMergeAtDiffC0000Cfg(); };

// T8ntooleip
class t8ntooleip_genRewardsCfg { public: t8ntooleip_genRewardsCfg(); };
class t8ntooleip_genLondon1884Cfg { public: t8ntooleip_genLondon1884Cfg(); };

// T8ntooletc
class t8ntooletc_genRewardsCfg { public: t8ntooletc_genRewardsCfg(); };
class t8ntooletc_genETC_AtlantisCfg { public: t8ntooletc_genETC_AtlantisCfg(); };
class t8ntooletc_genETC_AghartaCfg { public: t8ntooletc_genETC_AghartaCfg(); };
class t8ntooletc_genETC_PhoenixCfg { public: t8ntooletc_genETC_PhoenixCfg(); };
class t8ntooletc_genETC_MagnetoCfg { public: t8ntooletc_genETC_MagnetoCfg(); };
class t8ntooletc_genETC_MystiqueCfg { public: t8ntooletc_genETC_MystiqueCfg(); };

// Transition genesis configs
class genFrontierToHomesteadCfg { public: genFrontierToHomesteadCfg(); };
class genHomesteadToDaoCfg { public: genHomesteadToDaoCfg(); };
class genHomesteadToEIP150Cfg { public: genHomesteadToEIP150Cfg(); };
class genEIP158ToByzantiumCfg { public: genEIP158ToByzantiumCfg(); };
class genByzantiumToConstantinopleFixCfg { public: genByzantiumToConstantinopleFixCfg(); };
class genBerlinToLondonCfg { public: genBerlinToLondonCfg(); };

// Initializer
class OptionsInit
{
public:
    OptionsInit() {

        // Attached clients config
        alethcfg aleth;
        alethIpcDebugcfg alethIpcDebug;
        besucfg besu;
        t8ntoolcfg t8ntool;
        t8ntooletccfg t8ntooletc;
        t8ntooleipcfg t8ntooleip;
        oewrapcfg oewrap;
        ethereumjscfg ethereumjs;

        // Genesis configs for clients
        genRewardsCfg rewards;
        genFrontierCfg genFrontier;
        genHomesteadCfg genHomestead;
        genEIP150Cfg genEIP150;
        genEIP158Cfg genEIP158;
        genByzantiumCfg genByzantium;
        genConstantinopleCfg genConstantinople;
        genConstantinopleFixCfg genConstantinopleFix;
        genIstanbulCfg genIstanbuil;
        genBerlinCfg genBerlin;
        genLondonCfg genLondon;
        genArrowGlacierCfg genArrowGlacier;
        genGrayGlacierCfg genGrayGlacier;
        genMergeCfg genMerge;

        // T8ntooleip
        t8ntooleip_genRewardsCfg t8ntooleip_RewardsCfg;
        t8ntooleip_genLondon1884Cfg t8ntooleip_London1884Cfg;

        // T8ntooletc
        t8ntooletc_genRewardsCfg t8Ntooletc_RewardsCfg;
        t8ntooletc_genETC_AtlantisCfg t8ntooletc_ETC_AtlantisCfg;
        t8ntooletc_genETC_AghartaCfg t8ntooletc_ETC_AghartaCfg;
        t8ntooletc_genETC_PhoenixCfg t8ntooletc_ETC_PhoenixCfg;
        t8ntooletc_genETC_MagnetoCfg t8ntooletc_ETC_MagnetoCfg;
        t8ntooletc_genETC_MystiqueCfg t8ntooletc_ETC_MystiqueCfg;

        // Transition genesis configs
        genFrontierToHomesteadCfg genFrontierToHomestead;
        genHomesteadToDaoCfg genHomesteadToDao;
        genHomesteadToEIP150Cfg genHomesteadToEIP150;
        genEIP158ToByzantiumCfg genEIP158ToByzantium;
        genByzantiumToConstantinopleFixCfg genBezantiumToConstantinopleFix;
        genBerlinToLondonCfg genBerlinToLondon;
        genArrowGlacierToMergeAtDiffC0000Cfg genArrowGlacierToMergeAtDiffC0000;
    }
};
}  // namespace retesteth::options
