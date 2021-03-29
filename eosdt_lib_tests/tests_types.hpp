#ifndef TESTS_TYPES_HPP
#define TESTS_TYPES_HPP

#include "tests_defines.hpp"

struct sysaccount {
    ds_asset balance;

    ds_ulong primary_key() const { return balance.get_symbol().value(); }
};


struct sysstat {
    ds_asset supply;
    ds_asset max_supply;
    ds_account issuer;

    uint64_t
    primary_key() const {
        return supply.get_symbol().to_symbol_code();
    }
};

struct sysproducer {
    ds_account owner;
    double total_votes;
    ds_public_key producer_key;
    bool is_active;
    ds_string url;
    ds_uint unpaid_blocks;
    ds_ulong last_claim_time;
    uint16_t location;

    auto primary_key() const { return owner.to_uint64_t(); }
};

struct tokissuers {
    ds_account issuer;
    ds_asset max_supply;
    ds_asset supply;
    bool is_active;

    uint64_t
    primary_key() const { return issuer.to_uint64_t(); }
};

struct orasettings {
    ds_ulong id;
    ds_int rate_timeout;
    ds_int query_timeout;
    ds_int provablecb1a_interval;
    ds_int delphioracle_interval;
    ds_int equilibriumdsp_interval;
    ds_int backend_interval;

    ds_ulong primary_key() const { return id; }
};

struct orarate {
    ds_asset rate;
    ds_time update;
    ds_asset provablecb1a_price;
    ds_time provablecb1a_update;
    ds_asset delphioracle_price;
    ds_time delphioracle_update;
    ds_asset equilibriumdsp_price;
    ds_time equilibriumdsp_update;
    ds_asset backend_price;
    ds_time backend_update;
    ds_ulong id;
    ds_symbol base;

    uint64_t
    primary_key() const {
        return rate.get_symbol().value();
    }
};

struct oraqueries {
    ds_symbol asset_symbol;
    ds_string query;
    ds_int price_type;
    ds_time query_updated_at;
    ds_time query_executed_at;
    ds_checksum checksumm;
    ds_ulong id;
    ds_account source_contract;
    ds_symbol base;
    double filter;

    uint64_t primary_key() const {
        return id;
    }
};

struct orasubscribe {
    ds_account contract;
    ds_asset quantity;
    ds_uint callcount;
    ds_time lastcall;
    ds_time withdrawal_date;
    ds_ulong id;
    ds_symbol asset_symbol;
    ds_symbol base;

    uint64_t
    primary_key() const {
        return id;
    }
};


struct datapoint {
    ds_ulong id;
    ds_account owner;
    ds_ulong value;
    ds_ulong median;
    ds_ulong timestamp;

    ds_ulong primary_key() const { return id; }
};


struct ctrsetting {
    ds_ulong setting_id;
    uint8_t global_lock;
    ds_long time_shift;
    ds_account liquidator_account;
    ds_account oraclize_account;
    ds_account sttoken_account;
    ds_account nutoken_account;
    double governance_fee;
    double stability_fee;
    double critical_ltv;
    double liquidation_penalty;
    double liquidator_discount;
    ds_asset liquidation_price;
    double nut_auct_ratio;
    double nut_discount;
    double profit_factor;
    ds_uint vote_period;
    ds_uint stake_period;
    double reserve_ratio;
    double staking_weight;
    ds_account bpproxy_account;
    ds_account governc_account;
    double referral_ratio;
    ds_account savings_account;

    ds_ulong primary_key() const { return setting_id; }
};

struct ctrparameter {
    ds_ulong parameter_id;
    double total_collateral;
    ds_asset total_debt;
    double stability_rate;
    double governance_rate;
    ds_time prev_date;
    ds_time prev_vote;
    ds_time prev_stake;
    ds_asset eos_staked;

    ds_ulong primary_key() const { return parameter_id; }
};

struct posparameter {
    ds_ulong parameter_id;
    double total_collateral;
    ds_asset total_debt;
    double stability_rate;
    ds_time prev_date;

    ds_ulong primary_key() const { return parameter_id; }
};

struct ctrposition {
    ds_ulong position_id;
    ds_account maker;
    ds_asset outstanding;
    ds_asset governance;
    double collateral;

    uint64_t
    primary_key() const { return position_id; }

    uint64_t
    get_maker() const { return maker.to_uint64_t(); }
};

struct posposition {
    ds_ulong position_id;
    ds_account maker;
    ds_asset outstanding;
    double collateral;

    uint64_t
    primary_key() const { return position_id; }

    uint64_t
    get_maker() const { return maker.to_uint64_t(); }
};

struct ctrltvratio {
    ds_ulong position_id;
    double ltv_ratio;

    uint64_t primary_key() const { return position_id; }

