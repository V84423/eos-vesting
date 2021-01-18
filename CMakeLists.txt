cmake_minimum_required(VERSION 3.5)
project(contracts NONE)
set(CMAKE_BUILD_TYPE Release)
set(EOSIO_WASM_OLD_BEHAVIOR "On")
list(APPEND CMAKE_PREFIX_PATH "~/opt/eosio.cdt")
add_subdirectory(eosdtcntract)
add_subdirectory(eosdtexchang)
add_subdirectory(eosdtgovernc)
add_subdirectory(equiguardian)
add_subdirectory(eosdtliqdatr)
add_subdirectory(eosdtlockupp)
add_subdirectory(eosdtnutoken)
add_subdirectory(eosdtorclize)
add_subdirectory(eosdtreserve)
add_subdirectory(eosdtsttoken)
add_subdirectory(eosdttestest)
add_subdirectory(fakeeliqdatr)
add_subdirectory(eosdefivotes)
add_subdirectory(eosdtarmseos)
add_subdirectory(eos2dtdotcom)
add_subdirectory(eosdtposcont)
add_subdirectory(eosdtkgtoken)
add_subdirectory(eosdtontoken)
add_subdirectory(eosdtsavings)
add_subdirectory(fakeennewdex)
add_subdirectory(fakeelloantp)
add_subdirectory(tokenswap.eq)
add_subdirectory(eoslotteryeq)
add_subdirectory(cmkryvesting)
add_custom_target(eosdt)
add_dependencies(eosdt
        eosdtcntract.wasm
        eosdtexchang.wasm
        eosdtgovernc.wasm
        equiguardian.wasm
        eosdtliqdatr.wasm
        eosdtlockupp.wasm
        eosdtnutoken.wasm
        eosdtorclize.wasm
        eosdtreserve.wasm
        eosdtsttoken.wasm
        eosdttestest.wasm
        fakeeliqdatr.wasm
        eosdefivotes.wasm
        eosdtarmseos.wasm
        eos2dtdotcom.wasm
        eosdtposcont.wasm
        eosdtkgtoken.wasm
        eosdtontoken.wasm
        eosdtsavings.wasm
        fakeennewdex.wasm
        fakeelloantp.wasm
        tokenswap.eq.wasm
        eoslotteryeq.wasm
        ##cmkryvesting.wasm
        )
