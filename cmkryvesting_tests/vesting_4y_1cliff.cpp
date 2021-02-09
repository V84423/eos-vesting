#include "common.hpp"

BOOST_AUTO_TEST_SUITE(eosdt_exchang_tests)

    BOOST_AUTO_TEST_CASE(vesting_4y_1cliff)
    try {
        TESTER chain;
        if (!test_necessary(chain, "vesting_4y_1cliff")) {
            return;
        }
        PRINT_LABEL(chain, "started");
        contract_set(chain);
        set_ctrsetting(chain);
        new_accounts(chain, {N(employer), N(employee)});

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                     mvo()("asset_contract",(get_contract_by_token(ORE_SYMBOL)).to_string())("asset_symbol",ORE_SYMBOL));

        token_issue(chain, N(employer),ORE(48000));
        token_trans(chain, N(employer),CMKRYVESTING,ORE(48000),"deposit");

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*370-12));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), CMKRYVESTING,
                     mvo()("account_id","employee")("vesting_period",1)("vesting_amount",ORE(48000))
                             ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                             ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00")
        );
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(employee),mvo()("vesting_id",0));
        BOOST_CHECK_EQUAL(balance_get(chain, CMKRYVESTING, ORE_SYMBOL), ORE(29000));
        BOOST_CHECK_EQUAL(balance_get(chain, N(employee), ORE_SYMBOL), ORE(19000));

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*(370+361)-13));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(employee),mvo()("vesting_id",0));
        BOOST_CHECK_EQUAL(balance_get(chain, CMKRYVESTING, ORE_SYMBOL), ORE(17000));
        BOOST_CHECK_EQUAL(balance_get(chain, N(employee), ORE_SYMBOL), ORE(31000));

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*(370+365+365)-14));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(employee),mvo()("vesting_id",0));
        BOOST_CHECK_EQUAL(balance_get(chain, CMKRYVESTING, ORE_SYMBOL), ORE(5000));
        BOOST_CHECK_EQUAL(balance_get(chain, N(employee), ORE_SYMBOL), ORE(43000));

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*(370+365+365+151)-15));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(employee),mvo()("vesting_id",0));
        BOOST_CHECK_EQUAL(balance_get(chain, CMKRYVESTING, ORE_SYMBOL), ORE(0));
        BOOST_CHECK_EQUAL(balance_get(chain, N(employee), ORE_SYMBOL), ORE(48000));

        PRINT_LABEL(chain, "stopped");
    } FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()