    uint64_t by_ltv_ratio() const { return ltv_ratio; }
};

struct ctrreferral {
    ds_ulong referral_id;
    ds_account referral;
    ds_asset staked_amount;

    uint64_t primary_key() const { return referral_id; }

    uint64_t by_referral() const { return referral.to_uint64_t(); }
};

struct ctrpositionref {
    ds_ulong position_id;
    ds_ulong referral_id;

    uint64_t primary_key() const { return position_id; }

    uint64_t by_referral_id() const { return referral_id; }
};

struct liqparameter {
    ds_ulong parameter_id;
    ds_asset surplus_debt;
    ds_asset bad_debt;
    ds_asset eos_balance;
    ds_asset nut_collat_balance;

    ds_ulong primary_key() const { return parameter_id; }
};


struct liqsetting {
    ds_ulong setting_id;
    ds_account eosdtcntract_account;
    uint8_t global_unlock;
    ds_asset auction_price;
    double burn_rate;
    double gov_return_rate;
    double set_aside_rate;
    double savings_rate;

    ds_ulong primary_key() const { return setting_id; }
};

struct govsetting {
    ds_ulong setting_id;
    ds_account eosdtcntract_account;
    ds_asset min_proposal_weight;
    ds_uint freeze_period;
    double min_participation;
    double success_margin;
    ds_uint top_holders_amount;
    ds_uint max_bp_count;
    ds_uint max_bp_votes;
    ds_asset min_vote_stake;
    ds_uint unstake_period;

    ds_ulong primary_key() const { return setting_id; }
};

struct govproposal {
    ds_account proposal_name;
    ds_account proposer;
    ds_string title;
    ds_string proposal_json;
    ds_time created_at;
    ds_time expires_at;
    uint8_t proposal_type;

    auto primary_key() const { return proposal_name.to_uint64_t(); }

    uint64_t
    by_proposer() const { return proposer.to_uint64_t(); }

};


struct govvoter {
    ds_account voter;
    ds_asset voting_amount;
    ds_time withdrawal_date;

    ds_ulong primary_key() const {
        return voter.to_uint64_t();
    }
};


static __uint128_t compress_key(const ds_ulong &left, const ds_ulong &right) {
    return ((__uint128_t) left) << 64 | right;
}

struct govvote {
    ds_ulong id;
    ds_account proposal_name;
    ds_account voter;
    uint8_t vote;
    ds_time updated_at;
    ds_string vote_json;

    auto primary_key() const { return id; }

    __uint128_t by_proposal() const { return compress_key(proposal_name.to_uint64_t(), voter.to_uint64_t()); }

    __uint128_t by_voter() const { return compress_key(voter.to_uint64_t(), proposal_name.to_uint64_t()); }
};

struct govbpvote {
    ds_account producer;
    ds_asset votes;

    uint64_t
    primary_key() const { return producer.to_uint64_t(); }

    uint64_t
    byvotes() const { return votes.get_amount(); }
};

struct govparam {
    ds_ulong param_id;
    ds_asset NUT_voting_balance;

    uint64_t
    primary_key() const { return param_id; }
};

struct govbpparam {
    ds_account bp_name;
    ds_asset reward_amount;
    ds_asset balance;
    bool enabled;
    bool is_active;
    ds_time active_since;

    uint64_t
    primary_key() const { return bp_name.to_uint64_t(); }
};

struct govtopbp {
    ds_account bp_name;
    ds_asset reward_amount;

    uint64_t
    primary_key() const { return bp_name.to_uint64_t(); }
};

struct ressetting {
    ds_ulong setting_id;
    ds_account eosdtcntract_account;
    ds_uint unstake_period;
    ds_asset mindeposit;
    ds_uint maxdepositors;

    ds_ulong primary_key() const { return setting_id; }
};


struct respositions {
    ds_ulong position_id;
    ds_account depositor;
    ds_asset eos_balance;
    ds_time withdrawal_date;


    ds_ulong primary_key() const { return position_id; }

    ds_ulong get_depositor() const { return depositor.to_uint64_t(); }

    ds_ulong get_balance() const { return eos_balance.get_amount(); }
};


struct locsetting {
    ds_ulong setting_id;
    ds_account eosdtcntract_account;
    ds_account beneficiary;
    ds_asset nut_reward;
    std::vector<ds_asset> claim_amounts;
    std::vector<ds_time> claim_dates;

    ds_ulong primary_key() const { return setting_id; }
};

struct defiideavote {
    ds_ulong idea_id;
    ds_asset votes;//EOSDT

    ds_ulong primary_key() const { return idea_id; }
};

struct defiidea {
    ds_ulong idea_id;
    ds_string idea_name;

    ds_ulong primary_key() const { return idea_id; }

    ds_string by_name() const { return idea_name; }
};

