#!/usr/bin/env bash

set -x
set -e

# One-time run:
# Build the docker image.
# Comment after running once.
# It takes a while.
# ./dretesteth.sh build

# Now dretesteth.sh should act like retesteth.

# FIXME: set to fit your local environment. Where are the ethereum/tests? 
testspath=/home/ia/dev/ethereum/tests

# Run a single test.
# ./dretesteth.sh -t GeneralStateTests/stArgsZeroOneBalance -- --testpath "${testspath}" --clients t8ntool --singletest addmodNonConst

# Run a single test, alternate command.
# NOTE: It is not clear what the root directory of the 'testfile' argument should be. "tests/"? The absolute path to the tests/ directory?
#       The former works for me as expected, the latter seems to cause ./dretesteth to run eternally (or a long time). 
# ./dretesteth.sh -t GeneralStateTests -- --testpath "${testspath}" --clients t8ntool --testfile "tests/GeneralStateTests/stExample/add11.json"

# Run a suite of tests.
# ./dretesteth.sh -t GeneralStateTests/stArgsZeroOneBalance -- --testpath "${testspath}" --clients t8ntool

# Fill a test for a specific fork.
# ./dretesteth.sh -t GeneralStateTests/stArgsZeroOneBalance -- --filltests --testpath "${testspath}" --clients t8ntool --singletest addmodNonConst --singlenet London
# ./dretesteth.sh -t GeneralStateTests/stArgsZeroOneBalance -- --filltests --testpath "${testspath}" --clients t8ntool --singletest addmodNonConst --singlenet ETC_Mystique

# Fill tests for a suite of tests, still for a single fork.
# ./dretesteth.sh -t GeneralStateTests/stArgsZeroOneBalance -- --filltests --testpath "${testspath}" --clients t8ntool --singlenet ETC_Mystique
./dretesteth.sh -t GeneralStateTests -- --filltests --testpath "${testspath}" --clients t8ntool --singlenet ETC_Mystique

# NOTE: These tests can be run by file with the following evm command, as an example:
# for f in ~/dev/ethereum/tests/GeneralStateTests/stArgsZeroOneBalance/*; do echo $f; ./build/bin/evm statetest $f; done




