#include "cmkryvesting.hpp"

namespace eosdt {

    void cmkryvesting::assert_dates(const ds_uint &vesting_period, const ds_time &cliff,
                                    const ds_time &start, const ds_time end) {
        auto now = time_get();
        auto period = vesting_period_name(vesting_period);
        //ds_assert(start >= now, "Wrong start(%) expected more than now(%), vesting_period: %.", start, now, period);
        ds_assert(end >= now, "Wrong end(%) expected more than now(%), vesting_period: %.", end, now, period);
        //ds_assert(cliff >= now, "Wrong cliff(%) expected more than now(%), vesting_period: %.", cliff, now, period);
        ds_assert(end > start, "Wrong end(%) expected more than start(%), vesting_period: %.", end, start, period);
        ds_assert(cliff >= start, "Wrong cliff(%) expected more or equal than start(%), vesting_period: %.",
                  cliff, start, period);
        ds_assert(cliff <= end, "Wrong cliff(%) expected less than end(%), vesting_period: %.",
                  cliff, end, period);
        if (vesting_period == 1 || vesting_period == 2 || vesting_period == 3 || vesting_period == 6) {
            ds_assert(start.sec_since_epoch() % 86400 == 0,
                      "Wrong time in start(%) expected '00:00:00', vesting_period: %.", start, period);
            ds_assert(end.sec_since_epoch() % 86400 == 0,
                      "Wrong time in end(%) expected '00:00:00', vesting_period: %.", end, period);
            ds_assert(cliff.sec_since_epoch() % 86400 == 0,
                      "Wrong time in cliff(%) expected '00:00:00', vesting_period: %.", cliff, period);
        }
        auto stm = tm_get(start);
        auto etm = tm_get(end);
        //ds_assert(vesting_period != 1 || stm.tm_mday == 1,
        //          "Wrong start(%)[%] expected first day of month, vesting_period: %.", start, stm.tm_mday, period);
        //ds_assert(vesting_period != 1 || etm.tm_mday == 1,
        //          "Wrong end(%)[%] expected first day of month, vesting_period: %.", end, etm.tm_mday, period);
        //ds_assert(vesting_period != 2 || stm.tm_wday == 1,
        //          "Wrong start(%)[%] expected monday[1], vesting_period: %.", start, stm.tm_wday, period);
        //ds_assert(vesting_period != 2 || etm.tm_wday == 1,
        //          "Wrong end(%)[%] expected monday[1], vesting_period: %.", end, etm.tm_wday, period);
        ds_assert(vesting_period != 1 || stm.tm_mday == etm.tm_mday,
                  "Different day for start(%)[%] and end(%)[%], vesting_period: %.",
                  start, stm.tm_mday, end, etm.tm_mday, period);
        ds_assert(vesting_period != 2 || stm.tm_wday == etm.tm_wday,
                  "Different day of week for start(%)[%] and end(%)[%], vesting_period: %.",
                  start, stm.tm_wday, end, etm.tm_wday, period);
    }

    void cmkryvesting::addvesting(const ds_account &account_id,
                                  const ds_uint &vesting_period, const ds_asset &vesting_amount,
                                  ds_time &cliff_date, const double cliff_weight,
                                  const ds_time &start_date, const ds_time end_date) {
        PRINT_STARTED("addvesting"_n)
        require_auth(_self);
        ds_assert(is_account(account_id), "% '%' % %.", "to",
                  account_id, DOES_NOT_EXIST, AS_AN_ACCOUNT);
        ds_assert(1 <= vesting_period && vesting_period <= 6,
                  "Wrong vesting period(%) for '%' expected one from {1,2,3,4,5,6}",
                  vesting_period, account_id);
        ds_assert(vesting_amount.amount > 0, "Wrong vesting amount(%) for '%' expected more than zero.",
                  vesting_amount, account_id);
        ds_assert(0.0 <= cliff_weight && cliff_weight <= 1.0,
                  "Wrong cliff weight(%) for '%' expected more than zero and less or equal one",
                  cliff_weight, account_id);
        cliff_date = (cliff_weight == 0.0) ? start_date : cliff_date;
        assert_dates(vesting_period, cliff_date, start_date, end_date);
        auto asset = assets_move_balance(ds_asset(0, vesting_amount.symbol), -vesting_amount);
        cmkvestings_table cmkvestings(_self, _self.value);
        auto vesting_id = cmkvestings.available_primary_key();
        cmkvestings.emplace(_self, [&](auto &row) {
            row.vesting_id = vesting_id;
            row.account_id = account_id;
            row.vesting_period = vesting_period;
            row.vesting_amount = vesting_amount;
            row.cliff_date = cliff_date;
            row.cliff_weight = cliff_weight;
            row.start_date = start_date;
            row.end_date = end_date;
            row.status = 1;//active
            row.vested_amount = ds_asset(0, vesting_amount.symbol);
        });
        auto[total, elapsed] = calc_vesting_percent(vesting_period, cliff_date, end_date, time_get());
        ds_print("\r\nCreated vesting(%) for '%' [%-%] vesting_period: %, total: %, elapsed: %",
                 vesting_id, account_id, cliff_date, end_date, vesting_period, total, elapsed);
        PRINT_FINISHED("addvesting"_n)
    }

    void cmkryvesting::removevesting(const ds_ulong &vesting_id) {
        PRINT_STARTED("removevesting"_n)
        require_auth(_self);
        cmkvestings_table cmkvestings(_self, _self.value);
        auto itr = cmkvestings.find(vesting_id);
        ds_assert(itr != cmkvestings.end(),
                  "Vesting(%) does not exist in cmkvestings.", vesting_id);
        ds_assert(itr->vesting_amount == itr->vested_amount,
                  "Vesting(%) has different vesting_amount(%) and vested_amount(%).",
                  itr->vesting_amount, itr->vested_amount);
        auto next_vesting_id = cmkvestings.available_primary_key();
        ds_assert(itr->vesting_id + 1 < next_vesting_id, "This record(%) for account(%) is last in the table.",
                  itr->vesting_id, itr->account_id);
        cmkvestings.erase(itr);
        PRINT_FINISHED("removevesting"_n)
    }

}