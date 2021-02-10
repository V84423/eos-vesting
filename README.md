### Build and deploy contract cmkryvesting
For build EOS contracts you need download eosio-cpp 1.7.0 and EOS binaries https://developers.eos.io/manuals/eos/latest/install/install-prebuilt-binaries

Build command

```eosio-cpp -I=cmkryvesting/include cmkryvesting/src/cmkryvesting.cpp -o cmkryvesting/cmkryvesting.wasm```

After build the wasm you need to create the local wallet to deploy the contract

```cleos  wallet create --to-console```

Import your account's private key for active permissions

```cleos wallet import --private-key <PRIVATE_KEY>```

Deploy your contract

```cleos -u https://ore-staging.openrights.exchange set contract comakeryteam cmkryvesting cmkryvesting.wasm cmkryvesting.abi -p comakeryteam@active ```

Add the asset

```cleos -u https://ore-staging.openrights.exchange push action comakeryteam addaset '{"asset_contract":"eosio.token","asset_symbol":"4,ORE"}' -p comakeryteam@active```

### Add eosio.code active permissions

```cleos set account permission comakeryteam active comakeryteam --add-code```

### Add vesting

```cleos -u https://ore-staging.openrights.exchange push action comakeryteam addvesting '{"account_id": "<vesting_account>","vesting_period": 1, "vesting_amount":"48000.0000 ORE","cliff_date":"2020-06-01T00:00:00","cliff_weight":"0.25","start_date":"2019-06-01T00:00:00","end_date":"2023-06-01T00:00:00"}'  -p comakeryteam@active```

### Execute claim

```cleos -u https://ore-staging.openrights.exchange push action comakeryteam claim '{"vesting_id": 0}' -p <vesting_account>@<permission>```


