/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <cstdlib>
#include <iostream>
#include <boost/test/included/unit_test.hpp>
#include <fc/log/logger.hpp>
#include <eosio/chain/exceptions.hpp>


void translate_fc_exception(const fc::exception &e) {
    std::cerr << "\033[33m" << e.to_detail_string() << "\033[0m" << std::endl;
//    BOOST_TEST_FAIL("Caught Unexpected Exception");
}

boost::unit_test::test_suite *init_unit_test_suite(int argc, char *argv[]) {

    fc::logger::get(DEFAULT_LOGGER).set_log_level(fc::log_level::off);

    boost::unit_test::unit_test_monitor.register_exception_translator<fc::exception>(&translate_fc_exception);

    return nullptr;
}
