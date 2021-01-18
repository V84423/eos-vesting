#include "cmkryvesting.hpp"

namespace eosdt {

    void cmkryvesting::on_transfer(const ds_account &from, const ds_account &to,
                                   const ds_asset &quantity, const ds_string &memo) {
        PRINT_STARTED("trasfer"_n)
        if (to == _self) {
            if (memo == "deposit") {
                deposit(get_first_receiver(), quantity);
            } else if (memo == "init") {
                ds_print("init trasfer: %", quantity);
            } else {
                ds_assert(false, "Expected memo: deposit or init.");
            }
        } else if (from == _self) {
            ds_print("\r\nIf you will withdraw more tokens than on vesting_balance contract will stop working.");
        }
        PRINT_FINISHED("trasfer"_n)
    }

}
