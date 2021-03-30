#ifndef TESTS_DEFINES_HPP
#define TESTS_DEFINES_HPP

#define ds_int int
#define ds_uint unsigned int
#define ds_ulong unsigned long long
#define ds_long long long
#define ds_llong __int128_t
#define ds_string std::string
#define ds_nullptr std::nullptr_t

#include <eosio/testing/tester.hpp>
#include <eosio/chain/resource_limits.hpp>
#define ds_time eosio::chain::time_point_sec
#define ds_account eosio::chain::name
#define ds_asset eosio::chain::asset
#define ds_symbol eosio::chain::symbol
#define ds_checksum eosio::chain::checksum256_type
#define ds_public_key eosio::chain::public_key_type
#define ds_string std::string
#define ds_chain eosio::testing::validating_tester
#define ds_tran_trace eosio::testing::transaction_trace_ptr

#define EOSCTRACT N(eosio.token)
#define EOSDTCNTRACT N(eosdtcntract)
#define EOSDTORCLIZE N(eosdtorclize)
#define ORACLERATES N(orarates)
#define EOSDTLIQDATR N(eosdtliqdatr)
#define EOSDTNUTOKEN N(eosdtnutoken)
#define EOSDTSTTOKEN N(eosdtsttoken)
#define EOSDTBPPROXY N(eosdtbpproxy)
#define EOSDTGOVERNC N(eosdtgovernc)
#define EOSDTRESERVE N(eosdtreserve)
#define EOSDTEXCHANG N(eosdtexchang)
#define EOSDAQORDERS N(eosdaqorders)
#define EOSDTLOCKUPP N(eosdtlockupp)
#define EOSDEFIVOTES N(eosdefivotes)
#define EOS2DTDOTCOM N(eos2dtdotcom)
#define NUTTOKENSWAP EOS2DTDOTCOM
#define FAKEELIQDATR N(fakeeliqdatr)
#define EQUIGUARDIAN N(equiguardian)
#define EOSDTARMSEOS N(eosdtarmseos)
#define EOSDTKGTOKEN N(eosdtkgtoken)
#define WRAPPEDGRAM EOSDTKGTOKEN
#define EOSDTVSKGRAM N(eosdtvskgram)
#define KGRAMLIQDATR N(kgramliqdatr)
#define DELPHIORACLE N(delphioracle)
#define PROVABLECONN N(provableconn)
#define DAPPSERVICES N(dappservices)
#define EOSDTONTOKEN N(eosdtontoken)
#define EOSDTSAVINGS N(eosdtsavings)
#define FAKEENNEWDEX N(fakeennewdex)
#define FAKEELLOANTP N(fakeelloantp)
#define TOKENSWAP_EQ N(tokenswap.eq)
#define EOSIO_EVM N(eosio.evm)
#define PRICEFEED_EQ N(pricefeed.eq)
#define TETHERTETHER N(tethertether)
#define EOSLOTTERYEQ N(eoslotteryeq)
#define CMKRYVESTING N(cmkryvesting)

#define EOS_SYMBOL_STR "EOS"
#define EOS_SYMBOL_DECIMAL 4
#define REX_SYMBOL_STR "REX"
#define REX_SYMBOL_DECIMAL 4
#define USD_SYMBOL_STR "USD"
#define USD_SYMBOL_DECIMAL 4
#define CNY_SYMBOL_STR "CNY"
#define CNY_SYMBOL_DECIMAL 4
#define STABLE_SYMBOL_STR "EOSDT"
#define STABLE_SYMBOL_PAD_STR " " STABLE_SYMBOL_STR
#define STABLE_SYMBOL_DECIMAL 9
#define EOSDT_SYMBOL_STR "EOSDT"
#define EOSDT_SYMBOL_PAD_STR " " EOSDT_SYMBOL_STR
#define EOSDT_SYMBOL_DECIMAL 9
#define UTILITY_SYMBOL_STR "NUT"
#define UTILITY_SYMBOL_PAD_STR " " UTILITY_SYMBOL_STR
#define UTILITY_SYMBOL_DECIMAL 9
#define NUT_SYMBOL_STR "NUT"
#define NUT_SYMBOL_PAD_STR " " NUT_SYMBOL_STR
#define NUT_SYMBOL_DECIMAL 9
#define DAPP_SYMBOL_STR "DAPP"
#define DAPP_SYMBOL_DECIMAL 4
#define KG_SYMBOL_STR "KGRAM"
#define KG_SYMBOL_DECIMAL 8
#define USDT_SYMBOL_STR "USDT"
#define USDT_SYMBOL_DECIMAL 4
#define ORE_SYMBOL_STR "ORE"
#define ORE_SYMBOL_DECIMAL 4

