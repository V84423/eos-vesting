#ifndef TESTS_PRINTS_HPP
#define TESTS_PRINTS_HPP

#include "tests_tables.hpp"

#define PRINT

static std::string hex_str(const ds_checksum &checksum) {
    auto data = reinterpret_cast<uint8_t *>(checksum.data());
    auto len = checksum.data_size();

    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}


static const ds_string read_console(const ds_tran_trace &tran_trace) {
    ds_string console;
    for (auto action_trace:tran_trace->action_traces) {
        if (!action_trace.console.empty()) {
            console += action_trace.console;
        }
    }
    return console;
}

static const auto print_console(const ds_tran_trace &tran_trace) {
#ifdef PRINT
    std::cout << read_console(tran_trace);
#endif
    return tran_trace;
}

static void print_time(ds_chain &chain) {
#ifdef PRINT
    time_t time = chain.control->head_block_time().sec_since_epoch();
    std::tm tm = *std::localtime(&time);
    std::cout << std::endl << std::put_time(&tm, "[%M:%S]\t");
#endif
}

static void print_time(ds_time &moment) {
#ifdef PRINT
    time_t time = moment.sec_since_epoch();
    std::tm tm = *std::localtime(&time);
    std::cout << std::endl << std::put_time(&tm, "[%M:%S]\t");
#endif
}

static void print_label(ds_chain &chain, const char *comment, const char *filename, int line) {
#ifdef PRINT
    print_time(chain);
    std::cout << filename << ':' << line << ' ' << comment;
#endif
}

static void print_date(ds_time &moment) {
#ifdef PRINT
    time_t time = moment.sec_since_epoch();
    std::tm tm = *std::localtime(&time);
    std::cout << "'" << std::put_time(&tm, "%Y%m%d %H:%M:%S") << "'";
#endif
}

static auto get_contract_by_token(const ds_symbol &currency) {
    auto ctract = EOSCTRACT;
    if (currency == STABLE_SYMBOL) {
        ctract = EOSDTSTTOKEN;
    } else if (currency == UTILITY_SYMBOL) {
        ctract = EOSDTNUTOKEN;
    } else if (currency == DAPP_SYMBOL) {
        ctract = N(dappservices);
    } else if (currency == KG_SYMBOL) {
        ctract = EOSDTKGTOKEN;
    } else if (currency == USDT_SYMBOL) {
        ctract = TETHERTETHER;
    }
    return ctract;
}

