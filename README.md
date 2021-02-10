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

### Add/delete active permissions
The most complete manual i've found about add/delete/change permissions https://medium.com/leclevietnam/understanding-eos-permission-ee60dcfec8ad

### Create vesting
```cleos -u https://ore-staging.openrights.exchange push action comakeryteam addvesting '{"account_id": "<vesting_account>","vesting_period": 1, "vesting_amount":"48000.00","cliff_date":"2020-06-01","cliff_weight":"0.25","start_date":"2019-06-01","end_date":"2023-06-01"}' -p <vesting_account>@<permission>```

### Execute claim
```cleos -u https://ore-staging.openrights.exchange push action comakeryteam claim '{"vesting_id": <vesting_id>}' -p <vesting_account>@<permission>```