#define EOS_SYMBOL ds_symbol(EOS_SYMBOL_DECIMAL,EOS_SYMBOL_STR)
#define EOS_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(EOS_SYMBOL_DECIMAL,EOS_SYMBOL_STR)
#define USD_SYMBOL ds_symbol(USD_SYMBOL_DECIMAL,USD_SYMBOL_STR)
#define USD_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(USD_SYMBOL_DECIMAL,USD_SYMBOL_STR)
#define CNY_SYMBOL ds_symbol(CNY_SYMBOL_DECIMAL,CNY_SYMBOL_STR)
#define CNY_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(CNY_SYMBOL_DECIMAL,CNY_SYMBOL_STR)
#define STABLE_SYMBOL ds_symbol(STABLE_SYMBOL_DECIMAL,STABLE_SYMBOL_STR)
#define STABLE_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(STABLE_SYMBOL_DECIMAL,STABLE_SYMBOL_STR)
#define EOSDT_SYMBOL ds_symbol(EOSDT_SYMBOL_DECIMAL,EOSDT_SYMBOL_STR)
#define EOSDT_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(EOSDT_SYMBOL_DECIMAL,EOSDT_SYMBOL_STR)
#define UTILITY_SYMBOL ds_symbol(UTILITY_SYMBOL_DECIMAL,UTILITY_SYMBOL_STR)
#define UTILITY_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(UTILITY_SYMBOL_DECIMAL,UTILITY_SYMBOL_STR)
#define NUT_SYMBOL ds_symbol(NUT_SYMBOL_DECIMAL,NUT_SYMBOL_STR)
#define NUT_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(NUT_SYMBOL_DECIMAL,NUT_SYMBOL_STR)
#define REX_SYMBOL ds_symbol(REX_SYMBOL_DECIMAL,REX_SYMBOL_STR)
#define REX_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(REX_SYMBOL_DECIMAL,REX_SYMBOL_STR)
#define DAPP_SYMBOL ds_symbol(DAPP_SYMBOL_DECIMAL,DAPP_SYMBOL_STR)
#define DAPP_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(DAPP_SYMBOL_DECIMAL,DAPP_SYMBOL_STR)
#define KG_SYMBOL ds_symbol(KG_SYMBOL_DECIMAL,KG_SYMBOL_STR)
#define KG_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(KG_SYMBOL_DECIMAL,KG_SYMBOL_STR)
#define USDT_SYMBOL ds_symbol(USDT_SYMBOL_DECIMAL,USDT_SYMBOL_STR)
#define USDT_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(USDT_SYMBOL_DECIMAL,USDT_SYMBOL_STR)
#define ORE_SYMBOL ds_symbol(ORE_SYMBOL_DECIMAL,ORE_SYMBOL_STR)
#define ORE_SYMBOL_VALUE ::eosio::chain::string_to_symbol_c(ORE_SYMBOL_DECIMAL,ORE_SYMBOL_STR)

#define EOSCTRACT_STR "eosio.token"
#define EOSDTCNTRACT_STR "eosdtcntract"
#define EOSDTORCLIZE_STR "eosdtorclize"
#define ORACLERATES_STR "orarates"
#define EOSDTLIQDATR_STR "eosdtliqdatr"
#define EOSDTNUTOKEN_STR "eosdtnutoken"
#define EOSDTSTTOKEN_STR "eosdtsttoken"
#define EOSDTBPPROXY_STR "eosdtbpproxy"
#define EOSDTGOVERNC_STR "eosdtgovernc"
#define EOSDTRESERVE_STR "eosdtreserve"
#define EOSDTEXCHANG_STR "eosdtexchang"
#define EOS2DTDOTCOM_STR "eos2dtdotcom"
#define NUTTOKENSWAP_STR EOS2DTDOTCOM_STR
#define EQUIGUARDIAN_STR "equiguardian"
#define EOSDTARMSEOS_STR "eosdtarmseos"
#define EOSDTKGTOKEN_STR "eosdtkgtoken"
#define EOSDTVSKGRAM_STR "eosdtvskgram"
#define KGRAMLIQDATR_STR "kgramliqdatr"
#define DELPHIORACLE_STR "delphioracle"
#define EOSDTONTOKEN_STR "eosdtontoken"
#define EOSDTSAVINGS_STR "eosdtsavings"
#define FAKEENNEWDEX_STR "fakeennewdex"
#define FAKEELLOANTP_STR "fakeelloantp"
#define CMKRYVESTING_STR "cmkryvesting"


#endif
