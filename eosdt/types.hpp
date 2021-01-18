#pragma once

#include "enums.hpp"


namespace eosdt{

        struct sysaccount {
            ds_asset balance;

            ds_ulong primary_key() const { return balance.symbol.raw(); }
        };


        struct sysstat {
            ds_asset supply;
            ds_asset max_supply;
            ds_account issuer;

            uint64_t
            primary_key() const {
                return supply.symbol.code().raw();
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
            eosio::binary_extension<eosio::block_signing_authority>  producer_authority;
            auto primary_key() const { return owner.value; }
            double by_votes()const    { return is_active ? -total_votes : total_votes;  }
            template<typename DataStream>
            friend DataStream& operator << ( DataStream& ds, const sysproducer& t ) {
                ds << t.owner
                   << t.total_votes
                   << t.producer_key
                   << t.is_active
                   << t.url
                   << t.unpaid_blocks
                   << t.last_claim_time
                   << t.location;

                if( !t.producer_authority.has_value() ) return ds;

                return ds << t.producer_authority;
            }

            template<typename DataStream>
            friend DataStream& operator >> ( DataStream& ds, sysproducer& t ) {
                return ds >> t.owner
                          >> t.total_votes
                          >> t.producer_key
                          >> t.is_active
                          >> t.url
                          >> t.unpaid_blocks
                          >> t.last_claim_time
                          >> t.location
                          >> t.producer_authority;
            }
        };

        struct sysglobal: eosio::blockchain_parameters {
            uint64_t             max_ram_size;
            uint64_t             total_ram_bytes_reserved;
            int64_t              total_ram_stake;
            eosio::block_timestamp      last_producer_schedule_update;
            eosio::time_point           last_pervote_bucket_fill;
            int64_t              pervote_bucket;
            int64_t              perblock_bucket;
            uint32_t             total_unpaid_blocks;
            int64_t              total_activated_stake;
            eosio::time_point           thresh_activated_stake_time;
            uint16_t             last_producer_schedule_size;
            double               total_producer_vote_weight;
            eosio::block_timestamp      last_name_close;
            uint64_t primary_key()const { return 0; }
        };

        struct sysglobal4 {
            double   continuous_rate;
            int64_t  inflation_pay_factor;
            int64_t  votepay_factor;

            uint64_t primary_key()const { return 0; }
        };

        struct sysvoter {
            ds_account                owner;
            ds_account                proxy;
            std::vector<ds_account>   producers;
            int64_t             staked;
            double              last_vote_weight;
            double              proxied_vote_weight;
            bool                is_proxy;
            uint32_t            flags1;
            uint32_t            reserved2;
            ds_asset        reserved3;

            uint64_t primary_key()const { return owner.value; }
        };

        struct rexpool {
            uint8_t version;
            ds_asset total_lent;
            ds_asset total_unlent;
            ds_asset total_rent;
            ds_asset total_lendable;
            ds_asset total_rex;
            ds_asset namebid_proceeds;
            uint64_t loan_num;
            uint64_t
            primary_key()const { return 0; }
        };

        struct rexbalance {
            uint8_t version;
            ds_account owner;
            ds_asset vote_stake;
            ds_asset rex_balance;
            int64_t matured_rex;
            std::deque <std::pair<eosio::time_point_sec, int64_t>> rex_maturities;

            uint64_t
            primary_key()const { return owner.value; }
        };

        struct rexqueue {
            uint8_t version;
            ds_account owner;
            ds_asset rex_requested;
            ds_asset proceeds;
            ds_asset stake_change;
            eosio::time_point order_time;
            bool is_open = true;

            void close() { is_open = false; }
            uint64_t
            primary_key()const { return owner.value; }
            uint64_t
            by_time()const     { return is_open ? order_time.elapsed.count() : std::numeric_limits<uint64_t>::max(); }
        };

        struct rexfund {
            uint8_t version;
            ds_account owner;
            ds_asset balance;
            uint64_t
            primary_key()const { return owner.value; }
        };

        struct orasettings
        {
            ds_ulong id;
            ds_int rate_timeout;
            ds_int query_timeout;
            ds_int provablecb1a_interval;
            ds_int delphioracle_interval;
            ds_int equilibriumdsp_interval;
            ds_int validity_timeout;

            ds_ulong primary_key() const { return id; }
        };

        static __uint128_t compress_key(const ds_ulong &left, const ds_ulong &right) {
            return ((__uint128_t) left) << 64 | right;
        }

        struct orarate
        {
            ds_asset rate;
            ds_time update;
            ds_asset provablecb1a_price;
            ds_time provablecb1a_update;
            ds_asset delphioracle_price;
            ds_time delphioracle_update;
            ds_asset equilibriumdsp_price;
            ds_time equilibriumdsp_update;
            ds_ulong id;
            ds_symbol base;

            uint64_t
            primary_key() const {
                return id;
            }

