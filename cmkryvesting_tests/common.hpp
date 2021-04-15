#ifndef COMMON
#define COMMON

//#include <eosio.system/eosio.system.wast.hpp>
//#include <eosio.system/eosio.system.abi.hpp>
//#include <eosio.token/eosio.token.wast.hpp>
//#include <eosio.token/eosio.token.abi.hpp>
//#include <nitrogen/oraclize/oraclizeconn.wast.h>
//#include <nitrogen/oraclize/oraclizeconn.abi.h>


#include <../eosdt_lib_tests/tests_prints.hpp>
#include <eosio/chain/wast_to_wasm.hpp>

#ifndef TESTER
#define TESTER validating_tester
#endif

#define mvo mutable_variant_object
using namespace eosio::testing;

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define PRINT_LABEL(CHAIN, COMMENT) print_label(CHAIN,COMMENT, __FILENAME__, __LINE__)


static bool test_necessary(TESTER &chain, const ds_string &test_name) {
    return true;
    if (test_name == "buy_test") {
        return false;
    }


    PRINT_LABEL(chain, test_name.c_str());
    return false;
}


#define PUSH_EXCEPTION(CHAIN, CODE, ACTTYPE, ACTOR, DATA, MSG) push_exception(CHAIN, CODE, ACTTYPE, ACTOR, DATA, MSG, __FILENAME__, __LINE__)

static void
push_exception(TESTER &chain, const account_name &code, const action_name &acttype, const account_name &actor,
               const variant_object &data, const char *msg, const char *filename, int line) {
    try {
        BOOST_REQUIRE_EXCEPTION(chain.push_action(code, acttype, actor, data), eosio_assert_message_exception,
                                eosio_assert_message_is(msg));
        chain.produce_blocks();
    }
    catch (...) {
        chain.produce_blocks();
        print_label(chain, msg, filename, line);
        chain.push_action(code, acttype, actor, data);
    }
}

static void
push_exception(TESTER &chain, const account_name &code, const action_name &acttype,
               const vector<permission_level> &actor,
               const variant_object &data, const char *msg, const char *filename, int line) {
    try {
        BOOST_REQUIRE_EXCEPTION(chain.push_action(code, acttype, actor, data), eosio_assert_message_exception,
                                eosio_assert_message_is(msg));
        chain.produce_blocks();
    }
    catch (...) {
        chain.produce_blocks();
        print_label(chain, msg, filename, line);
        chain.push_action(code, acttype, actor, data);
    }
}

#define PUSH_THROW(CHAIN, CODE, ACTTYPE, ACTOR, DATA, EXTYPE) BOOST_REQUIRE_THROW(CHAIN.push_action(CODE, ACTTYPE, ACTOR, DATA), EXTYPE)

static auto full_console(const transaction_trace_ptr &transaction_trace) {
    for (auto action_trace:transaction_trace->action_traces) {
        if (!action_trace.console.empty()) {
            return true;
        }
        /*
        for (auto inline_trace:action_trace.inline_traces) {
            if (!inline_trace.console.empty()) {
                return true;
            }
        }*/
    }
    return false;
}


#define PUSH_CONSOLE(CHAIN, CODE, ACTTYPE, ACTOR, DATA) push_console(CHAIN, CODE, ACTTYPE, ACTOR, DATA, __FILENAME__, __LINE__)

static auto
push_console(TESTER &chain, const account_name &code, const action_name &acttype, const account_name &actor,
             const variant_object &data, const char *filename, int line) {
    try {
        auto tran = chain.push_action(code, acttype, actor, data);
        if (full_console(tran)) {
            print_label(chain, "", filename, line);
            print_console(tran);
        }
        chain.produce_blocks();
        return tran;
    }
    catch (...) {
        chain.produce_blocks();
        return chain.push_action(code, acttype, actor, data);
    }
}

static auto
push_console(TESTER &chain, const account_name &code, const action_name &acttype, const vector<permission_level> &actor,
             const variant_object &data, const char *filename, int line) {
    try {
        auto tran = chain.push_action(code, acttype, actor, data);
        if (full_console(tran)) {
            print_label(chain, "", filename, line);
            print_console(tran);
        }
        chain.produce_blocks();
        return tran;
    }
    catch (...) {
        chain.produce_blocks();
        return chain.push_action(code, acttype, actor, data);
    }
}

static void wasmabi_set(TESTER &chain, const ds_account &ctract, const ds_string wasm_path, const ds_string abi_path) {
    std::ifstream wasm(wasm_path);
    BOOST_TEST_INFO(wasm_path.c_str());
    BOOST_CHECK_EQUAL(wasm.good(), true);

    if (wasm_path.find("wast") != std::string::npos) {
        auto wast = std::string(std::istreambuf_iterator<char>(wasm), std::istreambuf_iterator<char>());
        auto w = eosio::chain::wast_to_wasm(wast);
        chain.set_code(ctract, w);
    } else {
        chain.set_code(ctract,
                       std::vector<uint8_t>(std::istreambuf_iterator<char>(wasm), std::istreambuf_iterator<char>()));
    }
    std::ifstream abi(abi_path);
    BOOST_TEST_INFO(abi_path.c_str());
    BOOST_CHECK_EQUAL(abi.good(), true);
    chain.set_abi(ctract, std::string(std::istreambuf_iterator<char>(abi), std::istreambuf_iterator<char>()).c_str());

}

static void wasmabi_set(TESTER &chain, const ds_account &ctract, ds_string name = "") {
    if (name == "") {
        name = ctract.to_string();
    }
    auto wasm_path = "../../../plt-eoscdt-contract/cmake-build-debug/contracts/testing/" + name + "/" + name + ".wasm";
    auto abi_path = "../../../plt-eoscdt-contract/contracts/testing/" + name + "/" + name + ".test.abi";

    wasmabi_set(chain, ctract, wasm_path, abi_path);
}

