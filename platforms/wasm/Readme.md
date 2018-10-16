## API Example

```javascript
// enable debug error message
Module.pcsa_pdbg_set_groups("all")

// error message contain
// error.what - contain string with error message

var error = new Module.pcsa_error()
// container for keys pair
var keyPairs = new Module.pcsa_keys_pair()

// generate private public key
Module.generate_key_pair(keyPairs, error)
// to check compare with Module.pcsa_result.PCSA_RES_OK
// like this Module.pcsa_result.PCSA_RES_OK == Module.generate_key_pair(keyPairs, error)

console.log("Private key: " + keyPairs.private_key())
console.log("Public key : " + keyPairs.public_key())

// generate private public key with phrase
Module.generate_key_pair_with_secret_phrase(keyPairs, "my super secret phrase",error)

/**
 * generate public key if you have only private.
 * @constructor
 * @param {pcsa_keys_pair} keyPairs - keys pair contain only private key.
 * @param {pcsa_error} error - error description.
 * example Module.generate_key_pair_from_private_key(keyPairs, error)
 */
Module.generate_key_pair_from_private_key(keyPairs, error)

/**
 * transfer asset transaction.
 * @constructor
 * @param {pcsa_keys_pair} keyPairs - src keys pair.
 * @param {string} publicKey - dst public key.
 * @param {string} ui - transaction id.
 * @param {number} asset - asset number from blockchain-info.
 * @param {string} amount - token amount.
 * @param {pcsa_error} error - error description.
 * @return {string} transaction - transaction string. If zero length show error	message.
 * example Module.create_transaction_transfer_assets(keyPairs, dstKeyPairs.public_key(), "2", 0, "1000", error)
 */

var dstKeyPairs = new Module.pcsa_keys_pair()
Module.generate_key_pair(dstKeyPairs, error)
Module.create_transaction_transfer_assets(keyPairs, dstKeyPairs.public_key(), ui, asset, amount, error))

```

## JSON-RPC Examples
Assuming we use `axios` package for requests we don't include `import` statement in examples

Json-rpc nodes list available on https://wallet.mile.global/v1/nodes.json
You can use any of node. In examples we use non-existing **example address**: https://example.i.mile.global

### Wallet state
```javascript
getBalance( publicKey ) {
  return axios.post(
    "https://node002.testnet.mile.global",
    {
      method: "get-wallet-state",
      params: { public_key: publicKey },
      id: 1,
      jsonrpc: "2.0",
      version: "1.0"
    }
  );
}
```

### Send transaction
```javascript
// transactionData created by create_transaction_transfer_assets function above
sendTransaction( transactionData ) {
  return axios.post(
    "https://node002.testnet.mile.global",
    {
      method: "send-signed-transaction",
      params: { transaction_data: transactionData },
      id: 2,
      jsonrpc: "2.0",
      version: "1.0"
    }
  );
}
```

### Get last transactions
```javascript
getLastTransactions( publicKey, count ) {
  return axios.post(
    "https://node002.testnet.mile.global",
    {
      method: "get-wallet-transactions",
      params: { public_key: publicKey, count },
      id: 3,
      jsonrpc: "2.0",
      version: "1.0"
    }
  );
}
```