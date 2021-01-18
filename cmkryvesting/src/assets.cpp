#include "cmkryvesting.hpp"

namespace eosdt {

    void cmkryvesting::addasset(const ds_account &asset_contract, const ds_symbol &asset_symbol) {
        PRINT_STARTED("addasset"_n)
        require_auth(_self);
        auto max_supply = max_supply_get_by_ctract(asset_contract, asset_symbol);
        ds_assert(max_supply.amount > 0, "asset_symbol(%) should have max supply on this contract(%).",
                  asset_symbol, asset_contract);
        cmkassets_table cmkassets(_self, _self.value);
        ds_assert(cmkassets.find(asset_symbol.code().raw()) == cmkassets.end(),
                  "asset_symbol(%) exists in cmkassets.", asset_symbol);
        cmkassets.emplace(_self, [&](auto &row) {
            row.asset_symbol = asset_symbol;
            row.asset_contract = asset_contract;
            row.asset_balance = ds_asset(0, asset_symbol);
            row.unallocated_balance = ds_asset(0, asset_symbol);
        });
        ds_print("\r\nCreated asset for asset_symbol(%) by contract(%).", asset_symbol, asset_contract);
        PRINT_FINISHED("addasset"_n)
    }

    auto cmkryvesting::assets_move_balance(const ds_asset &asset_balance, const ds_asset &unallocated_balance) {
        ds_assert(asset_balance.symbol == unallocated_balance.symbol,
                  "asset_balance(%) and unallocated_balance(%) should have the same symbol", asset_balance,
                  unallocated_balance);
        cmkassets_table cmkassets(_self, _self.value);
        auto itr = cmkassets.find(asset_balance.symbol.code().raw());
        ds_assert(itr != cmkassets.end(),
                  "Symbol(%) does not exist in cmkassets.", asset_balance.symbol);
        auto balance = balance_get_by_ctract(itr->asset_contract, _self, asset_balance.symbol);
        ds_assert(itr->asset_balance <= balance,
                  "Wrong asset_balance(%) expected less or equal: '%'", itr->asset_balance, balance);
        ds_assert(itr->unallocated_balance <= balance,
                  "Wrong unallocated_balance(%) expected less or equal: '%'", itr->unallocated_balance, balance);
        ds_assert(asset_balance.amount >= 0 || itr->asset_balance >= -asset_balance,
                  "Wrong asset_balance(%) expected less or equal: '%'", -asset_balance, itr->asset_balance);
        ds_assert(unallocated_balance.amount >= 0 || itr->unallocated_balance >= -unallocated_balance,
                  "Wrong unallocated_balance(%) expected less or equal: '%'", -unallocated_balance,
                  itr->unallocated_balance);
        cmkassets.modify(itr, ds_account(0), [&](auto &row) {
            row.asset_balance += asset_balance;
            row.unallocated_balance += unallocated_balance;
        });
        return *itr;
    }

    void cmkryvesting::deposit(const ds_account &asset_contract, const ds_asset &quantity) {
        PRINT_STARTED("deposit"_n)
        require_auth(_self);
        auto asset = assets_move_balance(quantity, quantity);
        ds_assert(asset.asset_contract == asset_contract,
                  "Wrong asset contract(%) expected: '%'.", asset.asset_contract, asset_contract);
        ds_print("\r\nReceived deposit(%) by contract(%)", quantity, asset_contract);
        PRINT_FINISHED("deposit"_n)
    }

    void cmkryvesting::withdraw(const ds_account &to, const ds_asset &quantity) {
        PRINT_STARTED("withdraw"_n)
        require_auth(_self);
        ds_assert(is_account(to), "% '%' % %.", "To",
                  to, DOES_NOT_EXIST, AS_AN_ACCOUNT);
        auto asset = assets_move_balance(-quantity, -quantity);
        trans_by_ctract(asset.asset_contract, to, quantity, "withdraw from cmkryvesting");
        PRINT_FINISHED("withdraw"_n)
    }

    auto cmkryvesting::assets_get(const ds_symbol &symbol) {
        cmkassets_table cmkassets(_self, _self.value);
        auto itr = cmkassets.find(symbol.code().raw());
        ds_assert(itr != cmkassets.end(),
                  "Symbol(%) does not exist in cmkassets.", symbol);
        return *itr;
    }

}
