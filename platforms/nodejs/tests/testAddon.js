"use strict";

/* global it,describe */
var milecsa = require('../build/Release/milecsa');
var assert = require("assert");

var Module = new milecsa.Module()

describe("Boost Based Addon", function() {
    it("initial test", function() {

        var pair = new Module.Pair.Random()

        console.log("Random private key: " + pair.private_key)
        console.log("Random public key : " + pair.public_key)

        var with_secret = new Module.Pair.WithSecret("the secreast phrase!")

        console.log("With secret private key: " + with_secret.private_key)
        console.log("With secret public key : " + with_secret.public_key)

        var from_private = new Module.Pair.FromPrivateKey(with_secret.private_key)

        console.log("From Private key private key: " + from_private.private_key)
        console.log("From Private key public key : " + from_private.public_key)

        var ret = Module.Pair.ValidatePublicKey(from_private.public_key)
        console.log("... ValidatePublicKey : " + ret)

        ret = Module.Pair.ValidatePrivateKey(from_private.private_key)
        console.log("... ValidatePrivateKey : " + ret)

        try {
            Module.Pair.ValidatePublicKey("")
        }
        catch (e) {
            console.log("Invalid ValidatePublicKey key: " + e)
        }

        try {
            var from_private = Module.Pair.FromPrivateKey("invalide")
        }
        catch (e) {
            console.log("Invalid Private key: " + e)
        }

        var transaction =  new Module.Transaction()

        console.log(" Transaction : " + transaction)

        var transfer  =  transaction.Transfer(
            pair,                    // pair
            from_private.public_key, // destination
            "2",                     // block id
            "0",                     // transaction id
            0,                       // asset code
            "1000",                  // amount
            "memo field",            // description
            ""                       // fee, always ""
            )

        console.log(" Transfer    : ", transfer);

        var emission  =  transaction.Emission(
            pair,                    // pair
            from_private.public_key, // destination
            "2",                     // block id
            "0",                     // transaction id
            0,                       // asset code
            "1000",                  // amount
            "memo field",            // description
            ""                       // fee, always ""
        )

        console.log(" Emission    : ", emission);


        try {
            var emission  =  transaction.Emission(
                pair,                    // pair
                "not valid",             // destination
                "2",                     // block id
                "0",                     // transaction id
                0,                       // asset code
                "1000",                  // amount
                "memo field",            // description
                ""                       // fee, always ""
            )
        }
        catch (e) {
            console.log("Invalid Emission " + e)
        }

        assert.equal(from_private.public_key, with_secret.public_key);
    });
});