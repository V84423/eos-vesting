#include "cmkryvesting.hpp"

namespace eosdt {

    auto cmkryvesting::vesting_period_name(const ds_uint &vesting_period) {
        switch (vesting_period) {
            case 1:
                return "Monthly";
            case 2:
                return "Weekly";
            case 3:
                return "Daily";
            case 4:
                return "Secondly";
            case 5:
                return "Locked";
            case 6:
                return "30Daily";
        }
        ds_assert(false, "Unknown vesting_period: %", vesting_period);
        return (ds_string("Unknown vesting_period: ") + std::to_string(vesting_period)).c_str();
    }

    auto cmkryvesting::vesting_period_size(const ds_uint &vesting_period) {
        switch (vesting_period) {
            case 1:
                return 0;
            case 2:
                return 604800;
            case 3:
                return 86400;
            case 4:
                return 1;
            case 5:
                return 0;
            case 6:
                return 2592000;
        }
        ds_assert(false, "Unknown vesting_period: %", vesting_period);
        return 0;
    }

    auto cmkryvesting::calc_vesting_percent(const ds_uint &vesting_period,
                                            const ds_time &start, const ds_time &end, const ds_time &curr) {
        ds_assert(start < end, "Wrong start(%) or end(%) since start should be less than end",
                  start, end);
        auto size = vesting_period_size(vesting_period);
        auto total = 0LL;
        auto elapsed = 0LL;
        if (size != 0) {
            total = (end - start).to_seconds();
            elapsed = (curr - start).to_seconds();
            total += size - total % size;
            elapsed -= elapsed % size;
            total /= size;
            elapsed /= size;
        } else if (vesting_period == 5) {
            total = 1LL;
            elapsed = 0L;
        } else if (vesting_period == 1) {
            auto start_tm = tm_get(start);
            auto end_tm = tm_get(end);
            auto curr_tm = tm_get(curr);
            total = (end_tm.tm_year - start_tm.tm_year) * 12LL + (end_tm.tm_mon - start_tm.tm_mon);
            if (end_tm.tm_mday > start_tm.tm_mday)total++;
            elapsed = (curr_tm.tm_year - start_tm.tm_year) * 12LL + (curr_tm.tm_mon - start_tm.tm_mon);
            if (curr_tm.tm_mday < start_tm.tm_mday)elapsed--;
            if (curr_tm.tm_mday == start_tm.tm_mday && curr.utc_seconds % 86400 < start.utc_seconds % 86400) elapsed--;
        }
        elapsed = (elapsed < total && curr < end) ? elapsed : total;
        elapsed = (elapsed < 0) ? 0 : elapsed;
        ds_print("\r\nvesting_period: %, start: '%', end: '%', curr: '%', total: %, elapsed: %.",
                 vesting_period_name(vesting_period), start, end, curr, total, elapsed);
        return std::tuple{total, elapsed};
    }

}