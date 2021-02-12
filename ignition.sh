#!/usr/bin/env bash

system_contracts=${system_contracts:-system_contracts}
bioshost=${bioshost:-localhost}
biosport=${biosport:-8888}
wdaddr=${wdaddr:-localhost:9000}
wdurl=${wdurl:-http://$wdaddr}
wddir=${wddir:-local-wd}
wdname=${wdname:-local_wallet}
pubsyskey=${pubsyskey:-EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV}
prisyskey=${prisyskey:-5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3}
step=1
pkill -15 keosd
rm -rf $wddir
mkdir $wddir
echo Initializing ignition sequence  at $(date)
echo "http-server-address = $wdaddr" > $wddir/config.ini
keosd --config-dir $wddir --data-dir $wddir 2> $wddir/wdlog.txt &
sleep 1

ecmd () {
    echo ===== Start: $step ============
    echo executing: cleos --wallet-url $wdurl --url http://$bioshost:$biosport $*
    echo -----------------------
    cleos  --wallet-url $wdurl --url http://$bioshost:$biosport $* || exit 1
    echo ==== End: $step ==============
    sleep 2;
    step=$(($step + 1))
}

wcmd () {
    ecmd wallet $* || exit 1
}

wcmd create --to-console -n ignition
echo eosio.* keys: $prisyskey $pubsyskey
wcmd import -n ignition --private-key $prisyskey
ecmd create account eosio eosio.bpay $pubsyskey $pubsyskey
ecmd create account eosio eosio.msig $pubsyskey $pubsyskey
ecmd create account eosio eosio.names $pubsyskey $pubsyskey
ecmd create account eosio eosio.ram $pubsyskey $pubsyskey
ecmd create account eosio eosio.ramfee $pubsyskey $pubsyskey
ecmd create account eosio eosio.saving $pubsyskey $pubsyskey
ecmd create account eosio eosio.stake $pubsyskey $pubsyskey
ecmd create account eosio eosio.token $pubsyskey $pubsyskey
ecmd create account eosio eosio.vpay $pubsyskey $pubsyskey
ecmd create account eosio eosio.wrap $pubsyskey $pubsyskey
ecmd create account eosio eosio.rex $pubsyskey $pubsyskey

ecmd set contract eosio.token ${system_contracts}/eosio.token eosio.token.wasm eosio.token.abi
ecmd set contract eosio.msig ${system_contracts}/eosio.msig eosio.msig.wasm eosio.msig.abi

echo ===== Start: $step ============
cleos --wallet-url $wdurl --url http://$bioshost:$biosport push action eosio.token create '["eosio", "10000000000.0000 EOS",0,0,0]' -p eosio.token || exit 1
cleos --wallet-url $wdurl --url http://$bioshost:$biosport push action eosio.token issue '["eosio","1000000000.0000 EOS", "issue"]' -p eosio || exit 1
until [[ $query -eq "500" ]]; do
    query=$(curl -s --request POST --url http://$bioshost:$biosport/v1/producer/schedule_protocol_feature_activations -d '{"protocol_features_to_activate": ["0ec7e080177b2c02b278d5088611686b49d739925a92d9bfcacd7fc6b74053bd"]}' | jq '.code')
   sleep 10;
done
echo ==== End: $step ==============
step=$(($step + 1))

ecmd set contract eosio ${system_contracts}/old.system  eosio.system.wasm eosio.system.abi
ecmd set contract eosio ${system_contracts}/old.bios  eosio.bios.wasm eosio.bios.abi

# FORWARD_SETCODE
ecmd push action eosio activate '["2652f5f96006294109b3dd0bbde63693f55324af452b799ee137a81a905eed25"]' -p eosio

# ONLY_BILL_FIRST_AUTHORIZER
ecmd push action eosio activate '["8ba52fe7a3956c5cd3a656a3174b931d3bb2abb45578befc59f283ecd816a405"]' -p eosio

# RESTRICT_ACTION_TO_SELF
ecmd push action eosio activate '["ad9e3d8f650687709fd68f4b90b41f7d825a365b02c23a636cef88ac2ac00c43"]' -p eosio

# DISALLOW_EMPTY_PRODUCER_SCHEDULE
ecmd push action eosio activate '["68dcaa34c0517d19666e6b33add67351d8c5f69e999ca1e37931bc410a297428"]' -p eosio

# FIX_LINKAUTH_RESTRICTION
ecmd push action eosio activate '["e0fb64b1085cc5538970158d05a009c24e276fb94e1a0bf6a528b48fbc4ff526"]' -p eosio

# REPLACE_DEFERRED
ecmd push action eosio activate '["ef43112c6543b88db2283a2e077278c315ae2c84719a8b25f25cc88565fbea99"]' -p eosio

# NO_DUPLICATE_DEFERRED_ID
ecmd push action eosio activate '["4a90c00d55454dc5b059055ca213579c6ea856967712a56017487886a4d4cc0f"]' -p eosio

# ONLY_LINK_TO_EXISTING_PERMISSION
ecmd push action eosio activate '["1a99a59d87e06e09ec5b028a9cbb7749b4a5ad8819004365d02dc4379a8b7241"]' -p eosio

# RAM_RESTRICTIONS
ecmd push action eosio activate '["4e7bf348da00a945489b2a681749eb56f5de00b900014e137ddae39f48f69d67"]' -p eosio

# WEBAUTHN_KEY
ecmd push action eosio activate '["4fca8bd82bbd181e714e283f83e1b45d95ca5af40fb89ad3977b653c448f78c2"]' -p eosio

# WTMSIG_BLOCK_SIGNATURES
ecmd push action eosio activate '["299dcb6af692324b899b39f16d5a530a33062804e41f09dc97e9f156b4476707"]' -p eosio

cleos --wallet-url $wdurl --url http://$bioshost:$biosport push action eosio setpriv '["eosio.msig", 1]' -p eosio@active || exit 1

ecmd set contract eosio ${system_contracts}/eosio.system eosio.system.wasm eosio.system.abi
ecmd set contract eosio ${system_contracts}/eosio.bios eosio.bios.wasm eosio.bios.abi
ecmd set contract eosio ${system_contracts}/eosio.system eosio.system.wasm eosio.system.abi

#cleos --wallet-url $wdurl --url http://$bioshost:$biosport push action eosio init '{"version": 0,"core": "EOS"}' -p eosio || exit 1

ecmd set contract eosio.wrap  ${system_contracts}/eosio.wrap eosio.wrap.wasm eosio.wrap.abi

pkill -15 keosd
