#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::debug;
using namespace test::compiler;

BOOST_FIXTURE_TEST_SUITE(SOLCSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(solc_abi_example1)
{
    string abicode = test::compiler::utiles::encodeAbi("baz(uint32,bool) 69 1");
    string const exp =
        "0xcdcd77c0"
        "0000000000000000000000000000000000000000000000000000000000000045"
        "0000000000000000000000000000000000000000000000000000000000000001";
    ETH_DC_MESSAGE(DC::LOWLOG, exp);
    ETH_FAIL_REQUIRE(abicode == exp);
}

BOOST_AUTO_TEST_CASE(solc_abi_example2)
{
    string abicode = test::compiler::utiles::encodeAbi("bar(bytes3[2]) [\"abc\",\"def\"]");
    string const exp =
        "0xfce353f6"
        "6162630000000000000000000000000000000000000000000000000000000000"
        "6465660000000000000000000000000000000000000000000000000000000000";
    ETH_DC_MESSAGE(DC::LOWLOG, exp);
    ETH_FAIL_REQUIRE(abicode == exp);
}

BOOST_AUTO_TEST_CASE(solc_abi_example2a)
{
    string abicode = test::compiler::utiles::encodeAbi("bar(bytes3[2]) [\"a c\",\"d f\"]");
    string const exp =
        "0xfce353f6"
        "6120630000000000000000000000000000000000000000000000000000000000"
        "6420660000000000000000000000000000000000000000000000000000000000";
    ETH_DC_MESSAGE(DC::LOWLOG, exp);
    ETH_FAIL_REQUIRE(abicode == exp);
}

BOOST_AUTO_TEST_CASE(solc_abi_example2b)
{
    string abicode = test::compiler::utiles::encodeAbi("bar(bytes3[2]) [\"a c\", \"d f\"]");
    string const exp =
        "0xfce353f6"
        "6120630000000000000000000000000000000000000000000000000000000000"
        "6420660000000000000000000000000000000000000000000000000000000000";
    ETH_DC_MESSAGE(DC::LOWLOG, exp);
    ETH_FAIL_REQUIRE(abicode == exp);
}

BOOST_AUTO_TEST_CASE(solc_abi_example3)
{
    string abicode = test::compiler::utiles::encodeAbi("sam(bytes,bool,uint256[]) \"dave\" 1 [1,2,3]");
    string const exp =
        "0xa5643bf2"
        "0000000000000000000000000000000000000000000000000000000000000060"
        "0000000000000000000000000000000000000000000000000000000000000001"
        "00000000000000000000000000000000000000000000000000000000000000a0"
        "0000000000000000000000000000000000000000000000000000000000000004"
        "6461766500000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000003"
        "0000000000000000000000000000000000000000000000000000000000000001"
        "0000000000000000000000000000000000000000000000000000000000000002"
        "0000000000000000000000000000000000000000000000000000000000000003";
    ETH_DC_MESSAGE(DC::LOWLOG, exp);
    ETH_FAIL_REQUIRE(abicode == exp);
}

BOOST_AUTO_TEST_CASE(solc_abi_example4)
{
    string abicode = test::compiler::utiles::encodeAbi(
        "f(uint256,uint32[],bytes10,bytes) 0x123 [0x456, 0x789] \"1234567890\" \"Hello, world!\"");
    string const exp =
        "0x8be65246"
        "0000000000000000000000000000000000000000000000000000000000000123"
        "0000000000000000000000000000000000000000000000000000000000000080"
        "3132333435363738393000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000e0"
        "0000000000000000000000000000000000000000000000000000000000000002"
        "0000000000000000000000000000000000000000000000000000000000000456"
        "0000000000000000000000000000000000000000000000000000000000000789"
        "000000000000000000000000000000000000000000000000000000000000000d"
        "48656c6c6f2c20776f726c642100000000000000000000000000000000000000";
    ETH_DC_MESSAGE(DC::LOWLOG, exp);
    ETH_FAIL_REQUIRE(abicode == exp);
}


BOOST_AUTO_TEST_CASE(solc_abi_example5)
{
    string abicode = test::compiler::utiles::encodeAbi("g(uint256[][],string[]) [[1,2],[3]] [\"one\",\"two\",\"three\"]");
    ETH_FAIL_REQUIRE(abicode ==
                     "0x2289b18c"
                     "0000000000000000000000000000000000000000000000000000000000000040"
                     "0000000000000000000000000000000000000000000000000000000000000140"
                     "0000000000000000000000000000000000000000000000000000000000000002"
                     "0000000000000000000000000000000000000000000000000000000000000040"
                     "00000000000000000000000000000000000000000000000000000000000000a0"
                     "0000000000000000000000000000000000000000000000000000000000000002"
                     "0000000000000000000000000000000000000000000000000000000000000001"
                     "0000000000000000000000000000000000000000000000000000000000000002"
                     "0000000000000000000000000000000000000000000000000000000000000001"
                     "0000000000000000000000000000000000000000000000000000000000000003"
                     "0000000000000000000000000000000000000000000000000000000000000003"
                     "0000000000000000000000000000000000000000000000000000000000000060"
                     "00000000000000000000000000000000000000000000000000000000000000a0"
                     "00000000000000000000000000000000000000000000000000000000000000e0"
                     "0000000000000000000000000000000000000000000000000000000000000003"
                     "6f6e650000000000000000000000000000000000000000000000000000000000"
                     "0000000000000000000000000000000000000000000000000000000000000003"
                     "74776f0000000000000000000000000000000000000000000000000000000000"
                     "0000000000000000000000000000000000000000000000000000000000000005"
                     "7468726565000000000000000000000000000000000000000000000000000000");
}

BOOST_AUTO_TEST_CASE(solc_abi_GithubWikiTest)
{
    string abicode =
        test::compiler::utiles::encodeAbi("f(uint256,uint32[],bytes10,bytes) 291 [1110,1929] \"1234567890\" \"Hello, world!\"");
    ETH_FAIL_REQUIRE(abicode ==
                     "0x8be65246"
                     "0000000000000000000000000000000000000000000000000000000000000123"
                     "0000000000000000000000000000000000000000000000000000000000000080"
                     "3132333435363738393000000000000000000000000000000000000000000000"
                     "00000000000000000000000000000000000000000000000000000000000000e0"
                     "0000000000000000000000000000000000000000000000000000000000000002"
                     "0000000000000000000000000000000000000000000000000000000000000456"
                     "0000000000000000000000000000000000000000000000000000000000000789"
                     "000000000000000000000000000000000000000000000000000000000000000d"
                     "48656c6c6f2c20776f726c642100000000000000000000000000000000000000");
}

BOOST_AUTO_TEST_CASE(solc_abi_SingleInteger)
{
    string abicode = test::compiler::utiles::encodeAbi("f(uint256) 98127491");
    ETH_FAIL_REQUIRE(abicode ==
                     "0xb3de648b"
                     "0000000000000000000000000000000000000000000000000000000005d94e83");
}

BOOST_AUTO_TEST_CASE(solc_abi_IntegerAndAddress)
{
    string abicode =
        test::compiler::utiles::encodeAbi("f(uint256,address) 324124 \"0xcd2a3d9f938e13cd947ec05abc7fe734df8dd826\"");
    ETH_FAIL_REQUIRE(abicode ==
                     "0x50336458"
                     "000000000000000000000000000000000000000000000000000000000004f21c"
                     "000000000000000000000000cd2a3d9f938e13cd947ec05abc7fe734df8dd826");
}

BOOST_AUTO_TEST_SUITE_END()
