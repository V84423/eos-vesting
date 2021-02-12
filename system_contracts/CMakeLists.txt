cmake_minimum_required(VERSION 3.5)
project(basic_contracts NONE)
set(CMAKE_BUILD_TYPE Release)
set(EOSIO_WASM_OLD_BEHAVIOR "On")
list(APPEND CMAKE_PREFIX_PATH "~/opt/eosio.cdt")
add_subdirectory(eosio.bios)
add_subdirectory(eosio.msig)
add_subdirectory(eosio.system)
add_subdirectory(eosio.token)
add_subdirectory(eosio.wrap)
add_subdirectory(dappservices)
add_subdirectory(delphioracle)
add_custom_target(basic_contracts)
add_dependencies(basic_contracts
        eosio.bios.wasm
        eosio.msig.wasm
        eosio.system.wasm
        eosio.token.wasm
        eosio.wrap.wasm
        dappservices.wasm
        delphioracle.wasm
        )