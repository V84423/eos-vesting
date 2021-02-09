#include "common.hpp"

BOOST_AUTO_TEST_SUITE(eosdt_exchang_tests)

    BOOST_AUTO_TEST_CASE(investor_locked_base_test)
    try {
        TESTER chain;
        if (!test_necessary(chain, "investor_locked_base_test")) {
            return;
        }
        PRINT_LABEL(chain, "started");
        contract_set(chain);
        set_ctrsetting(chain);
        new_accounts(chain, {N(employer), N(investor)});

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                     mvo()("asset_contract",(get_contract_by_token(ORE_SYMBOL)).to_string())("asset_symbol",ORE_SYMBOL));

        token_issue(chain, N(employer),ORE(100'000));
        token_trans(chain, N(employer),CMKRYVESTING,ORE(100'000),"deposit");

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*370-12));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), CMKRYVESTING,
                     mvo()("account_id","investor")("vesting_period",5)("vesting_amount",ORE(100'000))
                             ("cliff_date","2020-06-15T13:23:00")("cliff_weight",0.0)
                             ("start_date","2020-06-15T13:23:00")("end_date","2021-06-15T13:23:00")
        );
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(claim), N(investor), mvo()
                ("vesting_id",0)
        , "Account: investor does not have quantity for claim, vesting_amount: 100000.0000 ORE, vested_amount: 0.0000 ORE, vesting: 0.0000 ORE");


        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*(370+161)+(13*3600+23*60-14)));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(investor),mvo()("vesting_id",0));
        BOOST_CHECK_EQUAL(balance_get(chain, CMKRYVESTING, ORE_SYMBOL), ORE(0));
        BOOST_CHECK_EQUAL(balance_get(chain, N(investor), ORE_SYMBOL), ORE(100'000));

        PRINT_LABEL(chain, "stopped");
    } FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()