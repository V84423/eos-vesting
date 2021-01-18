#pragma once
#define EOSDTCURRENT "cmkryvesting"_n

#include "../../cmkryvesting/include/info.hpp"
#include "../../eosdt/eosdt.hpp"


namespace eosdt {

    class [[eosio::contract("cmkryvesting")]] cmkryvesting : public eosdt::contract {

    public:
        using eosdt::contract::contract;

        /**
        * Allows to receive version of contract by assert
        *
        */
        [[eosio::action]]
        void currentver();

        /**
        * Allows to receive version of contract by print out
        *
        */
        [[eosio::action]]
        void getcurrent();

        /**
         * Dummy notification handler, for linking purposes on earlier eosio.cdt versions.
         */
        [[eosio::on_notify("cmkryvesting::dummy")]]
        void on_notify_dummy() {}

        /**
          * Allows to receive notification about transfer
          * `from` account to transfer to `to`(this contract) account the `quantity` tokens.
          * One account is debited and the other is credited with quantity tokens.
          *
          * @param from - the account to transfer from,
          * @param to - the account to be transferred to,
          * @param quantity - the quantity of tokens to be transferred,
          * @param memo - the memo string to accompany the transaction with pair id.
          *
          * @pre Transaction must be signed by tokens owner
          *
          */
        [[eosio::on_notify("*::transfer")]]
        void on_transfer(const ds_account &from, const ds_account &to,
                         const ds_asset &quantity, const ds_string &memo);

        [[eosio::action]]
        void addasset(const ds_account &asset_contract, const ds_symbol &asset_symbol);

        [[eosio::action]]
        void withdraw(const ds_account &to, const ds_asset &quantity);

        [[eosio::action]]
        void addvesting(const ds_account &account_id,
                        const ds_uint &vesting_period, const ds_asset &vesting_amount,
                        ds_time &cliff_date, const double cliff_weight,
                        const ds_time &start_date, const ds_time end_date);

        [[eosio::action]]
        void pausevesting(const ds_ulong &vesting_id, const ds_time &pause_start);

        [[eosio::action]]
        void unpausevestng(const ds_ulong &vesting_id, const ds_time &pause_end);

        [[eosio::action]]
        void setpausetime(const ds_ulong &vesting_id, const ds_ulong &pause_time);

        [[eosio::action]]
        void cancelvesting(const ds_ulong &vesting_id, const ds_time &cancel_start);

        [[eosio::action]]
        void claim(const ds_ulong &vesting_id);

        [[eosio::action]]
        void removevesting(const ds_ulong &vesting_id);

    private:
        auto vesting_period_name(const ds_uint &vesting_period);

        auto vesting_period_size(const ds_uint &vesting_period);

        auto calc_vesting_percent(const ds_uint &vesting_period,
                                  const ds_time &start, const ds_time &end, const ds_time &curr);

        void assert_dates(const ds_uint &vesting_period, const ds_time &cliff,
                          const ds_time &start, const ds_time end);

        void deposit(const ds_account &asset_contract, const ds_asset &quantity);

        auto assets_get(const ds_symbol &symbol);

        auto assets_move_balance(const ds_asset &asset_balance, const ds_asset &unallocated_balance);

        auto now_fixed_by_pause(const ds_ulong &pause_time, const ds_uint status,const ds_time &pause_start);
    };
} /// namespace eosdt