            __uint128_t by_rate_base() const { return compress_key(rate.symbol.code().raw(), base.code().raw()); }
        };

        struct oraqueries
        {
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

            uint64_t
            primary_key() const {
                return id;
            }

            __uint128_t by_asset_source() const { return compress_key(asset_symbol.code().raw(), source_contract.value^base.code().raw()); }
        };

        struct orasubscribe
        {
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

            __uint128_t by_asset_contract() const { return compress_key(asset_symbol.code().raw(), contract.value^base.code().raw()); }
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
            //Rex>>
            ds_uint stake_period;
            double reserve_ratio;
            double staking_weight;
            ds_account bpproxy_account;
            //<<Rex
            ds_account governc_account;
            ds_asset referral_min_stake;
            double referral_ratio;
            ds_account collateral_account;
            ds_symbol collateral_token;
            ds_account savings_account;
            ds_asset min_pos;
            ds_account tokenswap_account;

            ds_uint top_margincalls() const { return 3; }
            ds_ulong primary_key() const { return setting_id; }
        };

        struct ctrsetting_time {
            ds_ulong setting_id;
            uint8_t global_lock;
            ds_long time_shift;

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

        struct ctrltvratio {
            ds_ulong position_id;
            double ltv_ratio;

            uint64_t
            primary_key() const { return position_id; }
            double
            by_ltv_ratio() const { return ltv_ratio; }
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
            get_maker() const { return maker.value; }
        };

        struct posposition {
            ds_ulong position_id;
            ds_account maker;
            ds_asset outstanding;
            double collateral;

            uint64_t
            primary_key() const { return position_id; }
            uint64_t
            get_maker() const { return maker.value; }
        };

        struct ctrreferral{
            ds_ulong referral_id;
            ds_account referral;
            ds_asset staked_amount;

            uint64_t primary_key() const { return referral_id;}
            uint64_t by_referral() const { return referral.value;}
        };

        struct ctrpositionref{
            ds_ulong position_id;
            ds_ulong referral_id;


            uint64_t primary_key() const { return position_id;}
            uint64_t by_referral_id() const { return referral_id;}
        };

        struct liqparameter {
            ds_ulong parameter_id;
            ds_asset surplus_debt;
            ds_asset bad_debt;
            ds_asset collat_balance;
            ds_asset nut_collat_balance;

            ds_ulong primary_key() const { return parameter_id; }
        };


        struct liqsetting {
            ds_ulong setting_id;
            ds_account position_account;
            uint8_t global_unlock;
            ds_asset auction_price;
            double burn_rate;
            double gov_return_rate;
            double tokenswap_rate;
            double savings_rate;
            ds_ulong primary_key() const { return setting_id; }
        };

        struct govsetting {
            ds_ulong setting_id;
            ds_account position_account;
            ds_asset min_proposal_weight;
            ds_uint freeze_period;
            double min_participation;
            double success_margin;
            ds_uint top_holders_amount;
            ds_uint max_bp_count;
            ds_uint max_bp_votes;
            ds_asset min_vote_stake;
            ds_uint unstake_period;
            double reward_weight;
            double stake_reward;

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

            auto primary_key() const { return proposal_name.value; }

            uint64_t
            by_proposer() const { return proposer.value; }

        };


        struct govvoter {
            ds_account voter;
            ds_asset voting_amount;
            ds_time withdrawal_date;

            ds_ulong primary_key() const {
                return voter.value;
            }
        };

        struct govvote {
            ds_ulong id;
            ds_account proposal_name;
            ds_account voter;
            uint8_t vote;
            ds_time updated_at;
            ds_string vote_json;

            auto primary_key() const { return id; }

            __uint128_t by_proposal() const { return compress_key(proposal_name.value, voter.value); }

            __uint128_t by_voter() const { return compress_key(voter.value, proposal_name.value); }
        };

        struct govbpvote
        {
            ds_account producer;
            ds_asset votes;

            uint64_t
            primary_key() const { return producer.value; }

            uint64_t
            byvotes() const { return votes.amount; }
        };

        struct govparam
        {
            ds_ulong param_id;
            ds_asset NUT_voting_balance;
            ds_asset min_reward;

            uint64_t
            primary_key() const { return param_id; }
        };


        struct govbpparam
        {
            ds_account bp_name;
            ds_asset reward_amount;
            ds_asset balance;
            bool enabled;
            bool is_active;
            ds_time active_since;

            uint64_t
            primary_key() const { return bp_name.value; }
        };

        struct govposcntr
        {
            ds_account position_account;

            uint64_t
            primary_key() const { return position_account.value; }
        };

        struct votingstatus_item {
            ds_ulong id;
            ds_time voted_at;
            ds_checksum vote_hash;

            ds_ulong primary_key() const { return id; }
        };


