## Build and deploy contract cmkryvesting
For build EOS contracts you need download eosio-cpp 1.7.0 and EOS binaries https://developers.eos.io/manuals/eos/latest/install/install-prebuilt-binaries

### To build contracts alone

Run the `build.sh` script in the top directory to build all the contracts.

```sh
./build.sh
```

All .wasm and .abi files will be located in the `build/cmkryvesting` directory

### To build the contracts and unit tests

1. Ensure an appropriate version of eosio has been built from source and installed. Installing eosio from binaries is not sufficient.
2. Run the build.sh script in the top directory with the -t flag to build all the contracts and the unit tests for these contracts.

```sh
./build.sh -t
```

Unit test will be located in the `build/cmkryvesting_tests` directory

After build the wasm you need to create the local wallet to deploy the contract

```cleos  wallet create --to-console```

Import your account's private key for active permissions

```cleos wallet import --private-key <PRIVATE_KEY>```

Deploy your contract

```cleos -u https://ore-staging.openrights.exchange set contract comakeryteam cmkryvesting cmkryvesting.wasm cmkryvesting.abi -p comakeryteam@active ```

Add the asset

```cleos -u https://ore-staging.openrights.exchange push action comakeryteam addaset '{"asset_contract":"eosio.token","asset_symbol":"4,ORE"}' -p comakeryteam@active```

### Add eosio.code active permissions

You need to add eosio.code permissions to contract and depositors. For example:

```cleos set account permission depositaccnt active depositaccnt --add-code```

```cleos set account permission comakeryteam active comakeryteam --add-code```

### Add vesting

```cleos -u https://ore-staging.openrights.exchange push action comakeryteam addvesting '{"account_id": "<vesting_account>","vesting_period": 1, "vesting_amount":"48000.0000 ORE","cliff_date":"2020-06-01T00:00:00","cliff_weight":"0.25","start_date":"2019-06-01T00:00:00","end_date":"2023-06-01T00:00:00"}'  -p comakeryteam@active```

### Execute claim

```cleos -u https://ore-staging.openrights.exchange push action comakeryteam claim '{"vesting_id": 0}' -p <vesting_account>@<permission>```

### Run local node and install system contracts

```
docker-compose up -d
./ignition.sh
```

The first step runs the local EOS node

The second step runs deployment system contracts

# Specification

The vesting contract has these features:

* Deployable to the EOS, ORE or Telos based blockchain
  * https://aikon.com/ore-blockchain
  * https://explorer.ore.network
* Reusable for EOS tokens and tokens on Telos blockchains such as ORE token

- A single vesting contract handles vesting escrows for multiple people. It is only deployed once.

- For each token recipient account an admin can specify: 

  1.  An amount to be granted after a period of time (“cliff”). For example 30% of tokens will be available for withdrawal after a 1 year period.
  2.  The remaining tokens will be distributed in monthly portions after the cliff. For example 1/36 of tokens will be available on the first day of each month after the cliff date. Note that the final period will be uneven due to rounding.

- An admin can pause the vesting on the account. 

- An admin can cancel the vesting for an account.

- An admin can reclaim the unvested token amount.

- After vesting or transfer there may be a separate lockup period. Unlike a vesting period, the tokens cannot be reclaimed by the issuer prior to the vesting date.

- Once the tokens are fully vested and the lockup period is over the recipient may transfer them wherever they like. 

- Vestings may have been promised in the past with the vestings contract token allocation configured later. So the calculation of vested and locked tokens should be able to use a start date in the past for vested and locked tokens.

- An admin can grant more than one vesting to the same recipient address.

- The vesting period defaults to monthly but can be set to monthly, weekly, daily and continuous. Continuous evenly vests the tokens with each verified block timestamp. The vesting occurs on the first moment of the vesting period such as midnight on the first of the month or midnight on the first day of the week, etc.

  <div style="page-break-after: always; break-after: page;"></div>

## Smart Contract Interface Pseudocode

The smart contract should implement the following or similar functions. Use unsigned ints or ints or make other changes as you see fit during development.

#### Admin Contract Configuration Functions Pseudocode

```cpp
void setToken(tokenAddress or identifier) //sets the token that is managed

void deposit(amount) //deposit tokens into the pool that are available for allocation

void withdraw(amount) //withdraw tokens from the pool that are available for allocation but not allocated
  
void getUnallocatedBalance() //returns the balance of unallocated tokens

void getAllocatedBalance() // returns the balance of allocated tokens

int getBalance() // returns the balance of allocated and unallocated tokens
  
void addAdmin(address) // adds an admin who can administrate the contract
  
void removeAdmin(address) // removes an admin. Does not allow less than 1 admin for the contract.
```

  <div style="page-break-after: always; break-after: page;"></div>

#### Admin Vesting Configuration Functions Pseudocode

