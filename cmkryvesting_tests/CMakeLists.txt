file(GLOB UNIT_TESTS "*.cpp")
set(UNIT_TESTS "main.cpp" "vesting_contract_test.cpp")
add_executable(cmkryvesting_tests ${UNIT_TESTS})
target_link_libraries(cmkryvesting_tests eosio_chain eosio_testing)






