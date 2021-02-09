#include "common.hpp"

BOOST_AUTO_TEST_SUITE(eosdt_exchang_tests)

    BOOST_AUTO_TEST_CASE(vesting_contract_test)
    try {
        TESTER chain;
        if (!test_necessary(chain, "vesting_contract_test")) {
            return;
        }
        PRINT_LABEL(chain, "started");
        contract_set(chain);
        set_ctrsetting(chain);
        new_accounts(chain, {N(userone), N(usertwo), N(userthree), N(userfour), N(userfive),
                             N(usersix), N(userseven), N(usereight), N(usernine), N(userten)});
        auto oretoken_str = (get_contract_by_token(ORE_SYMBOL)).to_string();

        //addasset
        BOOST_CHECK_THROW(chain.push_action(CMKRYVESTING, N(addasset), N(userone),
                                            mvo()("asset_contract", oretoken_str)("asset_symbol", ORE_SYMBOL)),
                          missing_auth_exception);

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addasset), CMKRYVESTING, mvo()
                ("asset_contract", "oretoken1")("asset_symbol", ORE_SYMBOL),
                       "asset_symbol(4,ORE) should have max supply on this contract(oretoken1).");

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addasset), CMKRYVESTING, mvo()
                ("asset_contract", oretoken_str)("asset_symbol", ds_symbol(5, ORE_SYMBOL_STR)),
                       "asset_symbol(5,ORE) should have max supply on this contract(eosio.token).");

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addasset), CMKRYVESTING, mvo()
                ("asset_contract", oretoken_str)("asset_symbol", ds_symbol(ORE_SYMBOL_DECIMAL, "ORT")),
                       "asset_symbol(4,ORT) should have max supply on this contract(eosio.token).");

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addasset), CMKRYVESTING,
                     mvo()("asset_contract", oretoken_str)("asset_symbol", ORE_SYMBOL));

        auto asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(0));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(0));

        //deposit
        vector<permission_level> auths_manager;
        auths_manager.push_back(permission_level{N(manager), config::active_name});
        auths_manager.push_back(permission_level{CMKRYVESTING, config::active_name});

        token_issue(chain, N(userthree), ORE(50'000));
        token_issue(chain, N(manager), EOSDT(50'000));
        token_issue(chain, N(manager), ORE(50'000));

        BOOST_CHECK_THROW(chain.push_action(EOSCTRACT, N(transfer), N(userthree), mvo()("from", "userone")
                ("to", CMKRYVESTING_STR)("quantity", ORE(50'000))("memo", "deposit")), missing_auth_exception);

        PUSH_EXCEPTION(chain, EOSDTSTTOKEN, N(transfer), auths_manager, mvo()("from", "manager")
        ("to", CMKRYVESTING_STR)("quantity", EOSDT(50'000))("memo", "deposit"),
                       "Symbol(9,EOSDT) does not exist in cmkassets.");

        //token_trans(chain, N(manager), CMKRYVESTING, ORE(50'000), "deposit");
        PUSH_CONSOLE(chain, EOSCTRACT, N(transfer), auths_manager,mvo()("from", "manager")
        ("to", CMKRYVESTING_STR)("quantity", ORE(50'000))("memo", "deposit"));
        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(50'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(50'000));

        //withdraw
        BOOST_CHECK_THROW(chain.push_action(CMKRYVESTING, N(withdraw), N(userone),
                mvo()("to", "usertwo")("quantity", ORE(10'000))), missing_auth_exception);

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(withdraw), auths_manager, mvo()
                ("to", "usertwo")("quantity", EOSDT(10'000)),
                       "Symbol(9,EOSDT) does not exist in cmkassets.");

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(withdraw), auths_manager, mvo()
                ("to", "usertwo")("quantity", ORE(60'000)),
                       "Wrong asset_balance(60000.0000 ORE) expected less or equal: '50000.0000 ORE'");

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(withdraw), auths_manager, mvo()
                ("to", "usereleven")("quantity", ORE(2'000)),
                       "To 'usereleven' does not exist as an account.");

        PUSH_CONSOLE(chain, CMKRYVESTING, N(withdraw), auths_manager, mvo()
                ("to", "userfour")("quantity", ORE(2'000)));

        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(48'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(48'000));
        //STEP:2
        //Checks for add vesting
        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*376-21));
        //82
        BOOST_CHECK_THROW(chain.push_action(CMKRYVESTING, N(addvesting), N(userone),
                                  mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                          ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                          ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00")), missing_auth_exception);

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                     mvo()("account_id","usereleven")("vesting_period",1)("vesting_amount",ORE(48'000))
                             ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                             ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00"),
        "to 'usereleven' does not exist as an account.");
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",7)("vesting_amount",ORE(48'000))
                               ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00"),
                       "Wrong vesting period(7) for 'userone' expected one from {1,2,3,4,5,6}");
        //108
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(0))
                               ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00"),
                       "Wrong vesting amount(0.0000 ORE) for 'userone' expected more than zero.");
        //116
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                               ("cliff_date","2019-05-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00"),
                       "Wrong cliff(20190501 00:00:00) expected more or equal than start(20190601 00:00:00), vesting_period: Monthly.");

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                               ("cliff_date","2020-06-01T00:00:00")("cliff_weight",-0.000001)
                               ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00"),
                       "Wrong cliff weight(-0.0000010000000001) for 'userone' expected more than zero and less or equal one");
        //132
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                               ("cliff_date","2020-06-01T00:00:00")("cliff_weight",1.1)
                               ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00"),
                       "Wrong cliff weight(1.1000000000000001) for 'userone' expected more than zero and less or equal one");
        //140
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                               ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","2021-06-01T00:00:00")("end_date","2023-06-01T00:00:00"),
                       "Wrong cliff(20200601 00:00:00) expected more or equal than start(20210601 00:00:00), vesting_period: Monthly.");
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                               ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","2019-06-01T00:00:00")("end_date","2019-05-01T00:00:00"),
                       "Wrong end(20190501 00:00:00) expected more than now(20210111 00:00:00), vesting_period: Monthly.");
        //156
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                               ("cliff_date","1970-01-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","1970-01-01T00:00:00")("end_date","1970-01-01T00:00:00"),
                       "Wrong end(19700101 00:00:00) expected more than now(20210111 00:00:01), vesting_period: Monthly.");
        //164
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                               ("cliff_date","1970-01-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","1970-01-01T00:00:00")("end_date","2021-01-01T00:00:00"),
                       "Wrong end(20210101 00:00:00) expected more than now(20210111 00:00:01), vesting_period: Monthly.");
        //171
        PUSH_EXCEPTION(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(60'000))
                               ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00"),
                       "Wrong unallocated_balance(60000.0000 ORE) expected less or equal: '48000.0000 ORE'");

        //STEP:3
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), auths_manager,
                       mvo()("account_id","userone")("vesting_period",1)("vesting_amount",ORE(48'000))
                               ("cliff_date","2020-06-01T00:00:00")("cliff_weight",0.25)
                               ("start_date","2019-06-01T00:00:00")("end_date","2023-06-01T00:00:00"));

        auto vest = print_cmkvesting(chain, 0);
        BOOST_CHECK_EQUAL(vest.vesting_id, 0);
        BOOST_CHECK_EQUAL(vest.account_id, N(userone));
        BOOST_CHECK_EQUAL(vest.vesting_period, 1);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(48'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "2020-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0.25);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "2019-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2023-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 0);
        BOOST_CHECK_EQUAL(vest.status, 1);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(0));

        BOOST_CHECK_THROW(chain.push_action(CMKRYVESTING, N(claim), N(usertwo),
                                            mvo()("vesting_id",0)), missing_auth_exception);

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(claim), N(userone),
                       mvo()("vesting_id","1"),
                       "Vesting(1) does not exist in cmkvestings.");

        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(userone),
                       mvo()("vesting_id","0"));

        BOOST_CHECK_EQUAL(balance_get(chain, N(userone), ORE_SYMBOL), ORE(19000));

        vest = print_cmkvesting(chain, 0);
        BOOST_CHECK_EQUAL(vest.vesting_id, 0);
        BOOST_CHECK_EQUAL(vest.account_id, N(userone));
        BOOST_CHECK_EQUAL(vest.vesting_period, 1);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(48'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "2020-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0.25);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "2019-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2023-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 0);
        BOOST_CHECK_EQUAL(vest.status, 1);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(19'000));

        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(29'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(0));

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(claim), N(userone),
                       mvo()("vesting_id","0"),
                       "Account: userone does not have quantity for claim, vesting_amount: 48000.0000 ORE, vested_amount: 19000.0000 ORE, vesting: 19000.0000 ORE");

        BOOST_CHECK_EQUAL(balance_get(chain, N(userone), ORE_SYMBOL), ORE(19000));

        //step:4
        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*(376+31)-24));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(userone),
                     mvo()("vesting_id","0"));

        BOOST_CHECK_EQUAL(balance_get(chain, N(userone), ORE_SYMBOL), ORE(20000));

        vest = print_cmkvesting(chain, 0);
        BOOST_CHECK_EQUAL(vest.vesting_id, 0);
        BOOST_CHECK_EQUAL(vest.account_id, N(userone));
        BOOST_CHECK_EQUAL(vest.vesting_period, 1);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(48'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "2020-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0.25);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "2019-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2023-06-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 0);
        BOOST_CHECK_EQUAL(vest.status, 1);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(20'000));

        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(28'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(0));
        //STEP:5
        //check lockup
        token_issue(chain, N(manager), ORE(100'000));
        PUSH_CONSOLE(chain, EOSCTRACT, N(transfer), auths_manager,mvo()("from", "manager")
                ("to", CMKRYVESTING_STR)("quantity", ORE(100'000))("memo", "deposit"));
        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(128'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(100'000));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), auths_manager,
                     mvo()("account_id","usertwo")("vesting_period",5)("vesting_amount",ORE(100'000))
                             ("cliff_date","1970-01-01T00:00:00")("cliff_weight",0)
                             ("start_date","1970-01-01T00:00:00")("end_date","2021-03-01T00:00:00"));

        vest = print_cmkvesting(chain, 1);
        BOOST_CHECK_EQUAL(vest.vesting_id, 1);
        BOOST_CHECK_EQUAL(vest.account_id, N(usertwo));
        BOOST_CHECK_EQUAL(vest.vesting_period, 5);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(100'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2021-03-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 0);
        BOOST_CHECK_EQUAL(vest.status, 1);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(0));

        PUSH_EXCEPTION(chain, CMKRYVESTING, N(claim), N(usertwo),
                       mvo()("vesting_id","1"),
                       "Account: usertwo does not have quantity for claim, vesting_amount: 100000.0000 ORE, vested_amount: 0.0000 ORE, vesting: 0.0000 ORE");

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*378-28+4060800));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(usertwo),
                     mvo()("vesting_id","1"));

        BOOST_CHECK_EQUAL(balance_get(chain, N(usertwo), ORE_SYMBOL), ORE(100'000));

        vest = print_cmkvesting(chain, 1);
        BOOST_CHECK_EQUAL(vest.vesting_id, 1);
        BOOST_CHECK_EQUAL(vest.account_id, N(usertwo));
        BOOST_CHECK_EQUAL(vest.vesting_period, 5);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(100'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2021-03-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 0);
        BOOST_CHECK_EQUAL(vest.status, 3);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(100'000));

        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(28'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(0));

        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*1247-29));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(userone),
                     mvo()("vesting_id","0"));

        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(0));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(0));
        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*376-34));
        //STEP:6
        //check pause
        //326
        token_issue(chain, N(manager), ORE(50'000));
        PUSH_CONSOLE(chain, EOSCTRACT, N(transfer), auths_manager,mvo()("from", "manager")
                ("to", CMKRYVESTING_STR)("quantity", ORE(50'000))("memo", "deposit"));
        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(50'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(50'000));
        //335
        PUSH_CONSOLE(chain, CMKRYVESTING, N(addvesting), auths_manager,
                     mvo()("account_id","userthree")("vesting_period",1)("vesting_amount",ORE(24'000))
                             ("cliff_date","1970-01-01T00:00:00")("cliff_weight",0)
                             ("start_date","2020-01-01T00:00:00")("end_date","2022-01-01T00:00:00"));

        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(50'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(26'000));

        vest = print_cmkvesting(chain, 2);
        BOOST_CHECK_EQUAL(vest.vesting_id, 2);
        BOOST_CHECK_EQUAL(vest.account_id, N(userthree));
        BOOST_CHECK_EQUAL(vest.vesting_period, 1);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(24'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2022-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "1970-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 0);
        BOOST_CHECK_EQUAL(vest.status, 1);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(0));
        //361
        BOOST_CHECK_THROW(chain.push_action(CMKRYVESTING, N(pausevesting), N(usertwo),
                                            mvo()("vesting_id",2)("pause_start","2021-01-20T00:00:00")), missing_auth_exception);
        //364
        PUSH_CONSOLE(chain, CMKRYVESTING, N(pausevesting), auths_manager,
                     mvo()("vesting_id",2)("pause_start","2021-01-11T00:00:00"));

        vest = print_cmkvesting(chain, 2);
        BOOST_CHECK_EQUAL(vest.vesting_id, 2);
        BOOST_CHECK_EQUAL(vest.account_id, N(userthree));
        BOOST_CHECK_EQUAL(vest.vesting_period, 1);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(24'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2022-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "2021-01-11T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 0);
        BOOST_CHECK_EQUAL(vest.status, 2);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(0));
        //379
        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*376-35+31536000));

        //381
        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(userthree), mvo()("vesting_id","2"));

        vest = print_cmkvesting(chain, 2);
        BOOST_CHECK_EQUAL(vest.vesting_id, 2);
        BOOST_CHECK_EQUAL(vest.account_id, N(userthree));
        BOOST_CHECK_EQUAL(vest.vesting_period, 1);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(24'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2022-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "2021-01-11T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 0);
        BOOST_CHECK_EQUAL(vest.status, 2);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(12'000));

        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(38'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(26'000));

        BOOST_CHECK_THROW(chain.push_action(CMKRYVESTING, N(unpausevestng), N(usertwo),
                                            mvo()("vesting_id",2)("pause_end","2021-01-20T00:00:00")), missing_auth_exception);
        //400
        PUSH_CONSOLE(chain, CMKRYVESTING, N(unpausevestng), auths_manager,
                     mvo()("vesting_id",2)("pause_end","2022-01-11T00:00:00"));

        vest = print_cmkvesting(chain, 2);
        BOOST_CHECK_EQUAL(vest.vesting_id, 2);
        BOOST_CHECK_EQUAL(vest.account_id, N(userthree));
        BOOST_CHECK_EQUAL(vest.vesting_period, 1);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(24'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2022-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "2021-01-11T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 31536000);
        BOOST_CHECK_EQUAL(vest.status, 1);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(12'000));

        //414
        PUSH_CONSOLE(chain, EOSDTCNTRACT, N(timemove), EOSDTCNTRACT, mvo()("seconds", 86400*376-21+31536000+2592000));

        PUSH_CONSOLE(chain, CMKRYVESTING, N(claim), N(userthree), mvo()("vesting_id","2"));

        vest = print_cmkvesting(chain, 2);
        BOOST_CHECK_EQUAL(vest.vesting_id, 2);
        BOOST_CHECK_EQUAL(vest.account_id, N(userthree));
        BOOST_CHECK_EQUAL(vest.vesting_period, 1);
        BOOST_CHECK_EQUAL(vest.vesting_amount, ORE(24'000));
        BOOST_CHECK_EQUAL(vest.cliff_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.cliff_weight, 0);
        BOOST_CHECK_EQUAL(vest.start_date.to_iso_string(), "2020-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.end_date.to_iso_string(), "2022-01-01T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_start.to_iso_string(), "2021-01-11T00:00:00");
        BOOST_CHECK_EQUAL(vest.pause_time, 31536000);
        BOOST_CHECK_EQUAL(vest.status, 1);
        BOOST_CHECK_EQUAL(vest.vested_amount, ORE(13'000));

        asset = print_cmkasset(chain, ORE_SYMBOL);
        BOOST_CHECK_EQUAL(asset.asset_balance, ORE(37'000));
        BOOST_CHECK_EQUAL(asset.unallocated_balance, ORE(26'000));



        PRINT_LABEL(chain, "stopped");
    } FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()