static auto systoken_print(ds_chain &chain, const ds_symbol &token, ds_string comment = "") {
    auto ctract = get_contract_by_token(token);
    sysstat stat;
    auto id = token.to_symbol_code().value;
    if(chain.get_table_entry(stat, ctract, ds_account(id), N(stat), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "token:{ ";
        std::cout << " contract: " << ctract;
        std::cout << " , issuer: " << stat.issuer;
        std::cout << " , max_supply: " << stat.max_supply;
        std::cout << " , supply: " << stat.supply << " }";
#endif
    }
    return stat;
}

static auto eosdt_issuer_print(ds_chain &chain, const ds_account &issuer, ds_string comment = "") {
    tokissuers tokissuer;
    if (chain.get_table_entry(tokissuer, EOSDTSTTOKEN, EOSDTSTTOKEN, N(tokissuers), issuer.to_uint64_t(), false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "tokissuer:{ issuer: " << tokissuer.issuer;
        std::cout << " , max_supply: " << tokissuer.max_supply;
        std::cout << " , supply: " << tokissuer.supply;
        std::cout << " , is_active: " << tokissuer.is_active << " }";
#endif
    }
    return tokissuer;
}


auto static orasettings_print(ds_chain &chain) {
    orasettings orasettings;
    chain.get_table_entry(orasettings, EOSDTORCLIZE, EOSDTORCLIZE, N(orasettings), 0);
    return orasettings;
}

static auto
orarate_print(ds_chain &chain, const ds_symbol &symbol, const ds_symbol &base, const ds_string &comment = "") {
    orarate oracle_rate;
    for (auto i = 0;
         i < 100 && chain.get_table_entry(oracle_rate, EOSDTORCLIZE, EOSDTORCLIZE, N(newrates), i, false); i++) {
        if (oracle_rate.rate.get_symbol() == symbol && oracle_rate.base == base) {
#ifdef PRINT
            std::cout << std::endl << comment;
            std::cout << "id: " << oracle_rate.id << " , ";
            std::cout << "rate:" << oracle_rate.rate << '/' << oracle_rate.base << ',' << '\t';
#endif
            return oracle_rate;
        }
    }
    return oracle_rate;
}

static void orarates_print(ds_chain &chain, const ds_string &comment = "") {
#ifdef PRINT
    orarate oracle_rate;
    std::cout << std::endl << comment;
    orarate_print(chain, USD_SYMBOL, EOS_SYMBOL);
    orarate_print(chain, NUT_SYMBOL, EOS_SYMBOL);
#endif
}

static auto orarate_print(ds_chain &chain, const ds_symbol &symbol, const ds_string &comment = "") {
    return orarate_print(chain, symbol, EOS_SYMBOL);
}


static auto oraquery_print(ds_chain &chain, const ds_ulong &id, const ds_string &comment = "") {
    oraqueries oraquery;
    if (chain.get_table_entry(oraquery, EOSDTORCLIZE, EOSDTORCLIZE, N(oraclqueries), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "oraquery: { ";
//        std::cout << " id: " << oraquery.id;
        std::cout << " ,asset_symbol : " << oraquery.asset_symbol;
//        std::cout << " ,source_contract : " << oraquery.source_contract;
        std::cout << " ,query : " << oraquery.query;
        std::cout << " ,price_type : " << oraquery.price_type;
        std::cout << " ,query_updated_at : ";
        print_date(oraquery.query_updated_at);
        std::cout << " ,query_executed_at : ";
        print_date(oraquery.query_executed_at);
        std::cout << " ,checksumm : " << hex_str(oraquery.checksumm);
        std::cout << "}";
#endif
    }
    return oraquery;
}

static void oraqueries_print(ds_chain &chain, const ds_string &comment = "") {
    for (auto i = 0; i < 10; i++) {
        oraquery_print(chain, i, comment);
    }
}

static auto print_subscriber(ds_chain &chain, const ds_ulong id) {
    orasubscribe subscribe;
    if (chain.get_table_entry(subscribe, EOSDTORCLIZE, EOSDTORCLIZE, N(subscribers), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\tsubscribe: { contract: " << subscribe.contract;
        std::cout << ", quantity: " << subscribe.quantity;
        std::cout << ", callcount: " << subscribe.callcount;
        std::cout << ", lastcall: ";
        print_time(subscribe.lastcall);
        std::cout << ", withdrawal_date: ";
        print_time(subscribe.withdrawal_date);
        std::cout << ", id: " << subscribe.id;
        std::cout << ", asset_symbol: " << subscribe.asset_symbol;
        std::cout << ", base: " << subscribe.base;
        std::cout << "}";
#endif
    }
    return subscribe;
}

static auto datapoints_print(ds_chain &chain, ds_account pair, ds_int id) {
    datapoint datapoint;
    if (chain.get_table_entry(datapoint, DELPHIORACLE, pair, N(datapoints), id, false)) {

#ifdef PRINT
        std::cout << std::endl;
        std::cout << "datapoint: {";
        std::cout << "id:" << datapoint.id << ",";
        std::cout << "owner:" << datapoint.owner << ",";
        std::cout << "value:" << datapoint.value << ",";
        std::cout << "median:" << datapoint.median << ",";
        std::cout << "timestamp:" << datapoint.timestamp << ",";
        std::cout << "}";
#endif
    }
    return datapoint;
}

static void datapoints_print(ds_chain &chain, ds_account pair) {
    for (auto i = 0; i < 100; i++) {
        datapoints_print(chain, pair, i);
    }

}

static auto print_ctrsetting(ds_chain &chain, const ds_string &comment = "") {

    ctrsetting setting;
    chain.get_table_entry(setting, EOSDTCNTRACT, EOSDTCNTRACT, N(ctrsettings), 0);
#ifdef PRINT
    print_time(chain);
    std::cout << comment << "ctrsetting: { ";
    std::cout << " global_lock: " << (setting.global_lock ? "true" : "false");
    std::cout << " , time_shift: " << setting.time_shift;

    std::cout << " ,liquidator_account : " << setting.liquidator_account;
    std::cout << " ,oraclize_account : " << setting.oraclize_account;
    std::cout << " ,sttoken_account : " << setting.sttoken_account;
    std::cout << " ,nutoken_account : " << setting.nutoken_account;

    std::cout << " , governance_fee: " << setting.governance_fee;
    std::cout << " , stability_fee: " << setting.stability_fee;
    std::cout << " , critical_ltv: " << setting.critical_ltv;
    std::cout << " , liquidation_penalty: " << setting.liquidation_penalty;
    std::cout << " , liquidator_discount: " << setting.liquidator_discount;
    std::cout << " , liquidation_price: " << setting.liquidation_price;
    std::cout << " , nut_auct_ratio: " << setting.nut_auct_ratio;
    std::cout << " , nut_discount: " << setting.nut_discount;
    std::cout << " , profit_factor: " << setting.profit_factor;
    std::cout << " , vote_period: " << setting.vote_period;
    std::cout << " , stake_period: " << setting.stake_period;
    std::cout << " , savings_account: " << setting.savings_account;
    std::cout << " }";
#endif
    return setting;
}


static auto print_parameters(ds_chain &chain, const ds_string &comment = "") {
    ctrparameter parameter;
    chain.get_table_entry(parameter, EOSDTCNTRACT, EOSDTCNTRACT, N(parameters), 0);
#ifdef PRINT
    print_time(chain);
    std::setprecision(20);
    std::cout << comment << "parameters: { total_collateral: " << parameter.total_collateral;
    std::cout << ", total_debt: " << parameter.total_debt;
    std::cout << ", stability_rate: " << parameter.stability_rate;
    std::cout << ", governance_rate: " << parameter.governance_rate;
    std::cout << ", prev_date: ";
    print_date(parameter.prev_date);
    std::cout << " }";
#endif
    return parameter;
}

static auto print_kg_parameters(ds_chain &chain, const ds_string &comment = "") {
    posparameter parameter;
    chain.get_table_entry(parameter, EOSDTVSKGRAM, EOSDTVSKGRAM, N(parameters), 0);
#ifdef PRINT
    print_time(chain);
    std::setprecision(20);
    std::cout << comment << "parameters: { total_collateral: " << parameter.total_collateral;
    std::cout << ", total_debt: " << parameter.total_debt;
    std::cout << ", stability_rate: " << parameter.stability_rate;
    std::cout << ", prev_date: ";
    print_date(parameter.prev_date);
    std::cout << " }";
#endif
    return parameter;
}

static auto print_position(ds_chain &chain, const ds_ulong &position_id, const ds_string &comment = "") {
    ctrposition position;
    chain.get_table_entry(position, EOSDTCNTRACT, EOSDTCNTRACT, N(positions), position_id);
#ifdef PRINT
    print_time(chain);
    std::cout << comment << "position: { " << " position_id: " << position.position_id;
    std::cout << ", maker: " << position.maker;
    std::cout << ", outstanding: " << position.outstanding;
    std::cout << ", governance: " << position.governance;
    std::cout << ", collateral: " << position.collateral << " }";
#endif
    return position;
}

static void print_positions(ds_chain &chain, const ds_string &comment = "") {
    ctrposition position;
    for (auto position_id = 0; position_id < 100; position_id++) {
        if (chain.get_table_entry(position, EOSDTCNTRACT, EOSDTCNTRACT, N(positions), position_id, false)) {
            print_position(chain, position_id);
        }
    }
}

static auto print_kg_position(ds_chain &chain, const ds_ulong &position_id, const ds_string &comment = "") {
    posposition position;
    chain.get_table_entry(position, EOSDTVSKGRAM, EOSDTVSKGRAM, N(positions), position_id);
#ifdef PRINT
    print_time(chain);
    std::cout << comment << "position: { " << " position_id: " << position.position_id;
    std::cout << ", maker: " << position.maker;
    std::cout << ", outstanding: " << position.outstanding;
    std::cout << ", collateral: " << position.collateral << " }";
#endif
    return position;
}

static void print_kg_positions(ds_chain &chain, const ds_string &comment = "") {
    posposition position;
    for (auto position_id = 0; position_id < 100; position_id++) {
        if (chain.get_table_entry(position, EOSDTVSKGRAM, EOSDTVSKGRAM, N(positions), position_id, false)) {
            print_position(chain, position_id);
        }
    }
}

static auto ltvratio_print(ds_chain &chain, const ds_ulong &position_id, const ds_string &comment = "") {
    ctrltvratio ltvratio;
    chain.get_table_entry(ltvratio, EOSDTCNTRACT, EOSDTCNTRACT, N(ctrltvratios), position_id);
#ifdef PRINT
    print_time(chain);
    std::cout << comment << "ctrltvratio: { " << " position_id: " << ltvratio.position_id;
    std::cout << ", ltv_ratio: " << ltvratio.ltv_ratio << " }";
#endif
    return ltvratio;
}

static auto ltvratio_kg_print(ds_chain &chain, const ds_ulong &position_id, const ds_string &comment = "") {
    ctrltvratio ltvratio;
    chain.get_table_entry(ltvratio, EOSDTVSKGRAM, EOSDTVSKGRAM, N(ctrltvratios), position_id);
#ifdef PRINT
    print_time(chain);
    std::cout << comment << "ctrltvratio: { " << " position_id: " << ltvratio.position_id;
    std::cout << ", ltv_ratio: " << ltvratio.ltv_ratio << " }";
#endif
    return ltvratio;
}


static auto ctrreferral_print(ds_chain &chain, const ds_ulong &referral_id, const ds_string &comment = "") {
    ctrreferral referral;
    chain.get_table_entry(referral, EOSDTCNTRACT, EOSDTCNTRACT, N(ctrreferrals), referral_id);
#ifdef PRINT
    print_time(chain);
    std::cout << comment << "referral: { " << " referral_id: " << referral.referral_id;
    std::cout << ", referral: " << referral.referral;
    std::cout << ", staked_amount: " << referral.staked_amount << " }";
#endif
    return referral;
}


static void ctrreferrals_print(ds_chain &chain, const ds_string &comment = "") {
    ctrreferral referral;
    for (auto referral_id = 0; referral_id < 100; referral_id++) {
        if (chain.get_table_entry(referral, EOSDTCNTRACT, EOSDTCNTRACT, N(ctrreferrals), referral_id, false)) {
            ctrreferral_print(chain, referral_id);
        }
    }
}

static auto ctrpositionref_print(ds_chain &chain, const ds_ulong &position_id, const ds_string &comment = "") {
    ctrpositionref positionref;
    if (chain.get_table_entry(positionref, EOSDTCNTRACT, EOSDTCNTRACT, N(positionrefs), position_id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "positionref: { " << " position_id: " << positionref.position_id;
        std::cout << ", referral_id: " << positionref.referral_id << " }";
#endif
    }
    return positionref;
}


static auto print_liquidator_param(ds_chain &chain, const ds_string &comment = "") {

    BOOST_REQUIRE_NE(chain.find_table(EOSDTLIQDATR, EOSDTLIQDATR, N(parameters)), nullptr);


    liqparameter parameter;
    chain.get_table_entry(parameter, EOSDTLIQDATR, EOSDTLIQDATR, N(parameters), 0);
#ifdef PRINT
    print_time(chain);
    std::cout << comment;
    std::cout << "liquid_parameters:{ surplus_debt: " << parameter.surplus_debt;
    std::cout << ", bad_debt: " << parameter.bad_debt;
    std::cout << ", eos_balance: " << parameter.eos_balance;
    std::cout << ", nut_collat_balance: " << parameter.nut_collat_balance << "}";
#endif

    return parameter;
}

static auto print_kg_liquidator_param(ds_chain &chain, const ds_string &comment = "") {

    BOOST_REQUIRE_NE(chain.find_table(KGRAMLIQDATR, KGRAMLIQDATR, N(parameters)), nullptr);


    liqparameter parameter;
    chain.get_table_entry(parameter, KGRAMLIQDATR, KGRAMLIQDATR, N(parameters), 0);
#ifdef PRINT
    print_time(chain);
    std::cout << comment;
    std::cout << "collat_liquid_parameters:{ surplus_debt: " << parameter.surplus_debt;
    std::cout << ", bad_debt: " << parameter.bad_debt;
    std::cout << ", collat_balance: " << parameter.eos_balance;
    std::cout << ", nut_collat_balance: " << parameter.nut_collat_balance << "}";
#endif

    return parameter;
}

static auto print_voter(ds_chain &chain, const ds_account &name) {
    govvoter voter;
    if (chain.find_table(EOSDTGOVERNC, name, N(voters)) != nullptr
        && chain.get_table_entry(voter, EOSDTGOVERNC, name, N(voters), (NUT_SYMBOL).to_symbol_code(), false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\tvoter: " << name;
        std::cout << ", voting_amount: " << voter.voting_amount;
        std::cout << ", withdrawal_date: ";
        print_date(voter.withdrawal_date);
        std::cout << "}";
#endif
    } else {
        voter.voting_amount = ds_asset(0, NUT_SYMBOL);
    }
    return voter;
}

static auto print_govvoter(ds_chain &chain, const ds_account &name) {
    govvoter voter;
    if (chain.get_table_entry(voter, EOSDTGOVERNC, EOSDTGOVERNC, N(govvoters), name.to_uint64_t(), false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\tvoter: " << name;
        std::cout << ", voting_amount: " << voter.voting_amount;
        std::cout << ", withdrawal_date: ";
        print_date(voter.withdrawal_date);
        std::cout << "}";
#endif
    } else {
        voter.voting_amount = ds_asset(0, NUT_SYMBOL);
    }
    return voter;
}

static auto print_vote(ds_chain &chain, ds_ulong id) {
    govvote vote;
    chain.get_table_entry(vote, EOSDTGOVERNC, EOSDTGOVERNC, N(votes), id);
#ifdef PRINT
    print_time(chain);
    std::cout << "\tvote: {id: " << vote.id;
    std::cout << ", proposal_name: " << vote.proposal_name;
    std::cout << ", voter: " << vote.voter;
    std::cout << ", vote: " << (vote.vote == 1 ? "YES" : "NO");
    std::cout << ", updated_at: ";
    print_time(vote.updated_at);
    std::cout << " }";
#endif
    return vote;
}

static auto print_votes(ds_chain &chain) {
    auto count = 0;
    govvote vote;
    for (auto index = 0; index < 100; index++) {
        if (chain.get_table_entry(vote, EOSDTGOVERNC, EOSDTGOVERNC, N(votes), index, false)) {
            print_vote(chain, index);
            count++;
        }
    }
    return count;
}

static auto print_govparam(ds_chain &chain) {
    govparam param;
    param.NUT_voting_balance = ds_asset(0, NUT_SYMBOL);
    if (chain.get_table_entry(param, EOSDTGOVERNC, EOSDTGOVERNC, N(govparams), 0, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\tparam: {param_id: " << param.param_id;
        std::cout << ", NUT_voting_balance: " << param.NUT_voting_balance;
        std::cout << " }";
#endif
    }
    return param;
}

static auto print_govbpparam(ds_chain &chain, ds_account id) {
    govbpparam bpparam;
    if (chain.get_table_entry(bpparam, EOSDTGOVERNC, EOSDTGOVERNC, N(govbpparams), id.to_uint64_t(), false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\tbpparam: {bp_name: " << bpparam.bp_name;
        std::cout << ", reward_amount: " << bpparam.reward_amount;
        std::cout << ", balance: " << bpparam.balance;
        std::cout << ", enabled: " << (bpparam.enabled == 1 ? "YES" : "NO");
        std::cout << ", is_active: " << (bpparam.is_active == 1 ? "YES" : "NO");
        std::cout << ", active_since: ";
        print_date(bpparam.active_since);
        std::cout << " }";
#endif
    }
    return bpparam;
}


static auto print_govbpvote(ds_chain &chain, const ds_account &producer) {
    govbpvote bpvote;
    chain.get_table_entry(bpvote, EOSDTGOVERNC, EOSDTGOVERNC, N(bpvotes), producer.to_uint64_t());
#ifdef PRINT
    print_time(chain);
    std::cout << "\tbpvote: {producer: " << bpvote.producer;
    std::cout << ", votes: " << bpvote.votes;
    std::cout << " }";
#endif
    return bpvote;
}

static auto print_locsetting(ds_chain &chain, const ds_string &comment = "") {

    locsetting setting;
    if (chain.get_table_entry(setting, EOSDTLOCKUPP, EOSDTLOCKUPP, N(locsettings), 0, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "locsetting: { ";


        std::cout << "setting_id : " << setting.setting_id;
        std::cout << ", eosdtcntract_account : " << setting.eosdtcntract_account;
        std::cout << ", beneficiary : " << setting.beneficiary;
        std::cout << ", nut_reward : " << setting.nut_reward;
        std::cout << ", claim_amounts : [";
        for (auto claim_amount:setting.claim_amounts) {
            std::cout << claim_amount << ",";
        }
        std::cout << "] ,claim_dates : [";
        for (auto claim_date:setting.claim_dates) {
            print_date(claim_date);
            std::cout << ",";
        }

        std::cout << "] }";
#endif
    }
    return setting;
}

static auto print_defiideavote(ds_chain &chain, ds_ulong id) {
    defiideavote defiideavote;
    if (chain.get_table_entry(defiideavote, EOSDEFIVOTES, EOSDEFIVOTES, N(defideavotes), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\tdefiideavote: {idea_id: " << defiideavote.idea_id;
        std::cout << ", votes: " << defiideavote.votes;
        std::cout << " }";
#endif
    }
    return defiideavote;
}


static auto print_defiidea(ds_chain &chain, ds_ulong id) {
    defiidea defiidea;
    if (chain.get_table_entry(defiidea, EOSDEFIVOTES, EOSDEFIVOTES, N(defideas), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\tdefiidea: {idea_id: " << defiidea.idea_id;
        std::cout << ", idea_name: " << defiidea.idea_name;
        std::cout << " }";
#endif
    }
    return defiidea;
}

static auto print_defiideavoter(ds_chain &chain, ds_ulong id) {
    defiideavoter defiideavoter;
    if (chain.get_table_entry(defiideavoter, EOSDEFIVOTES, EOSDEFIVOTES, N(defideavotrs), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\tdefiideavoter: {voter: " << defiideavoter.voter;
        std::cout << ", voting_amount: " << defiideavoter.voting_amount;
        std::cout << " }";
#endif
    }
    return defiideavoter;
}

static auto print_xchtokens(ds_chain &chain, ds_ulong id) {
    xchtoken xchtoken;
    if (chain.get_table_entry(xchtoken, EOS2DTDOTCOM, EOS2DTDOTCOM, N(xchtokens), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\txchtoken: {idea_id: " << xchtoken.token_symbol;
        std::cout << ", idea_name: " << xchtoken.token_account;
        std::cout << ", min_trade: " << xchtoken.min_trade;
        std::cout << " }";
#endif
    }
    return xchtoken;
}

static auto print_xchpair(ds_chain &chain, ds_ulong id) {
    xchpair xchpair;
    if (chain.get_table_entry(xchpair, EOS2DTDOTCOM, EOS2DTDOTCOM, N(xchpairs), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << "\txchpair: {pair_id: " << xchpair.pair_id;
        std::cout << ", base_currency: " << xchpair.base_currency;
        std::cout << ", quote_currency: " << xchpair.quote_currency;
        std::cout << ", total_base_balance: " << xchpair.total_base_balance;
        std::cout << ", total_quote_balance: " << xchpair.total_quote_balance;
        std::cout << ", buy_slippage: " << xchpair.buy_slippage;
        std::cout << ", sell_slippage: " << xchpair.sell_slippage;
        std::cout << ", price_currency: " << xchpair.price_currency;
        std::cout << ", price_type: " << xchpair.price_type;
        std::cout << ", manager_account: " << xchpair.manager_account;
        std::cout << ", price: " << xchpair.price;
        std::cout << " }";
#endif
    }
    return xchpair;
}

static auto print_savsetting(ds_chain &chain, const ds_string &comment = "") {

    savsetting setting;
    chain.get_table_entry(setting, EOSDTSAVINGS, EOSDTSAVINGS, N(savsettings), 0);
#ifdef PRINT
    print_time(chain);
    std::cout << comment << "savsettings: { ";
    std::cout << " setting_id: " << setting.setting_id;
    std::cout << " ,sttoken_account : " << setting.sttoken_account;
    std::cout << " ,min_deposit : " << setting.min_deposit;
    std::cout << " }";
#endif
    return setting;
}


static auto print_savparam(ds_chain &chain, const ds_string &comment = "") {

    savparam param;
    chain.get_table_entry(param, EOSDTSAVINGS, EOSDTSAVINGS, N(savparams), 0);
#ifdef PRINT
    print_time(chain);
    std::cout << comment << "savparams: { ";
    std::cout << " param_id: " << param.param_id;
    std::cout << " ,total_discounted_balance : " << param.total_discounted_balance;
    std::cout << " }";
#endif
    return param;
}

static auto print_savposition(ds_chain &chain, const ds_ulong id, const ds_string &comment = "") {

    savposition position;
    if (chain.get_table_entry(position, EOSDTSAVINGS, EOSDTSAVINGS, N(savpositions), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "savpositions: { ";
        std::cout << " position_id: " << position.position_id;
        std::cout << " ,owner : " << position.owner;
        std::cout << " ,balance : " << position.balance;
        std::cout << " }";
#endif
    } else {
        position.position_id = -1;
    }
    return position;
}

static auto print_swpsetting(ds_chain &chain, const ds_string &comment = "") {
    swpsetting setting;
    if (chain.get_table_entry(setting, TOKENSWAP_EQ, TOKENSWAP_EQ, N(swpsettings), 0, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "swpsetting: { ";
        std::cout << " setting_id: " << setting.setting_id;
        std::cout << " ,min_deposit: " << setting.min_deposit;
        std::cout << " ,token_address : " << setting.token_address;
        std::cout << " ,swap_lock : " << setting.swap_lock;

        std::cout << " }";
#endif
    }
    return setting;
}

static auto print_swpparam(ds_chain &chain, const ds_string &comment = "") {
    swpparam param;
    if (chain.get_table_entry(param, TOKENSWAP_EQ, TOKENSWAP_EQ, N(swpparams), 0, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "swpparam: { ";
        std::cout << " param_id: " << param.param_id;
        std::cout << " ,total_nut_amount: " << param.total_nut_amount;

        std::cout << " }";
#endif
    }
    return param;
}

static auto print_swpposition(ds_chain &chain, const ds_long id, const ds_string &comment = "") {
    swpposition position;
    if (chain.get_table_entry(position, TOKENSWAP_EQ, TOKENSWAP_EQ, N(swppositions), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "swpposition: { ";
        std::cout << " position_id: " << position.position_id;
        std::cout << " ,moment: ";
        print_date(position.moment);
        std::cout << " ,nut_amount : " << position.nut_amount;
        std::cout << " ,eth_address : " << position.eth_address.str();

        std::cout << " }";
#endif
    }
    return position;
}

static auto print_betsetting(ds_chain &chain, const ds_string &comment = "") {
    betsetting setting;
    if (chain.get_table_entry(setting, EOSLOTTERYEQ, EOSLOTTERYEQ, N(betsettings), 0, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "betsetting: { ";
        std::cout << " setting_id: " << setting.setting_id;
        std::cout << " ,manager_account: " << setting.manager_account;
        std::cout << " ,withdrawal_fee : " << setting.withdrawal_fee;
        std::cout << " ,reveal_timer : " << setting.reveal_timer;
        std::cout << " ,manager_reveal_timer : " << setting.manager_reveal_timer;
        std::cout << " }";
#endif
    }
    return setting;
}

static auto print_betdraw(ds_chain &chain, const ds_long id, const ds_string &comment = "") {
    betdraw draw;
    if (chain.get_table_entry(draw, EOSLOTTERYEQ, EOSLOTTERYEQ, N(betdraws), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "betdraw: { ";
        std::cout << " draw_id: " << draw.draw_id;
        std::cout << " ,draw_type: " << draw.draw_type;
        std::cout << " ,asset_contract: " << draw.asset_contract;
        std::cout << " ticket_cost: " << draw.ticket_cost;
        std::cout << " ,draw_end_time: ";
        print_date(draw.draw_end_time);
        std::cout << " ,reveal_stop_time: ";
        print_date(draw.reveal_stop_time);
        std::cout << " ,reveal_stop_time: ";
        print_date(draw.reveal_stop_time);
        std::cout << " ,draw_status : " << draw.draw_status;
        std::cout << " ,jackpot : " << draw.jackpot;
        std::cout << ", winner_weights : [";
        for (auto winner_weight:draw.winner_weights) {
            std::cout << winner_weight << ",";
        }
        std::cout << "] ";
        std::cout << " }";
#endif
    }
    return draw;
}

static auto print_betposition(ds_chain &chain, const ds_long id, const ds_string &comment = "") {
    betposition position;
    if (chain.get_table_entry(position, EOSLOTTERYEQ, EOSLOTTERYEQ, N(betpositions), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "betposition: { ";
        std::cout << " position_id: " << position.position_id;
        std::cout << " ,account_id : " << position.account_id;
        std::cout << " ,draw_id : " << position.draw_id;
        std::cout << " ,win_amount : " << position.win_amount;
        std::cout << " }";
#endif
    }
    return position;
}


static auto print_betsecret(ds_chain &chain, const ds_long id, const ds_string &comment = "") {
    betsecret secret;
    if (chain.get_table_entry(secret, EOSLOTTERYEQ, EOSLOTTERYEQ, N(betsecrets), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "betsecret: { ";
        std::cout << " secret_id: " << secret.secret_id;
        std::cout << " ,position_id: " << secret.position_id;
        std::cout << " ,draw_id : " << secret.draw_id;
        std::cout << " ,seed : " << secret.seed.str();
        std::cout << " ,secret : " << secret.secret;
        std::cout << " }";
#endif
    }
    return secret;
}

static auto print_cmkasset(ds_chain &chain, const ds_symbol id, const ds_string &comment = "") {
    cmkasset cmkasset;
    if (chain.get_table_entry(cmkasset, CMKRYVESTING, CMKRYVESTING, N(cmkassets), id.to_symbol_code(), false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "cmkasset: { ";
        std::cout << " asset_symbol: " << cmkasset.asset_symbol;
        std::cout << " ,asset_contract: " << cmkasset.asset_contract;
        std::cout << " ,asset_balance : " << cmkasset.asset_balance;
        std::cout << " ,unallocated_balance : " << cmkasset.unallocated_balance;
        std::cout << " }";
#endif
    }
    return cmkasset;
}

static auto print_cmkvesting(ds_chain &chain, const ds_long id, const ds_string &comment = "") {
    cmkvesting cmkvesting;
    if (chain.get_table_entry(cmkvesting, CMKRYVESTING, CMKRYVESTING, N(cmkvestings), id, false)) {
#ifdef PRINT
        print_time(chain);
        std::cout << comment << "cmkvesting: { ";
        std::cout << " vesting_id: " << cmkvesting.vesting_id;
        std::cout << " ,account_id: " << cmkvesting.account_id;
        std::cout << " ,vesting_period : " << cmkvesting.vesting_period;
        std::cout << " ,vesting_amount : " << cmkvesting.vesting_amount;
        std::cout << " ,cliff_date : ";
        print_date(cmkvesting.cliff_date);
        std::cout << " ,cliff_weight: " << cmkvesting.cliff_weight;
        std::cout << " ,start_date: ";
        print_date(cmkvesting.start_date);
        std::cout << " ,end_date : ";
        print_date(cmkvesting.end_date);
        std::cout << " ,pause_start : ";
        print_date(cmkvesting.pause_start);
        std::cout << " ,pause_time : " << cmkvesting.pause_time;
        std::cout << " ,status : " << cmkvesting.status;
        std::cout << " ,vested_amount : " << cmkvesting.vested_amount;
        std::cout << " }";
#endif
    }
    return cmkvesting;
}


#endif