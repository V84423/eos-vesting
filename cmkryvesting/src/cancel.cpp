#include "cmkryvesting.hpp"

namespace eosdt {

    void cmkryvesting::cancelvesting(const ds_ulong &vesting_id, const ds_time &cancel_start) {
        PRINT_STARTED("cancelvestng"_n)
        require_auth(_self);
        cmkvestings_table cmkvestings(_self, _self.value);
        auto itr = cmkvestings.find(vesting_id);
        ds_assert(itr != cmkvestings.end(),
                  "Vesting(%) does not exist in cmkvestings.", vesting_id);
        auto now = now_fixed_by_pause(itr->pause_time, itr->status, itr->pause_start);
        ds_assert(cancel_start >= now, "Wrong cancel_start(%) expected more or equal now(%)",
                  cancel_start, now);
        ds_assert(itr->status == 1 || itr->status == 2,
                  "Wrong vesting status(%) for '%'  expected: '1'(Active) or '2'(Pause).",
                  itr->status, itr->account_id);
        ds_assert(cancel_start < itr->end_date, "Wrong cancel_start(%) expected less than end_date(%)",
                  cancel_start, itr->end_date);
        auto[total, elapsed] = calc_vesting_percent(itr->vesting_period, itr->cliff_date, itr->end_date, cancel_start);
        auto vesting = op_mul_div(itr->vesting_amount, elapsed * (1.0L - itr->cliff_weight), total);
        ds_print("\r\nVesting(%)[%] = vesting_amount(%)*elapsed(%)*(1-cliff_weight(%))/total(%)",
                 vesting, itr->account_id, itr->vesting_amount, elapsed, itr->cliff_weight, total);
        if (itr->cliff_date < now && itr->cliff_weight > 0.0) {
            auto cliff = op_mul_div(itr->vesting_amount, (long double) itr->cliff_weight, 1.0L);
            ds_print("\r\nVesting(%)[%] Add cliff(%) cliff_date: '%', now: '%'.",
                     vesting, itr->account_id, cliff, itr->cliff_date, now);
            vesting += cliff;
        }
        ds_print("\r\nCancel vesting(%)  for account_id: %, vesting_amount: %, vested_amount: %, vesting: %.",
                 vesting_id, itr->account_id, itr->vesting_amount, itr->vested_amount, vesting);
        if (vesting > itr->vesting_amount || (total == elapsed && itr->cliff_date < now)) {
            vesting = itr->vesting_amount;
        } else if (vesting < itr->vesting_amount) {
            assets_move_balance(ds_asset(0, itr->vesting_amount.symbol), itr->vesting_amount - vesting);
        }
        cmkvestings.modify(itr, ds_account(0), [&](auto &row) {
            row.status = 3;//cancel
            row.vesting_amount = vesting;
            row.end_date = cancel_start;
        });
        PRINT_FINISHED("cancelvestng"_n)
    }

}