static auto set_orasettings(TESTER &chain) {
    auto data = mvo()("rate_timeout", 300)("query_timeout", 60)("provablecb1a_interval", 60)
            ("delphioracle_interval", 60)("equilibriumdsp_interval", 15)
            ("validity_timeout", 120);
    PUSH_CONSOLE(chain, EOSDTORCLIZE, N(settingset), EOSDTORCLIZE, data);
    chain.produce_blocks();
    auto orasettings = orasettings_print(chain);

    BOOST_CHECK_EQUAL(orasettings.rate_timeout, 300);
    BOOST_CHECK_EQUAL(orasettings.query_timeout, 60);
    BOOST_CHECK_EQUAL(orasettings.provablecb1a_interval, 60);
    BOOST_CHECK_EQUAL(orasettings.delphioracle_interval, 60);
    BOOST_CHECK_EQUAL(orasettings.equilibriumdsp_interval, 15);
    return data;
}

static auto USD(long double v) {
    return asset((int64_t) (v * 10000.0L), USD_SYMBOL);
}

static auto EOS(long double v) {
    return asset((int64_t) (v * 10000.0L), EOS_SYMBOL);
}

static auto REX(long double v) {
    return asset((int64_t) (v * 10000.0L), REX_SYMBOL);
}


static auto STABLE(long double v) {
    return asset((int64_t) (v * 1000000000.0L), STABLE_SYMBOL);
}

static auto EOSDT(long double v) {
    return STABLE(v);
}

//static auto STABLE(ds_string amount) {
//    return asset::from_string(amount + " " + STABLE_SYMBOL_STR);
//}

static auto UTILITY(long double v) {
    return asset((int64_t) (v * 1000000000.0L), UTILITY_SYMBOL);
}

static auto NUT(long double v) {
    return UTILITY(v);
}

static auto USDT(long double v) {
    return asset((int64_t) (v * 10000.0L), USDT_SYMBOL);
}

static auto DAPP(long double v) {
    return asset((int64_t) (v * 10000.0L), DAPP_SYMBOL);
}

static auto KG(long double v) {
    return asset((int64_t) (v * 100000000L), KG_SYMBOL);
}

static auto KGRAM(long double v) {
    return KG(v);
}

static auto KGRAM(int64_t nom, int64_t denom) {
    return asset(nom * 100000000L / denom, KG_SYMBOL);
}

static auto ORE(long double v) {
    return asset((int64_t) (v * 10000.0L), ORE_SYMBOL);
}

static auto set_govsettins(TESTER &chain) {
    auto data = mvo()
            ("position_account", ds_account(EOSDTCNTRACT).to_string())
            ("min_proposal_weight", USD(5000))
            ("freeze_period", 259200)
            ("min_participation", 0.01)
            ("success_margin", 0.55)
            ("top_holders_amount", 10)
            ("min_threshold", 0.05)
            ("max_bp_count", 2U)
            ("max_bp_votes", 2U)
            ("min_vote_stake", NUT(2))
            ("unstake_period", 864000)
            ("reward_weight", 0.7)
            ("min_reward", EOS(0))
            ("stake_reward", 0.9);
    PUSH_CONSOLE(chain, EOSDTGOVERNC, N(settingset), EOSDTGOVERNC, data);
    return data;
}

static auto token_create(TESTER &chain, const ds_asset &quantity) {
    print_time(chain);
    auto ctract = get_contract_by_token(quantity.get_symbol());
    auto issuer = name{ctract == EOSCTRACT ? N(eosio) : EOSDTCNTRACT};
    std::cout << "{ create:" << quantity << ", on: " << ctract << ", issuer: " << issuer << "}";
    auto result = PUSH_CONSOLE(chain, ctract, N(create), ctract, mvo()
            ("issuer", issuer.to_string())("maximum_supply", quantity));
    systoken_print(chain, quantity.get_symbol());
    return result;
}

static auto token_issueradd(TESTER &chain, const ds_account &issuer, const ds_asset &maximum_supply) {
    print_time(chain);
    auto ctract = get_contract_by_token(maximum_supply.get_symbol());
#ifdef PRINT
    std::cout << std::endl << "{ issueradd:" << issuer << ", on: " << ctract << ", maximum_supply: " << maximum_supply
              << "}";
#endif
    return PUSH_CONSOLE(chain, ctract, N(issueradd), ctract, mvo()
            ("issuer", issuer.to_string())("maximum_supply", maximum_supply));
}

