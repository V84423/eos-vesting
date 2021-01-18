#include "cmkryvesting.hpp"

namespace eosdt {

    void cmkryvesting::currentver() {
        ds_assert(false, GIT_VERSION);
    }

    void cmkryvesting::getcurrent() {
        ds_print("sec=%\tver=%", time_get(), GIT_VERSION);
    }

}