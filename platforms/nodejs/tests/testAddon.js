"use strict";

/* global it,describe */
var milecsa = require('../build/Release/milecsa');
var assert = require("assert");

describe("Boost Based Addon", function() {
    it("initial test", function() {

        var Module = new milecsa.Module()
        var pair = Module.Pair.Random()

        console.log("Random private key: " + pair.private_key)
        console.log("Random public key : " + pair.public_key)

        var with_secret = Module.Pair.WithSecret("the secreast phrase!")

        console.log("With secret private key: " + with_secret.private_key)
        console.log("With secret public key : " + with_secret.public_key)

        var from_private = Module.Pair.FromPrivateKey(with_secret.private_key)

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
            console.log("Invalide ValidatePublicKey key: " + e)
        }

        try {
            var from_private = Module.Pair.FromPrivateKey("invalide")
        }
        catch (e) {
            console.log("Invalide Private key: " + e)
        }


        assert.equal(from_private.public_key, with_secret.public_key);
    });
});