static auto tokens_create(TESTER &chain) {
    token_create(chain, USD(461168601842738));
    token_create(chain, EOS(461168601842738));
    token_create(chain, NUT(4611686018.4273));//4611686018427387903
    token_create(chain, EOSDT(4611686018.4273));
    token_create(chain, USDT(4611686018.4273));
    token_issueradd(chain, EOSDTCNTRACT, EOSDT(4611686018.4273 / 2));
    //token_issueradd(chain, EOSDTVSKGRAM, EOSDT(4611686018.4273/2));
    token_create(chain, KG(4611686018.4273));
    PUSH_CONSOLE(chain, N(dappservices), N(create), N(dappservices), mvo()("maximum_supply_amount", 1'000'000)
            ("inflation_per_block", 0.000'000'000'42394888)("inflation_starts_at", "1562914813500"));
    token_create(chain, ORE(4611686018.4273));
}

static auto token_trans(TESTER &chain, const ds_account &from, const ds_account &to, const ds_asset &quantity,
                        const variant &memo) {
    print_time(chain);
    auto ctract = get_contract_by_token(quantity.get_symbol());
    std::cout << "trans:" << quantity << " by " << ctract << " from: " << from << " to: " << to;
    return PUSH_CONSOLE(chain, ctract, N(transfer), from, mvo()
            ("from", from.to_string())("to", to.to_string())("quantity", quantity)("memo", memo));
}

static auto token_trans(TESTER &chain, const ds_account &from, const ds_account &to, const ds_asset &quantity,
                        const ds_asset &memo) {
    return token_trans(chain, from, to, quantity, memo.to_string());
}

static auto token_issue(TESTER &chain, const ds_account &to, const ds_asset &quantity, const ds_string memo = "") {
    print_time(chain);
    auto ctract = get_contract_by_token(quantity.get_symbol());
    auto issuer = ds_account(ctract == EOSCTRACT ? N(eosio) : EOSDTCNTRACT);
    std::cout << "issue:" << quantity << " by: " << issuer << " to: " << to;
    auto result = PUSH_CONSOLE(chain, ctract, N(issue), issuer, mvo()
            ("to", issuer.to_string())("quantity", quantity)("memo", memo == "" ? "issue" : memo));
    if (to == issuer) {
        return result;
    }
    return token_trans(chain, issuer, to, quantity, memo);
}


static void contract_set(TESTER &chain) {
    print_time(chain);

    chain.create_accounts({N(eosio.bpay), N(eosio.msig), N(eosio.names), N(eosio.ram), N(eosio.ramfee), N(eosio.saving),
                           N(eosio.stake), N(eosio.token), N(eosio.vpay), N(eosio.rex)});
    chain.produce_blocks();

    chain.create_accounts(
            {N(manager), EOSDTORCLIZE, EOSDTCNTRACT, ORACLERATES, N(oraclizeconn), N(provableconn), EOSDTLIQDATR,
             EOSDTNUTOKEN, EOSDTSTTOKEN, EOSDTGOVERNC, EOSDTBPPROXY, EOSDTEXCHANG, TOKENSWAP_EQ, EOSIO_EVM,
             EOSDTSAVINGS,
             TETHERTETHER, EOS2DTDOTCOM, EOSLOTTERYEQ, CMKRYVESTING, EOSDTKGTOKEN, DAPPSERVICES});


    wasmabi_set(chain, EOSCTRACT,
                "../../../plt-eoscdt-contract/cmake-build-debug/basic_contracts/testing/eosio.token/eosio.token.wasm",
                "../../../plt-eoscdt-contract/basic_contracts/testing/eosio.token/eosio.token.abi"
    );
    /*wasmabi_set(chain, DAPPSERVICES,
                "../../../plt-eoscdt-contract/cmake-build-debug/basic_contracts/testing/dappservices/dappservices.wasm",
                "../../../plt-eoscdt-contract/basic_contracts/testing/dappservices/dappservices.abi"
    );
    wasmabi_set(chain, EOSDTNUTOKEN);
    wasmabi_set(chain, EOSDTSTTOKEN);
    wasmabi_set(chain, EOSDTKGTOKEN);
    wasmabi_set(chain, TETHERTETHER,
                "../../../plt-eoscdt-contract/cmake-build-debug/basic_contracts/testing/eosio.token/eosio.token.wasm",
                "../../../plt-eoscdt-contract/basic_contracts/testing/eosio.token/eosio.token.abi"
    );*/
    tokens_create(chain);
    token_issue(chain, N(eosio), EOS(800000000));

    wasmabi_set(chain, N(eosio),
                "../../../plt-eoscdt-contract/cmake-build-debug/basic_contracts/testing/eosio.system/eosio.system.wasm",
                "../../../plt-eoscdt-contract/basic_contracts/testing/eosio.system/eosio.system.abi"
    );
    PUSH_CONSOLE(chain, N(eosio), N(init), N(eosio), mvo()("version", 0)("core", EOS_SYMBOL));

    /*wasmabi_set(chain, EOSDTORCLIZE);
    wasmabi_set(chain, EOSDTCNTRACT);

    wasmabi_set(chain, N(provableconn),
                "../../../plt-eoscdt-contract/contracts/testing/provableconn/provableconn.wasm",
                "../../../plt-eoscdt-contract/contracts/testing/provableconn/provableconn.abi"
    );*/

    //set_orasettings(chain);

    /*wasmabi_set(chain, EOSDTLIQDATR);
    wasmabi_set(chain, EOSDTGOVERNC);
    wasmabi_set(chain, EOSDTEXCHANG);
    wasmabi_set(chain, TOKENSWAP_EQ);
    wasmabi_set(chain, EOSLOTTERYEQ);*/
    wasmabi_set(chain, CMKRYVESTING);
    //wasmabi_set(chain,EOSIO_EVM,
    //            "../../../plt-eoscdt-contract/cmake-build-debug/contracts/testing/eosio.evm/eosio.evm/eosio.evm.wasm",
    //            "../../../plt-eoscdt-contract/contracts/testing/eosio.evm/eosio.evm/eosio.evm.abi"
    //);
    wasmabi_set(chain, EOS2DTDOTCOM);


    chain.produce_blocks();
}

static void contract_set_eosio_system(TESTER &chain) {
    std::ifstream wasm("../../../plt-eos-contract/cmake-build-debug/contracts/eosio.system/eosio.system.wasm");
    chain.set_code(N(eosio),
                   std::vector<uint8_t>(std::istreambuf_iterator<char>(wasm), std::istreambuf_iterator<char>()));

    //std::ifstream abi("../../../plt-eoscdt-contract/contracts/" + name + "/" + name + ".test.abi");
    //chain.set_abi(ctract, std::string(std::istreambuf_iterator<char>(abi), std::istreambuf_iterator<char>()).c_str());
}


//static void require_table_empty(TESTER &chain, account_name table) {
//    auto t = chain.find_table(N(eosdt.ctract), N(eosdt.ctract), table);
//    BOOST_REQUIRE_EQUAL(nullptr, t);
//}
//
//static void require_table_exists(TESTER &chain, account_name managers) {
//    auto t = chain.find_table(N(eosdt.ctract), N(eosdt.ctract), managers);
//    BOOST_REQUIRE_NE(nullptr, t);
//}


static auto balance_get(TESTER &chain, const ds_account &account, const ds_symbol &currency) {

    return chain.get_currency_balance(get_contract_by_token(currency), currency, account);
}

enum class price_type : ds_int {
    SYMBOL_TO_EOS,
    EOS_TO_SYMBOL,
    SYMBOL_TO_USD
};

static ds_int price_type_to_i(const price_type price_type) {
    return static_cast<ds_int>(price_type);
}

static price_type i_to_price_type(const ds_int num) {
    BOOST_CHECK(num >= static_cast<ds_int>(
            price_type::SYMBOL_TO_EOS) && num <= static_cast<ds_int>(price_type::SYMBOL_TO_USD));

    return static_cast<price_type>(num);
}

static void
send_callback(TESTER &chain, const ds_string &queryId, const ds_asset &rate, const ds_asset &expected_rate,
              const ds_symbol &base = EOS_SYMBOL) {
    auto price = rate.to_string();
    std::vector<unsigned char> result(price.begin(), price.end());
    std::vector<unsigned char> proof;
    PUSH_CONSOLE(chain, EOSDTORCLIZE, N(callback), EOSDTORCLIZE, mvo()
            ("queryId", queryId)
            ("result", result)
            ("proof", proof)
    );
    auto oracle_rate = orarate_print(chain, rate.get_symbol(), base);
    BOOST_CHECK_EQUAL(oracle_rate.rate, expected_rate);
}

static void
set_rate(TESTER &chain, const ds_asset &rate, const ds_asset &expected_rate, const ds_symbol &base = EOS_SYMBOL) {
    PUSH_CONSOLE(chain, EOSDTORCLIZE, N(refreshutil), EOSDTORCLIZE, mvo()("symbol", rate.get_symbol())("base", base));
    ds_checksum empty;
    oraqueries oraqueries;
    for (auto i = 0; i < 10; i++) {
        if (chain.get_table_entry(oraqueries, EOSDTORCLIZE, EOSDTORCLIZE, N(oraclqueries), i, false)) {
            std::cout << std::endl << oraqueries.asset_symbol << " / " << oraqueries.base << " "
                      << hex_str(oraqueries.checksumm);
            if (oraqueries.source_contract == N(provablecb1a) && oraqueries.checksumm != empty
                && oraqueries.asset_symbol == rate.get_symbol() && oraqueries.base == base) {
                break;
            }
        }
    }
    auto queryId = hex_str(oraqueries.checksumm);
    send_callback(chain, queryId, rate, expected_rate, base);
}

static void set_rate(TESTER &chain, const ds_asset &rate, const ds_symbol &base = EOS_SYMBOL) {
    set_rate(chain, rate, rate, base);
}

static void set_query(TESTER &chain, const ds_symbol &symbol, const ds_string &query,
                      const price_type price_type, const ds_symbol &base = EOS_SYMBOL) {
    PUSH_CONSOLE(chain, EOSDTORCLIZE, N(queryadd), EOSDTORCLIZE,
                 mvo()("symbol", symbol)("query", query)("source_contract", "provablecb1a")
                         ("price_type", price_type_to_i(price_type))("base", base)
                         ("filter", 0.008)
    );
}

static auto queries_create(TESTER &chain) {
    set_query(chain, USD_SYMBOL, "json(https://min-api.cryptocompare.com/data/price?fsym=EOS&tsyms=USD).USD",
              price_type::EOS_TO_SYMBOL, EOS_SYMBOL);
    set_query(chain, NUT_SYMBOL, "json(https://api.hitbtc.com/api/2/public/ticker/NUTEOS).last",
              price_type::EOS_TO_SYMBOL, EOS_SYMBOL);
    set_query(chain, USD_SYMBOL, "json(https://api.hitbtc.com/api/2/public/ticker/NUTUSD).last",
              price_type::EOS_TO_SYMBOL, NUT_SYMBOL);
    set_query(chain, USD_SYMBOL,
              "json(https://api.bitforex.com/api/v1/market/trades?symbol=coin-usdt-gram).data.price",
              price_type::EOS_TO_SYMBOL, KG_SYMBOL);
}


static auto print_proposal(TESTER &chain, const ds_account &name) {
    govproposal proposal;
    chain.get_table_entry(proposal, EOSDTGOVERNC, EOSDTGOVERNC, N(proposals), name.to_uint64_t());
    print_time(chain);
    std::cout << "\tproposal_name: " << proposal.proposal_name;
    std::cout << ", proposer: " << proposal.proposer;
    std::cout << ", title: " << proposal.title;
    std::cout << ", proposal_json: " << proposal.proposal_json;
    std::cout << ", created_at: ";
    print_time(proposal.created_at);
    std::cout << ", expires_at: ";
    print_time(proposal.expires_at);
    return proposal;
}


static auto print_ctr_settings(TESTER &chain) {
    ctrsetting settings;
    chain.get_table_entry(settings, EOSDTCNTRACT, EOSDTCNTRACT, N(ctrsettings), 0);
    print_time(chain);
    std::cout << "\tctract_settings: { id: " << settings.setting_id;
    std::cout << ", governance_fee: " << settings.governance_fee;
    std::cout << ", stability_fee: " << settings.stability_fee;
    std::cout << ", critical_ltv: " << settings.critical_ltv;
    std::cout << ", liquidation_penalty: " << settings.liquidation_penalty;
    std::cout << ", liquidator_discount: " << settings.liquidator_discount;
    std::cout << " }";
    return settings;
}

static auto set_ctrsetting(TESTER &chain, double stability_fee = 0.0, double liquidation_penalty = 0.13,
                           double liquidator_discount = 0.03, double nut_auct_ratio = 0.3, double nut_discount = 0.03,
                           double profit_factor = 0.5, uint32_t vote_period = 864000, uint32_t stake_period = 86400,
                           double staking_weight = 0.5) {
    auto data = mvo()
            ("liquidator_account", EOSDTLIQDATR_STR)("oraclize_account", EOSDTORCLIZE_STR)("sttoken_account",
                                                                                           EOSDTSTTOKEN_STR)
            ("nutoken_account", EOSDTNUTOKEN_STR)
            ("governance_fee", 0.005)("stability_fee", stability_fee)("critical_ltv", 1.6)
            ("liquidation_penalty", liquidation_penalty)("liquidator_discount", liquidator_discount)
            ("nut_auct_ratio", nut_auct_ratio)("nut_discount", nut_discount)("profit_factor", profit_factor)
            ("vote_period", vote_period)("stake_period", stake_period)("reserve_ratio", 1.0)
            ("staking_weight", staking_weight)("bpproxy_account", EOSDTBPPROXY_STR)("governc_account", EOSDTGOVERNC_STR)
            ("referral_min_stake", NUT(2))
            ("referral_ratio", 1.0)
            ("collateral_account", EOSCTRACT_STR)
            ("collateral_token", EOS_SYMBOL)
            ("savings_account", ds_account(EOSDTSAVINGS).to_string())
            ("min_pos", EOS(0.0005))
            ("tokenswap_account", TOKENSWAP_EQ);
    PUSH_CONSOLE(chain, EOSDTCNTRACT, N(settingset), EOSDTCNTRACT, data);

    ctrsetting setting;
    chain.get_table_entry(setting, EOSDTCNTRACT, EOSDTCNTRACT, N(ctrsettings), 0);


    BOOST_CHECK_EQUAL(setting.liquidator_account, EOSDTLIQDATR);
    BOOST_CHECK_EQUAL(setting.oraclize_account, EOSDTORCLIZE);
    BOOST_CHECK_EQUAL(setting.sttoken_account, EOSDTSTTOKEN);
    BOOST_CHECK_EQUAL(setting.nutoken_account, EOSDTNUTOKEN);
    BOOST_CHECK_EQUAL(setting.global_lock, false);
    BOOST_CHECK_EQUAL(setting.time_shift, 0);
    BOOST_CHECK_EQUAL(setting.governance_fee, 0.005);
    BOOST_CHECK_EQUAL(setting.stability_fee, stability_fee);
    BOOST_CHECK_EQUAL(setting.critical_ltv, 1.6000000000000001);
    BOOST_CHECK_EQUAL(setting.liquidation_penalty, liquidation_penalty);
    BOOST_CHECK_EQUAL(setting.liquidator_discount, liquidator_discount);
    BOOST_CHECK_EQUAL(setting.liquidation_price, USD(0));
    BOOST_CHECK_EQUAL(setting.nut_auct_ratio, 0.3);
    BOOST_CHECK_EQUAL(setting.nut_discount, 0.03);
    BOOST_CHECK_EQUAL(setting.profit_factor, profit_factor);
    BOOST_CHECK_EQUAL(setting.vote_period, 864000U);
    BOOST_CHECK_EQUAL(setting.stake_period, 86400U);
    return data;
}

static auto set_liqsetting(TESTER &chain) {
    auto data = mvo()
            ("position_account", EOSDTCNTRACT_STR)
            ("burn_rate", 0.1)
            ("gov_return_rate", 0.45)
            ("tokenswap_rate", 0.45)
            ("savings_rate", 0);

    PUSH_CONSOLE(chain, EOSDTLIQDATR, N(settingset), EOSDTLIQDATR, data);
    return data;
}


static auto print_ctr_parameters(TESTER &chain, const string &comment = "") {
    print_time(chain);
    ctrparameter parameter;
    if (chain.get_table_entry(parameter, EOSDTCNTRACT, EOSDTCNTRACT, N(parameters), 0, false)) {
        std::cout << std::setprecision(std::numeric_limits<double>::digits10 + 1);
        std::cout << comment << "ctr_parameters: { total_collateral: " << parameter.total_collateral;
        std::cout << ", total_debt: " << parameter.total_debt;
        std::cout << ", stability_rate: " << parameter.stability_rate;
        std::cout << ", governance_rate: " << parameter.governance_rate;
        std::cout << ", prev_date: ";
        print_time(parameter.prev_date);
        std::cout << ", eos_staked: " << parameter.eos_staked;
        std::cout << " }";
    }
    return parameter;
}


static auto print_liq_settings(TESTER &chain) {
    liqsetting settings;
    chain.get_table_entry(settings, EOSDTLIQDATR, EOSDTLIQDATR, N(liqsettings), 0);
    print_time(chain);
    std::cout << "\tliqdatr_settings: { setting_id: " << settings.setting_id;
    std::cout << ", eosdtcntract_account: " << settings.eosdtcntract_account;
    std::cout << ", global_unlock: " << settings.global_unlock;
    std::cout << ", auction_price: " << settings.auction_price;
    std::cout << " }";
    return settings;
}


static auto print_ora_settings(TESTER &chain) {
    orasettings settings;
    chain.get_table_entry(settings, EOSDTORCLIZE, EOSDTORCLIZE, N(orasettings), 0);
    print_time(chain);
    std::cout << "\toracle_settings: { id: " << settings.id;
    std::cout << ", rate_timeout: " << settings.rate_timeout;
    std::cout << ", query_timeout: " << settings.query_timeout;
    std::cout << ", provablecb1a_interval: " << settings.provablecb1a_interval;
    std::cout << ", delphioracle_interval: " << settings.delphioracle_interval;
    std::cout << ", equilibriumdsp_interval: " << settings.equilibriumdsp_interval;

    std::cout << " }";
    return settings;
}

static auto print_gov_settings(TESTER &chain) {
    govsetting settings;
    chain.get_table_entry(settings, EOSDTGOVERNC, EOSDTGOVERNC, N(govsettings), 0);
    print_time(chain);
    std::cout << "\tforum_settings: { id: " << settings.setting_id;
    std::cout << ", eosdtcntract_account: " << settings.eosdtcntract_account;
    std::cout << ", min_proposal_weight: " << settings.min_proposal_weight;
    std::cout << ", freeze_period: " << settings.freeze_period;
    std::cout << ", min_participation: " << settings.min_participation;
    std::cout << ", success_margin: " << settings.success_margin;
    std::cout << ", top_holders_amount: " << settings.top_holders_amount;

    std::cout << " }";
    return settings;
}

static auto print_bpvote(TESTER &chain, ds_ulong id) {

    govbpvote bpvote;
    if (chain.get_table_entry(bpvote, EOSDTGOVERNC, EOSDTGOVERNC, N(bpvotes), id, false)) {
        print_time(chain);
        std::cout << "\tbp_vote: { producer: " << bpvote.producer;
        std::cout << ", votes: " << bpvote.votes;
        std::cout << " }";
    }
    return bpvote;
}


struct delegated_bandwidth {
    ds_account from;
    ds_account to;
    ds_asset net_weight;
    ds_asset cpu_weight;

    ds_ulong primary_key() const { return to.to_uint64_t(); }
};

FC_REFLECT(delegated_bandwidth, (from)(to)(net_weight)(cpu_weight))

static auto print_delband(TESTER &chain, account_name scope, ds_ulong id) {

    delegated_bandwidth delband;

    if (chain.get_table_entry(delband, N(eosio), scope, N(delband), id, false)) {
        print_time(chain);
        std::cout << "\tdelband(" << scope << "): ";
        std::cout << "{from: " << delband.from;
        std::cout << ", to: " << delband.to;
        std::cout << ", net_weight: " << delband.net_weight;
        std::cout << ", cpu_weight: " << delband.cpu_weight << " }";
    }

    return delband;
}

static void create_newaccount(TESTER &chain, const name &creator, const name &newaccount) {

    signed_transaction trx;

    auto owner_auth = eosio::chain::authority{1, {{chain.get_public_key(newaccount, "owner"), 1}}, {}};
    auto active_auth = eosio::chain::authority{1, {{chain.get_public_key(newaccount, "active"), 1}}, {}};
    trx.actions.emplace_back(vector<eosio::chain::permission_level>{{creator, N(active)}},
                             eosio::chain::newaccount{creator, newaccount, owner_auth, active_auth});

    trx.actions.emplace_back(chain.get_action(config::system_account_name, N(buyrambytes),
                                              vector<permission_level>{{creator, config::active_name}},
                                              mvo()
                                                      ("payer", creator)
                                                      ("receiver", newaccount)
                                                      ("bytes", 100000000))
    );
    trx.actions.emplace_back(chain.get_action(config::system_account_name, N(delegatebw),
                                              vector<permission_level>{{creator, config::active_name}},
                                              mvo()
                                                      ("from", creator)
                                                      ("receiver", newaccount)
                                                      ("stake_net_quantity", EOS(100))
                                                      ("stake_cpu_quantity", EOS(100))
                                                      ("transfer", 0)
                             )
    );

    chain.set_transaction_headers(trx);
    trx.sign(chain.get_private_key(creator, "active"), chain.control->get_chain_id());
    chain.push_transaction(trx);

}

static auto new_accounts(TESTER &chain, vector<account_name> names) {
    for (auto n : names) {
        create_newaccount(chain, config::system_account_name, n);
    }
    return names;
}

struct eosio_global_state {
    uint64_t max_block_net_usage;
    uint32_t target_block_net_usage_pct;
    uint32_t max_transaction_net_usage;
    uint32_t base_per_transaction_net_usage;
    uint32_t net_usage_leeway;
    uint32_t context_free_discount_net_usage_num;
    uint32_t context_free_discount_net_usage_den;
    uint32_t max_block_cpu_usage;
    uint32_t target_block_cpu_usage_pct;
    uint32_t max_transaction_cpu_usage;
    uint32_t min_transaction_cpu_usage;
    uint64_t context_free_discount_cpu_usage_num;
    uint64_t context_free_discount_cpu_usage_den;
    uint32_t max_transaction_lifetime;
    uint32_t deferred_trx_expiration_window;
    uint32_t max_transaction_delay;
    uint32_t max_inline_action_size;
    uint16_t max_inline_action_depth;
    uint16_t max_authority_depth;
    uint64_t max_ram_size;
    uint64_t total_ram_bytes_reserved;
    int64_t total_ram_stake;
    block_timestamp_type last_producer_schedule_update;
    uint64_t last_pervote_bucket_fill;
    int64_t pervote_bucket;
    int64_t perblock_bucket;
    uint32_t total_unpaid_blocks;
    int64_t total_activated_stake;
    uint64_t thresh_activated_stake_time;
    uint16_t last_producer_schedule_size;
    double total_producer_vote_weight;
    block_timestamp_type last_name_close;

};
FC_REFLECT(eosio_global_state,
           (max_block_net_usage)(target_block_net_usage_pct)
                   (max_transaction_net_usage)(base_per_transaction_net_usage)(net_usage_leeway)
                   (context_free_discount_net_usage_num)(context_free_discount_net_usage_den)
                   (max_block_cpu_usage)(target_block_cpu_usage_pct)
                   (max_transaction_cpu_usage)(min_transaction_cpu_usage)
                   (max_transaction_lifetime)(deferred_trx_expiration_window)(max_transaction_delay)
                   (max_inline_action_size)(max_inline_action_depth)(max_authority_depth)

                   (max_ram_size)(total_ram_bytes_reserved)(total_ram_stake)
                   (last_producer_schedule_update)(last_pervote_bucket_fill)
                   (pervote_bucket)(perblock_bucket)(total_unpaid_blocks)(total_activated_stake)(
                   thresh_activated_stake_time)
                   (last_producer_schedule_size)(total_producer_vote_weight)(last_name_close));

static auto print_global(TESTER &chain) {

    eosio_global_state global;
    if (chain.get_table_entry(global, N(eosio), N(eosio), N(global), N(global).to_uint64_t(), false)) {
        print_time(chain);
        std::cout << "\tglobal: { max_block_net_usage: " << global.max_block_net_usage;
        std::cout << ", target_block_net_usage_pct: " << global.target_block_net_usage_pct;
        std::cout << ",max_transaction_net_usage : " << global.max_transaction_net_usage;
        std::cout << ",base_per_transaction_net_usage : " << global.base_per_transaction_net_usage;
        std::cout << ",net_usage_leeway : " << global.net_usage_leeway;
        std::cout << ",context_free_discount_net_usage_num : " << global.context_free_discount_net_usage_num;
        std::cout << ",context_free_discount_net_usage_den : " << global.context_free_discount_net_usage_den;
        std::cout << ",max_block_cpu_usage : " << global.max_block_cpu_usage;
        std::cout << ",target_block_cpu_usage_pct : " << global.target_block_cpu_usage_pct;
        std::cout << ",max_transaction_cpu_usage : " << global.max_transaction_cpu_usage;
        std::cout << ",min_transaction_cpu_usage : " << global.min_transaction_cpu_usage;
        std::cout << ",max_transaction_lifetime : " << global.max_transaction_lifetime;
        std::cout << ",deferred_trx_expiration_window : " << global.deferred_trx_expiration_window;
        std::cout << ",max_transaction_delay : " << global.max_transaction_delay;
        std::cout << ",max_inline_action_size : " << global.max_inline_action_size;
        std::cout << ",max_inline_action_depth : " << global.max_inline_action_depth;
        std::cout << ",max_authority_depth : " << global.max_authority_depth;

        std::cout << ",max_ram_size : " << global.max_ram_size;
        std::cout << ",total_ram_bytes_reserved : " << global.total_ram_bytes_reserved;
        std::cout << ",total_ram_stake : " << global.total_ram_stake;


    }
}

struct refund_request {
    account_name owner;
    ds_time request_time;
    ds_asset net_amount;
    ds_asset cpu_amount;

    uint64_t primary_key() const { return owner.to_uint64_t(); }
};

FC_REFLECT(refund_request, (owner)(request_time)(net_amount)(cpu_amount))

static auto print_refund(TESTER &chain, account_name from) {

    refund_request refund;
    if (chain.get_table_entry(refund, N(eosio), from, N(refunds), from.to_uint64_t(), false)) {
        print_time(chain);
        std::cout << "\trefund: { owner: " << refund.owner;
        std::cout << ", request_time: ";
        print_time(refund.request_time);
        std::cout << ",net_amount : " << refund.net_amount;
        std::cout << ",cpu_amount : " << refund.cpu_amount;
        std::cout << "}";
    }
    return refund;
}

static auto print_eosio_producer(TESTER &chain, account_name id) {
    sysproducer producer;
    if (chain.get_table_entry(producer, N(eosio), N(eosio), N(producers), id.to_uint64_t(), false)) {
        print_time(chain);
        std::cout << "\tproducer: { owner: " << producer.owner;
        std::cout << ", total_votes: " << producer.total_votes;
        std::cout << ", producer_key: " << producer.producer_key;
        std::cout << ", is_active: " << producer.is_active;
        std::cout << ", url: " << producer.url;
        std::cout << ", unpaid_blocks: " << producer.unpaid_blocks;
        std::cout << ", last_claim_time: " << producer.last_claim_time;
        std::cout << ", location: " << producer.location;
        std::cout << "}";
    }
    return producer;
}


struct voter_info {
    account_name owner;
    account_name proxy;
    std::vector<account_name> producers;
    int64_t staked;
    double last_vote_weight;
    double proxied_vote_weight;
    bool is_proxy;
    uint32_t reserved1;
    ds_time reserved2;
    ds_asset reserved3;

};
FC_REFLECT(voter_info,
           (owner)(proxy)(producers)(staked)(last_vote_weight)(proxied_vote_weight)(is_proxy)(reserved1)(reserved2)(
                   reserved3))

static auto print_eosio_voter(TESTER &chain, account_name id) {
    voter_info voter;
    if (chain.get_table_entry(voter, N(eosio), N(eosio), N(voters), id.to_uint64_t(), false)) {
        print_time(chain);
        std::cout << "\tvoters: { owner: " << voter.owner;
        std::cout << ",proxy : " << voter.proxy;
        std::cout << ",producers : [";
        for (auto producer : voter.producers) {
            std::cout << producer << ",";
        }
        std::cout << "],staked : " << voter.staked;
        std::cout << ",last_vote_weight : " << voter.last_vote_weight;
        std::cout << ",proxied_vote_weight : " << voter.proxied_vote_weight;
        std::cout << ",is_proxy : " << voter.is_proxy;
        std::cout << ",reserved1 : " << voter.reserved1;
        std::cout << ",reserved2 : ";
        print_time(voter.reserved2);
        std::cout << ",reserved3 : " << voter.reserved3;
        std::cout << "}";
    }
    return voter;
}

struct rex_pool {
    uint8_t version = 0;
    ds_asset total_lent; /// total amount of CORE_SYMBOL in open rex_loans
    ds_asset total_unlent; /// total amount of CORE_SYMBOL available to be lent (connector)
    ds_asset total_rent; /// fees received in exchange for lent  (connector)
    ds_asset total_lendable; /// total amount of CORE_SYMBOL that have been lent (total_unlent + total_lent)
    ds_asset total_rex; /// total number of REX shares allocated to contributors to total_lendable
    ds_asset namebid_proceeds; /// the amount of CORE_SYMBOL to be transferred from namebids to REX pool
    uint64_t loan_num = 0; /// increments with each new loan

    uint64_t primary_key() const { return 0; }
};
FC_REFLECT(rex_pool,
           (version)(total_lent)(total_unlent)(total_rent)(total_lendable)(total_rex)(namebid_proceeds)(loan_num))

static auto print_rex_pool(TESTER &chain) {
    rex_pool rex_pool;
    if (chain.get_table_entry(rex_pool, N(eosio), N(eosio), N(rexpool), 0, false)) {
        print_time(chain);
        std::cout << "\trex_pool: { version: " << rex_pool.version;
        std::cout << ", total_lent: " << rex_pool.total_lent;
        std::cout << ", total_unlent: " << rex_pool.total_unlent;
        std::cout << ", total_rent: " << rex_pool.total_rent;
        std::cout << ", total_lendable: " << rex_pool.total_lendable;
        std::cout << ", total_rex: " << rex_pool.total_rex;
        std::cout << ", namebid_proceeds: " << rex_pool.namebid_proceeds;
        std::cout << ", loan_num: " << rex_pool.loan_num;
        std::cout << "}";

    }
    return rex_pool;
}

struct rex_fund {
    uint8_t version = 0;
    name owner;
    asset balance;

    uint64_t primary_key() const { return owner.to_uint64_t(); }
};

FC_REFLECT(rex_fund, (version)(owner)(balance))

static auto print_rex_fund(TESTER &chain, account_name id) {
    rex_fund rex_fund;
    if (chain.get_table_entry(rex_fund, N(eosio), N(eosio), N(rexfund), id.to_uint64_t(), false)) {
        print_time(chain);
        std::cout << "\trex_fund: { version: " << rex_fund.version;
        std::cout << ", owner: " << rex_fund.owner;
        std::cout << ", balance: " << rex_fund.balance;
        std::cout << "}";

    }
    return rex_fund;
}

struct rex_balance {
    uint8_t version = 0;
    name owner;
    asset vote_stake; /// the amount of CORE_SYMBOL currently included in owner's vote
    asset rex_balance; /// the amount of REX owned by owner
    int64_t matured_rex = 0; /// matured REX available for selling
    std::deque<std::pair<time_point_sec, int64_t>> rex_maturities; /// REX daily maturity buckets

    uint64_t primary_key() const { return owner.to_uint64_t(); }
};

FC_REFLECT(rex_balance, (version)(owner)(vote_stake)(rex_balance)(matured_rex)(rex_maturities))

static auto print_rex_balance(TESTER &chain, account_name id) {
    rex_balance rex_balance;
    if (chain.get_table_entry(rex_balance, N(eosio), N(eosio), N(rexbal), id.to_uint64_t(), false)) {
        print_time(chain);
        std::cout << "\trex_balance: { version: " << rex_balance.version;
        std::cout << ", owner: " << rex_balance.owner;
        std::cout << ", vote_stake: " << rex_balance.vote_stake;
        std::cout << ", rex_balance: " << rex_balance.rex_balance;
        std::cout << ", matured_rex: " << rex_balance.matured_rex;
        std::cout << ", rex_maturities: [";
        for (auto item:rex_balance.rex_maturities) {
            std::cout << "{";
            print_time(item.first);
            std::cout << "," << item.second << "},";
        }
        std::cout << "],}";

    }
    return rex_balance;
}

struct rex_loan {
    uint8_t version = 0;
    name from;
    name receiver;
    asset payment;
    asset balance;
    asset total_staked;
    uint64_t loan_num;
    ds_time expiration;

    uint64_t primary_key() const { return loan_num; }
};

FC_REFLECT(rex_loan, (version)(from)(receiver)(payment)(balance)(total_staked)(loan_num)(expiration))

static auto print_rex_loan_cpu(TESTER &chain, account_name id) {
    rex_loan rex_loan;
    for (auto i = 0; i < 100; i++) {
        if (chain.get_table_entry(rex_loan, N(eosio), N(eosio), N(cpuloan), i, false)) {
            if (rex_loan.from == id) {
                print_time(chain);
                std::cout << "\trex_loan_cpu: { version: " << rex_loan.version;
                std::cout << ", from: " << rex_loan.from;
                std::cout << ", receiver: " << rex_loan.receiver;
                std::cout << ", payment: " << rex_loan.payment;
                std::cout << ", balance: " << rex_loan.balance;
                std::cout << ", total_staked: " << rex_loan.total_staked;
                std::cout << ", loan_num: " << rex_loan.loan_num << ", ";
                print_time(rex_loan.expiration);
                std::cout << "}";
                //return rex_loan;
            }
        }
    }
    return rex_loan;
}


#endif