        struct ressetting {
            ds_ulong setting_id;
            ds_account eosdtcntract_account;
            ds_uint unstake_period;
            ds_asset mindeposit;
            ds_uint maxdepositors;

            ds_ulong primary_key() const { return setting_id; }
        };


        struct resparam {
            ds_ulong param_id;
            uint8_t funds_sent;

            ds_ulong primary_key() const { return param_id; }
        };


        struct respositions {
            ds_ulong position_id;
            ds_account depositor;
            ds_asset eos_balance;
            ds_time withdrawal_date;


            ds_ulong primary_key() const { return position_id; }
            ds_ulong get_depositor() const { return depositor.value; }
            ds_ulong get_balance() const { return eos_balance.amount; }
        };

        struct excsetting {
            ds_ulong setting_id;
            ds_asset low_ask;
            ds_asset high_bid;
            ds_asset qty_ask;
            ds_asset qty_bid;
            double fee;
            ds_symbol base;
            ds_symbol quote;
            int num_of_trades;

            ds_ulong primary_key() const { return setting_id; }
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

        struct tstsetting {
            ds_ulong setting_id;
            ds_account eosdtcntract_account;
            ds_time last_update;
            ds_time last_error;

            ds_ulong primary_key() const { return setting_id; }
        };

        struct defiideavote
        {
            ds_ulong idea_id;
            ds_asset votes;//EOSDT

            ds_ulong primary_key() const { return idea_id; }
        };

        ds_checksum compress_key(const ds_string &str)
        {
            return eosio::sha256(const_cast<char*>(str.c_str()), str.size());
        }

        struct defiidea
        {
            ds_ulong idea_id;
            ds_string idea_name;
            ds_ulong primary_key() const { return idea_id; }
            ds_checksum by_name() const { return compress_key(idea_name); }
        };

        struct defiideavoter
        {
            ds_account voter;
            ds_asset voting_amount;//1 EOSD

            ds_ulong primary_key() const { return voter.value; }
        };

    struct savsetting
    {
        ds_ulong setting_id;
        ds_account sttoken_account;
        ds_asset min_deposit;

        ds_ulong primary_key() const { return setting_id; }
    };

    struct savparam
    {
        ds_ulong param_id;
        ds_asset total_discounted_balance;

        uint64_t
        primary_key() const { return param_id; }
    };

    struct savposition {
        ds_ulong position_id;
        ds_account owner;
        ds_asset balance;

        uint64_t
        primary_key() const { return position_id; }
        uint64_t
        get_owner() const { return owner.value; }
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

    ds_string checksum_add_date(const eosio::checksum160 &c,const ds_time t)
    {
        return ds_string(((char*)c.data()),20)+"/"+std::to_string(t.sec_since_epoch());
    }

    ds_checksum convert160_to_256(const eosio::checksum160 &c)
    {
        ds_checksum checksum;
        auto p = (char*)checksum.data();
        auto d = (char*)c.data();
        for(auto i=0;i<20;i++)
        {
            if(i<16) p[i]=d[i];
            if(i>15) p[i+12]=d[i+12];
        }
        return checksum;
    }


    struct swpposition {
        ds_ulong position_id;
        ds_time moment;
        ds_asset nut_amount;
        eosio::checksum160 eth_address;

        ds_ulong primary_key() const { return position_id; }
        ds_checksum by_eth_address() const { return convert160_to_256(eth_address); }
        ds_checksum by_eth_address_and_moment() const { return compress_key(checksum_add_date(eth_address,moment)); }
    };

    struct betsetting{
        ds_ulong setting_id;
        ds_account manager_account;
        double withdrawal_fee;
        ds_uint reveal_timer;
        ds_uint manager_reveal_timer;
        ds_uint max_draw_timer;
        ds_ulong primary_key() const { return 0; }
    };

    struct betdraw{
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
        std::vector<double>   winner_weights;
        ds_ulong primary_key() const { return draw_id; }
    };

    struct betposition{
        ds_ulong position_id;
        ds_account account_id;
        ds_ulong draw_id;
        ds_asset win_amount;
        ds_ulong primary_key() const { return position_id; }
        ds_ulong by_draw_id() const { return draw_id; }
    };

    struct betsecret{
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

    struct cmkasset{
        ds_symbol asset_symbol;
        ds_account asset_contract;
        ds_asset asset_balance;
        ds_asset unallocated_balance;
        ds_ulong primary_key() const { return asset_symbol.code().raw(); }
    };

    struct cmkvesting{
        ds_ulong vesting_id;
        ds_account account_id;
        ds_uint vesting_period;
        ds_asset vesting_amount;
        ds_time cliff_date;
        double cliff_weight;
        ds_time start_date;
        ds_time end_date;
        ds_time pause_start;
        ds_ulong pause_time;
        ds_uint status;
        ds_asset vested_amount;
        ds_ulong primary_key() const { return vesting_id; }
    };
}