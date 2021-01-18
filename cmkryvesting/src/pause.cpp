#include "cmkryvesting.hpp"

namespace eosdt {

    void cmkryvesting::pausevesting(const ds_ulong &vesting_id, const ds_time &pause_start) {
        PRINT_STARTED("pausevesting"_n)
        require_auth(_self);
        auto now = time_get();
        ds_assert(pause_start >= now, "Wrong pause_start(%) expected more or equal now(%).",
                  pause_start, now);
        cmkvestings_table cmkvestings(_self, _self.value);
        auto itr = cmkvestings.find(vesting_id);
        ds_assert(itr != cmkvestings.end(), "Vesting(%) does not exist in cmkvestings.", vesting_id);
        ds_assert(itr->status == 1, "Wrong vesting status(%) for '%' expected: '1'(Active).",
                  itr->status, itr->account_id);
        cmkvestings.modify(itr, ds_account(0), [&](auto &row) {
            row.status = 2;//pause
            row.pause_start = pause_start;
        });
        ds_print("\r\nPause vesting(%) for account_id: %, pause_start: %, now: %.",
                 vesting_id, itr->account_id, pause_start, now);
        PRINT_FINISHED("pausevesting"_n)
    }

    void cmkryvesting::unpausevestng(const ds_ulong &vesting_id, const ds_time &pause_end) {
        PRINT_STARTED("unpausevestng"_n)
        require_auth(_self);
        auto now = time_get();
        ds_assert(pause_end >= now, "Wrong pause_end(%) expected more or equal now(%).", pause_end, now);
        cmkvestings_table cmkvestings(_self, _self.value);
        auto itr = cmkvestings.find(vesting_id);
        ds_assert(itr != cmkvestings.end(), "Vesting(%) does not exist in cmkvestings.", vesting_id);
        ds_assert(itr->status == 2, "Wrong vesting status(%) for '%' expected: '2'(Pause).",
                  itr->status, itr->account_id);
        auto pause_time = (itr->pause_start < pause_end) ? (pause_end - itr->pause_start).to_seconds() : 0UL;
        cmkvestings.modify(itr, ds_account(0), [&](auto &row) {
            row.status = 1;//Active
            row.pause_time += pause_time;
        });
        ds_print("\r\nUnPause vesting(%) for account_id: %, pause_start: %, pause_end: %, pause_sec: %, pause_days: %.",
                 vesting_id, itr->account_id, itr->pause_start, pause_end, pause_time, pause_time / 86400UL);
        PRINT_FINISHED("unpausevestng"_n)
    }

    void cmkryvesting::setpausetime(const ds_ulong &vesting_id, const ds_ulong &pause_time) {
        PRINT_STARTED("setpausetime"_n)
        require_auth(_self);
        ds_assert(pause_time > 0, "Wrong pause_tima(%) expected more than zero.", pause_time);
        cmkvestings_table cmkvestings(_self, _self.value);
        auto itr = cmkvestings.find(vesting_id);
        ds_assert(itr != cmkvestings.end(), "Vesting(%) does not exist in cmkvestings.", vesting_id);
        cmkvestings.modify(itr, ds_account(0), [&](auto &row) {
            row.pause_time = pause_time;
        });
        ds_print("\r\nSet pause time vesting(%) for account_id: %, pause_time: %.",
                 vesting_id, itr->account_id, (ds_ulong) pause_time);
        PRINT_FINISHED("setpausetime"_n)
    }

    auto
    cmkryvesting::now_fixed_by_pause(const ds_ulong &pause_time, const ds_uint status, const ds_time &pause_start) {
        auto now = time_get();
        auto pt = pause_time;
        if (status == 2 && pause_start < now) //pause
        {
            pt += (now - pause_start).to_seconds();
        }
        if (pt > 0) {
            ds_print("\r\nNow(%) reduced by pause(%) so now(%).", now, pt, now - pt);
            now -= pt;
        }
        return now;
    }
}