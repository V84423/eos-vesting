#include "common.hpp"

BOOST_AUTO_TEST_SUITE(eosdt_exchang_tests)

    BOOST_AUTO_TEST_CASE(vesting_base_test)
    try {
        TESTER chain;
        if (!test_necessary(chain, "vesting_base_test")) {
            return;
        }
        PRINT_LABEL(chain, "started");
        contract_set(chain);
        set_ctrsetting(chain);
        new_accounts(chain, {N(user),
                             N(usermonthly), N(userweekly), N(userdaily), N(usersecondly), N(userlocked)});
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                mvo()("asset_contract",(EOSCTRACT).to_string())("asset_symbol",EOS_SYMBOL));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                     mvo()("asset_contract",(EOSDTSTTOKEN).to_string())("asset_symbol",EOSDT_SYMBOL));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                     mvo()("asset_contract",(EOSDTNUTOKEN).to_string())("asset_symbol",NUT_SYMBOL));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                    mvo()("asset_contract",(N(dappservices)).to_string())("asset_symbol",DAPP_SYMBOL));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                     mvo()("asset_contract",(EOSDTKGTOKEN).to_string())("asset_symbol",KG_SYMBOL));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                     mvo()("asset_contract",(TETHERTETHER).to_string())("asset_symbol",USDT_SYMBOL));

        print_cmkasset(chain, EOS_SYMBOL);
        print_cmkasset(chain, EOSDT_SYMBOL);
        print_cmkasset(chain, NUT_SYMBOL);
        print_cmkasset(chain, DAPP_SYMBOL);
        print_cmkasset(chain, KG_SYMBOL);
        print_cmkasset(chain, USDT_SYMBOL);
        //[00:03]	{ create:4611686018.427300453 EOSDT, on: eosdtsttoken, issuer: eosdtcntract}
        //[00:03]	token:{  contract: eosdtsttoken , issuer: eosdtsttoken , max_supply: 0.000000000 EOSDT , supply: 0.000 000 000 EOSDT }
        //                    contract(eosdtsttoken).asset_symbol   (9,EOSDT) should have max supply on this
        //           "asset_contract":"eosdtsttoken","asset_symbol":"9,EOSDT"

        token_issue(chain, N(user),EOS(1000));
        token_issue(chain, N(user),EOSDT(1000));
        token_issue(chain, N(user),NUT(1000));
        //token_issue(chain, N(user),DAPP(1000));
        token_issue(chain, N(user),KG(1000));
        token_issue(chain, N(user),EOSDT(1000));
        token_issue(chain, CMKRYVESTING,EOS(1000),"init");
        token_issue(chain, CMKRYVESTING,EOS(1000),"deposit");

        token_trans(chain, N(user),CMKRYVESTING,EOS(1000),"deposit");
        token_trans(chain, N(user),CMKRYVESTING,EOSDT(1000),"deposit");
        token_trans(chain, N(user),CMKRYVESTING,NUT(1000),"deposit");
        //token_trans(chain, N(user),CMKRYVESTING,DAPP(1000),"deposit");
        token_trans(chain, N(user),CMKRYVESTING,KG(1000),"deposit");
        token_trans(chain, N(user),CMKRYVESTING,EOSDT(1000),"deposit");

        print_cmkasset(chain, EOS_SYMBOL);
        PUSH_CONSOLE(chain, CMKRYVESTING, N(withdraw), CMKRYVESTING,
                     mvo()("to","user")("quantity",EOS(1000)));
        BOOST_CHECK_EQUAL(balance_get(chain, CMKRYVESTING, EOS_SYMBOL), EOS(2000));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), CMKRYVESTING,
                     mvo()("account_id","usermonthly")("vesting_period",1)("vesting_amount",EOS(100))
                             ("cliff_date","2021-02-01T00:00:00")("cliff_weight",0.5)
                             ("start_date","2020-02-01T00:00:00")("end_date","2022-02-01T00:00:00")
                     );
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), CMKRYVESTING,
                     mvo()("account_id","userweekly")("vesting_period",2)("vesting_amount",EOS(100))
                             ("cliff_date","2021-02-01T00:00:00")("cliff_weight",0.5)
                             ("start_date","2020-02-03T00:00:00")("end_date","2022-01-31T00:00:00")
        );
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), CMKRYVESTING,
                     mvo()("account_id","userdaily")("vesting_period",3)("vesting_amount",EOS(100))
                             ("cliff_date","2021-02-01T00:00:00")("cliff_weight",0.5)
                             ("start_date","2020-02-01T00:00:00")("end_date","2022-02-01T00:00:00")
        );
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), CMKRYVESTING,
                     mvo()("account_id","usersecondly")("vesting_period",4)("vesting_amount",EOS(100))
                             ("cliff_date","2021-02-01T00:00:00")("cliff_weight",0.5)
                             ("start_date","2020-02-01T01:02:03")("end_date","2022-02-01T00:00:00")
        );
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), CMKRYVESTING,
                     mvo()("account_id","userlocked")("vesting_period",5)("vesting_amount",EOS(100))
                             ("cliff_date","2021-02-01T00:00:00")("cliff_weight",0.5)
                             ("start_date","2020-02-07T01:02:03")("end_date","2022-02-13T04:05:06")
        );
        PUSH_CONSOLE(chain, CMKRYVESTING, N(pausevesting), CMKRYVESTING,mvo()("vesting_id",0)("pause_start","2020-01-01T00:00:25"));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(pausevesting), CMKRYVESTING,mvo()("vesting_id",1)("pause_start","2020-01-01T00:00:25"));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(pausevesting), CMKRYVESTING,mvo()("vesting_id",2)("pause_start","2020-01-01T00:00:26"));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(pausevesting), CMKRYVESTING,mvo()("vesting_id",3)("pause_start","2020-01-01T00:00:26"));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(pausevesting), CMKRYVESTING,mvo()("vesting_id",4)("pause_start","2020-01-01T00:00:27"));

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(unpausevestng), CMKRYVESTING,mvo()("vesting_id",0)("pause_end","2020-01-02T00:00:28"));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(unpausevestng), CMKRYVESTING,mvo()("vesting_id",1)("pause_end","2020-01-02T00:00:28"));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(unpausevestng), CMKRYVESTING,mvo()("vesting_id",2)("pause_end","2020-01-02T00:00:30"));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(unpausevestng), CMKRYVESTING,mvo()("vesting_id",3)("pause_end","2020-01-02T00:00:30"));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(unpausevestng), CMKRYVESTING,mvo()("vesting_id",4)("pause_end","2020-01-02T00:00:31"));

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*30*27));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(usermonthly),mvo()("vesting_id",0));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(userweekly),mvo()("vesting_id",1));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(userdaily),mvo()("vesting_id",2));
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(usersecondly),mvo()("vesting_id",3));
        //PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(userlocked),mvo()("vesting_id",4));


        PRINT_LABEL(chain, "stopped");
    } FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()