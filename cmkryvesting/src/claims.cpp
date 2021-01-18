#include "cmkryvesting.hpp"

namespace eosdt {

    void cmkryvesting::claim(const ds_ulong &vesting_id) {
        PRINT_STARTED("claim"_n)
        cmkvestings_table cmkvestings(_self, _self.value);
        auto itr = cmkvestings.find(vesting_id);
        ds_assert(itr != cmkvestings.end(),
                  "Vesting(%) does not exist in cmkvestings.", vesting_id);
        require_auth(itr->account_id);
        //ds_assert(itr->status == 1,
        //        "Wrong vesting status(%) expected: '1'(Active).", itr->status);
        auto now = now_fixed_by_pause(itr->pause_time, itr->status, itr->pause_start);
        auto[total, elapsed] = calc_vesting_percent(itr->vesting_period, itr->cliff_date, itr->end_date, now);
        auto vesting = op_mul_div(itr->vesting_amount, elapsed * (1.0L - itr->cliff_weight), total);
        ds_print("\r\nVesting(%)[%] = vesting_amount(%)*elapsed(%)*(1-cliff_weight(%))/total(%)",
                 vesting, itr->account_id, itr->vesting_amount, elapsed, itr->cliff_weight, total);
        if (itr->cliff_date < now && itr->cliff_weight > 0.0) {
            auto cliff = op_mul_div(itr->vesting_amount, (long double) itr->cliff_weight, 1.0L);
            ds_print("\r\nVesting(%)[%] Add cliff(%) cliff_date: '%', now: '%'.",
                     vesting, itr->account_id, cliff, itr->cliff_date, now);
            vesting += cliff;
        }
        if (vesting > itr->vesting_amount || (total == elapsed && itr->cliff_date < now)) {
            vesting = itr->vesting_amount;
        }
        ds_assert(itr->vested_amount < vesting,
                  "Account: % does not have quantity for claim, vesting_amount: %, vested_amount: %, vesting: %",
                  itr->account_id, itr->vesting_amount, itr->vested_amount, vesting);
        auto quantity = vesting - itr->vested_amount;
        ds_print("\r\nQuantity(%) = Vesting(%) - vested_amount(%)", quantity, vesting, itr->vested_amount);
        trans_by_ctract(assets_get(quantity.symbol).asset_contract, itr->account_id, quantity, "claim");
        assets_move_balance(-quantity, ds_asset(0, quantity.symbol));
        cmkvestings.modify(itr, ds_account(0), [&](auto &row) {
            row.vested_amount += quantity;
            row.status = (row.vesting_amount == row.vested_amount) ? 3 : row.status;//cancel
        });
        PRINT_FINISHED("claim"_n)
    }

}