```cpp
int createVesting(recipientAddress, amount, startDate, endDate, vestingPeriod=defaultMonthly, lockedUntilDate=defaultNone, cliffDate=defaultNone, cliffTokenAmount=defaultNone) // create a vesting and return the allocationId. Keep in mind that one recipient address can have multiple allocation.

int createAllocation(recipientAddress, amount, lockedUntilDate=defaultNoLockup) // create an allocation and return the allocationId. Keep in mind that one recipient address can have multiple allocations.

  
void cancelVesting(allocationId, pauseTime=defaultToNow) // cancels the vesting at a vesting time that is not in the past
  
void pauseVesting(allocationId, pauseTime=defaultToNow) // pauses the vesting at a vesting time that is not in the past
  
void unpauseVesting(allocationId, pauseTime=defaultToNow) // unpauses the vesting at a vesting time that is not in the past
```

#### Recipient Functions Pseudocode

```cpp
int transferTokens(amount, recipientAddress) // can only transfer vested and unlocked tokens

int getTransferableBalance(allocationId) // can only transfer vested and unlocked tokens

int getLockedBalance(allocationId)

int getLockedUntilDate(allocationId)

int getUnvestedBalance(allocationId)

??? getAllocationInfo(allocationId) // get summary of information about the allocation including vesting schedule
```

  <div style="page-break-after: always; break-after: page;"></div>

## Examples

#### 4 year vesting with 1 year cliff

* An employee is allocated 48,000 TKN tokens with a start date of  June 1, 2019
* 25% of the tokens (12,000 TKN) are scheduled to vest on the "cliff date" of June 1, 2020
* 1/36 of the remaining 36,000 tokens vest on the first day of each calendar month (1,000 TKN per month)
* The contract is configured on January 5th, 2021 with the backdated start date, cliff date and vesting schedule
* When the contract is configured on Jan 5th, 2021 the employee has vested the 12,000 TKN cliff amount and 7,000 TKN for the 7 monthly vestings occuring on the first day of July 2020 through January 2021.
* The employee can transfer 19,000 TKN as there is no lockup period. 
* 29,000 TKN tokens remain unvested in January.

#### Investor transfer with lockup period

* An investor purchases 100,000 TKN on June 15, 2020 at 1:23pm UTC with a 1 year lockup period.
* The contract admin configures the token contract for the investors tokens on Jan 5, 2021
* As of Jan 5 the investor cannot transfer the tokens and the contract admin cannot reclaim them
* The investor can transfer the tokens exactly 1 year later on June 15, 2021 at 1:23pm UTC

#### 2 year vesting with cancelation

* An employee is allocated 24,000 TKN tokens with a start date of  January 1, 2020
* The tokens vest evenly on the first day of each calendar month (1,000 TKN per month)
* The contract is configured on January 5th, 2021 with the backdated start date and vesting schedule
* When the contract is configured on Jan 5th, 2021 the employee has vested 13,000 tokens (12 months in 2020 + Jan 1, 2021 = 13 vestings)
* The employee is fired on January 20th, 2021 and the contract is canceled
* The employee can transfer 13,000 tokens and the contract admin can transfer or reallocate the remaining 11,000 TKN tokens from the vesting contract token pool
* Without withdrawing the tokens from the vesting contract, the contract admin allocates the tokens to another employee with a 12 month vesting schedule.

#### 2 year vesting with paused contract

* An employee is allocated 24,000 TKN tokens with a start date of  January 1, 2020
* The tokens vest evenly on the first day of each calendar month (1,000 TKN per month)
* The contract is configured on January 5th, 2021 with the backdated start date and vesting schedule
* When the contract is configured on Jan 5th, 2021 the employee has vested 13,000 tokens (12 months in 2020 + Jan 1, 2021 = 13 vestings)
* The employee goes on sabbatical on January 20th, 2021 but the company wants to be able to restart their vesting later.
* The vesting for the remaining 11,000 tokens for the employee address is paused
* The employee returns to work and the vesting is unpaused on January 2nd, 2022
* On February 1, 2022 the employee receives their next vesting of 1,000 tokens and 10,000 tokens remain
* The remaining 10,000 tokens vest at a rate of 1,000 tokens on the first day of each month from March 1, 2022 through December 1, 2022

#### Vesting & lockup together

* The employee received a 100 day vesting with a 100 day lockup for 100 tokens.
* The start date is set to 50 days ago,
* The employee is fired and the remaining vesting is canceled.
* The admin reclaims the 50 unvested tokens.
* The employee owns 50 tokens but they are still locked until the lockup period is over.
* The admin tries to claim the 50 vested employee owned tokens but the reclaim fails.
* 99 days from the start date the employee tries to transfer 50 tokens but the transfer fails because they are still locked.
100 days from the start date the tokens unlock and the ex-employee transfers their 50 tokens successfully.
