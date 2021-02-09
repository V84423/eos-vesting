#include "common.hpp"

BOOST_AUTO_TEST_SUITE(eosdt_exchang_tests)

    BOOST_AUTO_TEST_CASE(vesting_2y_fired_2y_base_test)
    try {
        TESTER chain;
        if (!test_necessary(chain, "vesting_2y_fired_2y_base_test")) {
            return;
        }
        PRINT_LABEL(chain, "started");
        contract_set(chain);
        set_ctrsetting(chain);
        new_accounts(chain, {N(employer), N(employee)});

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                     mvo()("asset_contract",(get_contract_by_token(ORE_SYMBOL)).to_string())("asset_symbol",ORE_SYMBOL));

        token_issue(chain, N(employer),ORE(24'000));
        token_trans(chain, N(employer),CMKRYVESTING,ORE(24'000),"deposit");
        auto asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(24'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(24'000));

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*370-12));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), CMKRYVESTING,
                     mvo()("account_id","employee")("vesting_period",1)("vesting_amount",ORE(24000))
                             ("cliff_date","2020-01-01T00:00:00")("cliff_weight",0.0)
                             ("start_date","2020-01-01T00:00:00")("end_date","2022-01-01T00:00:00")
        );
        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(24000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(0));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(employee),mvo()("vesting_id",0));
        BOOST_CHECK_EQUAL(balance_get(chain, CMKRYVESTING, ORE_SYMBOL), ORE(12000));
        BOOST_CHECK_EQUAL(balance_get(chain, N(employee), ORE_SYMBOL), ORE(12000));
        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(12000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(0));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(cancelvesting), CMKRYVESTING,
                mvo()("vesting_id",0)("cancel_start","2021-02-02T00:00:00"));
        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(12000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(11000));

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*(370+30)-12));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(employee),mvo()("vesting_id",0));
        BOOST_CHECK_EQUAL(balance_get(chain, CMKRYVESTING, ORE_SYMBOL), ORE(11000));
        BOOST_CHECK_EQUAL(balance_get(chain, N(employee), ORE_SYMBOL), ORE(13000));
        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(11000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(11000));

        PRINT_LABEL(chain, "stopped");
    } FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()