struct defiideavoter {
    ds_account voter;
    ds_asset voting_amount;//1 EOSD

    ds_ulong primary_key() const { return voter.to_uint64_t(); }
};

struct xchpair {
    ds_ulong pair_id;
    ds_symbol base_currency;
    ds_symbol quote_currency;
    ds_asset total_base_balance;
    ds_asset total_quote_balance;
    double buy_slippage;
    double sell_slippage;
    ds_symbol price_currency;
    ds_uint price_type;
    ds_account manager_account;
    ds_asset price;

    ds_ulong primary_key() const { return pair_id; }
};

struct xchtoken {
    ds_symbol token_symbol;
    ds_account token_account;
    ds_asset min_trade;

    ds_ulong primary_key() const { return token_symbol.value(); }
};


struct tonorder {
    ds_ulong order_id;

    ds_string primary_chain_id;
    ds_string primary_trx_id;
    ds_string primary_address;
    ds_string side_chain_id;
    ds_string side_trx_id;
    ds_string side_address;

    ds_asset quantity;
    ds_time created_at;
    ds_int order_status;
    ds_ulong confirm_mask;
    ds_ulong cancel_mask;

    ds_ulong primary_key() const { return order_id; }
};

struct tonparam {
    ds_ulong param_id;
    ds_asset ton_gram_balance;
    ds_asset total_fees;

    ds_ulong primary_key() const { return param_id; }
};

struct savsetting {
    ds_ulong setting_id;
    ds_account sttoken_account;
    ds_asset min_deposit;

    ds_ulong primary_key() const { return setting_id; }
};

struct savparam {
    ds_ulong param_id;
    ds_asset total_discounted_balance;

    ds_ulong
    primary_key() const { return param_id; }
};

struct savposition {
    ds_ulong position_id;
    ds_account owner;
    ds_asset balance;

    ds_ulong
    primary_key() const { return position_id; }

    ds_ulong
    get_owner() const { return owner.to_uint64_t(); }
};

struct swpsetting {
    ds_ulong setting_id;
    ds_asset min_deposit;
    ds_account token_address;
    uint8_t swap_lock;

    ds_ulong primary_key() const { return 0; }
};

struct swpparam {
    ds_ulong param_id;
    ds_asset total_nut_amount;

    uint64_t
    primary_key() const { return 0; }
};

struct swpposition {
    ds_ulong position_id;
    ds_time moment;
    ds_asset nut_amount;
    eosio::chain::checksum160_type eth_address;

    ds_ulong primary_key() const { return position_id; }

    //ds_checksum by_eth_address_and_moment() const { return compress_key(string_add_date(eth_address,moment)); }
};

struct betsetting {
    ds_ulong setting_id;
    ds_account manager_account;
    double withdrawal_fee;
    ds_uint reveal_timer;
    ds_uint manager_reveal_timer;
    ds_uint max_draw_timer;

    ds_ulong primary_key() const { return 0; }
};

struct betdraw {
    ds_ulong draw_id;
    ds_uint draw_type;//0-time end of timer / 1-fill entire amount filled
    ds_account asset_contract;
    ds_asset ticket_cost;
    ds_time created;
    ds_time draw_end_time;
    ds_time reveal_start_time;
    ds_time reveal_stop_time;
    ds_uint draw_status;//new, active, completed.
    ds_asset jackpot;
    std::vector<double> winner_weights;

    ds_ulong primary_key() const { return draw_id; }
};

struct betposition {
    ds_ulong position_id;
    ds_account account_id;
    ds_ulong draw_id;
    ds_asset win_amount;

    ds_ulong primary_key() const { return position_id; }

    ds_ulong by_draw_id() const { return draw_id; }
};

struct betsecret {
    ds_ulong secret_id;
    ds_ulong position_id;
    ds_ulong draw_id;
    ds_checksum seed;
    ds_string secret;

    ds_ulong primary_key() const { return secret_id; }

    ds_ulong by_position_id() const { return position_id; }

    ds_ulong by_draw_id() const { return draw_id; }

    ds_checksum by_seed() const { return seed; }
};

struct cmkasset {
    ds_symbol asset_symbol;
    ds_account asset_contract;
    ds_asset asset_balance;
    ds_asset unallocated_balance;

    ds_ulong primary_key() const { return asset_symbol.to_symbol_code(); }
};

struct cmkvesting {
    ds_ulong vesting_id;
    ds_account account_id;
    ds_uint vesting_period;
    ds_asset vesting_amount;
    ds_time cliff_date;
    double cliff_weight;
    ds_time start_date;
    ds_time end_date;
    ds_time lock_date;
    ds_time pause_start;
    ds_ulong pause_time;
    ds_uint status;
    ds_asset vested_amount;

    ds_ulong primary_key() const { return vesting_id; }
};

#endif
