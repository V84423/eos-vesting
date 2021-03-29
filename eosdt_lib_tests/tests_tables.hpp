#ifndef TESTS_TABLES_HPP
#define TESTS_TABLES_HPP

#include "tests_types.hpp"


FC_REFLECT(sysproducer, (owner)(total_votes)(producer_key)(is_active)(url)(unpaid_blocks)(last_claim_time)(location))

FC_REFLECT(sysstat, (supply)(max_supply)(issuer));

FC_REFLECT(tokissuers, (issuer)(max_supply)(supply)(is_active));

FC_REFLECT(orasettings,
           (id)(rate_timeout)(query_timeout)(provablecb1a_interval)(delphioracle_interval)(equilibriumdsp_interval)(backend_interval))
FC_REFLECT(orarate, (id)(rate)(base)(update)(provablecb1a_price)(provablecb1a_update)(delphioracle_price)(delphioracle_update)
(equilibriumdsp_price)(equilibriumdsp_update)(backend_price)(backend_update))
FC_REFLECT(oraqueries,
           (asset_symbol)(query)(price_type)(query_updated_at)(query_executed_at)(checksumm)(id)(source_contract)(base)(
                   filter));
FC_REFLECT(datapoint, (id)(owner)(value)(median)(timestamp))
FC_REFLECT(orasubscribe, (contract)(quantity)(callcount)(lastcall)(withdrawal_date)(id)(asset_symbol)(base))

FC_REFLECT(ctrsetting, (setting_id)
        (global_lock)(time_shift)
        (liquidator_account)(oraclize_account)(sttoken_account)(nutoken_account)
        (governance_fee)(stability_fee)(critical_ltv)
        (liquidation_penalty)(liquidator_discount)(liquidation_price)(nut_auct_ratio)(nut_discount)(profit_factor)
        (vote_period)(stake_period)(reserve_ratio)(staking_weight)(bpproxy_account)(governc_account)(referral_ratio)
        (savings_account))
FC_REFLECT(ctrparameter, (parameter_id)(total_collateral)(total_debt)
        (stability_rate)(governance_rate)(prev_date)(prev_vote)(prev_stake)(eos_staked));
FC_REFLECT(posparameter, (parameter_id)(total_collateral)(total_debt)(stability_rate)(prev_date));
FC_REFLECT(ctrposition, (position_id)(maker)(outstanding)(governance)(collateral));
FC_REFLECT(posposition, (position_id)(maker)(outstanding)(collateral));
FC_REFLECT(ctrltvratio, (position_id)(ltv_ratio));
FC_REFLECT(ctrreferral, (referral_id)(referral)(staked_amount));
FC_REFLECT(ctrpositionref, (position_id)(referral_id));


FC_REFLECT(liqsetting, (setting_id)(eosdtcntract_account)(global_unlock)(auction_price)
        (burn_rate)(gov_return_rate)(set_aside_rate)(savings_rate))

FC_REFLECT(liqparameter, (parameter_id)
        (surplus_debt)(bad_debt)(eos_balance)(nut_collat_balance));

FC_REFLECT(govsetting, (setting_id)(eosdtcntract_account)
        (min_proposal_weight)(freeze_period)(min_participation)(success_margin)(top_holders_amount)
        (max_bp_count)(max_bp_votes)(min_vote_stake)(unstake_period));
FC_REFLECT(govproposal, (proposal_name)(proposer)(title)(proposal_json)(created_at)(expires_at)(proposal_type))
FC_REFLECT(govvoter, (voter)(voting_amount)(withdrawal_date))
FC_REFLECT(govvote, (id)(proposal_name)(voter)(vote)(updated_at)(vote_json))
FC_REFLECT(govbpvote, (producer)(votes));
FC_REFLECT(govparam, (param_id)(NUT_voting_balance));
FC_REFLECT(govbpparam, (bp_name)(reward_amount)(balance)(enabled)(is_active)(active_since));


FC_REFLECT(respositions, (position_id)(depositor)(eos_balance)(withdrawal_date));

FC_REFLECT(locsetting, (setting_id)(eosdtcntract_account)(beneficiary)(nut_reward)(claim_amounts)(claim_dates));


FC_REFLECT(defiideavote, (idea_id)(votes));
FC_REFLECT(defiidea, (idea_id)(idea_name));
FC_REFLECT(defiideavoter, (voter)(voting_amount));

FC_REFLECT(xchpair, (pair_id)(base_currency)(quote_currency)(total_base_balance)(total_quote_balance)(buy_slippage)(
        sell_slippage)(price_currency)(price_type)(manager_account)(price))
FC_REFLECT(xchtoken, (token_symbol)(token_account)(min_trade))
FC_REFLECT(tonorder,
           (order_id)
                   (primary_chain_id)
                   (primary_trx_id)
                   (primary_address)
                   (side_chain_id)
                   (side_trx_id)
                   (side_address)
                   (quantity)
                   (created_at)
                   (order_status)
                   (confirm_mask)
                   (cancel_mask)
);
FC_REFLECT(tonparam,
           (param_id)
                   (ton_gram_balance)
                   (total_fees)
);

FC_REFLECT(savsetting, (setting_id)(sttoken_account)(min_deposit));
FC_REFLECT(savparam, (param_id)(total_discounted_balance));
FC_REFLECT(savposition, (position_id)(owner)(balance));

FC_REFLECT(swpsetting, (setting_id)(min_deposit)(token_address));
FC_REFLECT(swpparam, (param_id)(total_nut_amount));
FC_REFLECT(swpposition, (position_id)(moment)(nut_amount)(eth_address));

FC_REFLECT(betsetting,
           (setting_id)(manager_account)(withdrawal_fee)(reveal_timer)(manager_reveal_timer)(max_draw_timer));
FC_REFLECT(betdraw, (draw_id)(draw_type)(asset_contract)(ticket_cost)(created)(draw_end_time)(reveal_start_time)
        (reveal_stop_time)(draw_status)(jackpot)(winner_weights));
FC_REFLECT(betposition, (position_id)(account_id)(draw_id)(win_amount));
FC_REFLECT(betsecret, (secret_id)(position_id)(draw_id)(seed)(secret));

FC_REFLECT(cmkasset, (asset_symbol)(asset_contract)(asset_balance)(unallocated_balance));
FC_REFLECT(cmkvesting, (vesting_id)(account_id)(vesting_period)(vesting_amount)
        (cliff_date)(cliff_weight)(start_date)(end_date)(lock_date)(pause_start)(pause_time)
        (status)(vested_amount));

#endif