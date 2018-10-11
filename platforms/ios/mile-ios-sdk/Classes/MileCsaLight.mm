//
//  MileWalletBridge.m
//  mile-ios-sdk
//
//  Created by denis svinarchuk on 18.06.2018.
//  Copyright © 2018 Mile Core. All rights reserved.
//

#include <stdio.h>
#include "milecsa.hpp"
#import "MileCsaLight.h"

using result = milecsa::result;

inline NSError *error2NSError(std::string &errorMessage, milecsa::result code, NSString *description){
    if (!errorMessage.empty()) {
        NSDictionary *userInfo = @{
                                   NSLocalizedDescriptionKey: NSLocalizedString([NSString stringWithUTF8String:errorMessage.c_str()], nil),
                                   NSLocalizedFailureReasonErrorKey: NSLocalizedString(description, nil)
                                   };
        NSError *error = [NSError errorWithDomain:NSMileErrorDomain code:(NSInteger)(code)
                                         userInfo:userInfo];
        return error;
    }
    return nil;
}


@interface MileCsaKeys()
//@property(readonly) milecsa::keys::pair *keysPair;
@end

@implementation MileCsaKeys
{
    milecsa::keys::Pair _keysPair;
}

- (NSString *) publicKey {
    return [NSString stringWithUTF8String: _keysPair.get_public_key().encode().c_str()];
}

- (NSString *) privateKey {
    return [NSString stringWithUTF8String: _keysPair.get_private_key().encode().c_str()];
}

+ (BOOL) validate:(MileCsaKeys *)keys {
    return milecsa::keys::Pair::Validate(
                                         keys->_keysPair,
                                         [](milecsa::result code,std::string error) {});
}

+ (BOOL) validatePublic:(NSString *)key {
    return milecsa::keys::Pair::ValidatePublicKey(
                                                  [key UTF8String],
                                                  [](milecsa::result code,std::string error) {});
}

+ (BOOL) validatePrivate:(NSString *)key {
    return milecsa::keys::Pair::ValidatePrivateKey(
                                                   [key UTF8String],
                                                   [](milecsa::result code,std::string error) {});
}

- (nonnull instancetype) initWith:(const milecsa::keys::Pair &)pair {
    self = [super init];
    if (self) {
        _keysPair = pair;
    }
    return self;
}

//- (milecsa::keys::pair*) keysPair {
//    return &_keysPair;
//}

- (BOOL) isEqual:(id)object {
    return [self.privateKey isEqual:[(MileCsaKeys*)object privateKey]];
} 

- (NSString*) description {
    return [[NSString alloc] initWithFormat:@"%@:%@", self.publicKey, self.privateKey];
}

+ (nullable MileCsaKeys*) Random:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error{
    
    if (auto pair =
        milecsa::keys::Pair::Random([error](
                                            milecsa::result code,
                                            std::string result) mutable -> void {
        *error = error2NSError(result, code, @"Key generator error");
    }))
    {
        return [[MileCsaKeys alloc] initWith: *pair];
    }
    else {
        return nil;
    }
}

+ (nullable MileCsaKeys*) WithSecretPhrase:(NSString *)phrase
                                     error:(NSError *__autoreleasing  _Null_unspecified *)error {
    if (auto pair =
        milecsa::keys::Pair::WithSecret([phrase UTF8String], [error](
                                                                     milecsa::result code,
                                                                     std::string result) mutable -> void {
        *error = error2NSError(result, code, @"Key generator error");
    }))
    {
        return [[MileCsaKeys alloc] initWith: *pair];
    }
    else {
        return nil;
    }
}

+ (nullable MileCsaKeys*) FromPrivateKey:(NSString *)privateKey
                                   error:(NSError *__autoreleasing  _Null_unspecified *)error {
    if (auto pair =
        milecsa::keys::Pair::FromPrivateKey([privateKey UTF8String], [error](
                                                                             milecsa::result code,
                                                                             std::string result) mutable -> void {
        *error = error2NSError(result, code, @"Key generator error");
    }))
    {
        return [[MileCsaKeys alloc] initWith: *pair];
    }
    else {
        return nil;
    }
    
}
@end

@implementation MileCsa

//+ (nullable NSString*) registerNode:(MileCsaKeys *)keyPair
//                   address:(NSString *)address
//             transactionId:(uint64_t)transactionId
//                     error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error
//{
//    milecsa_keys_pair keys;  dumpKeys(&keys, keyPair);
//    char *errorMessage = nullptr;
//    char *transaction = nullptr;
//    milecsa_result code;
//
//    if ( (code=milecsa_create_transaction_register_node(&keys,
//                                         [address UTF8String],
//                                         transactionId,
//                                         &transaction, &errorMessage)) == PCSA_RES_OK){
//        return string2NSString(transaction);
//    }
//    else {
//        *error = error2NSError(errorMessage, code, @"Register node transaction error");
//    }
//    return nil;
//}

//+ (nullable NSString*) unregisterNode:(MileCsaKeys *)keyPair
//               transactionId:(uint64_t)transactionId
//                       error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error{
//    milecsa_keys_pair keys;  dumpKeys(&keys, keyPair);
//    char *errorMessage = nullptr;
//    char *transaction = nullptr;
//    milecsa_result code;
//
//    if((code=create_transaction_unregister_node(&keys, transactionId, &transaction, &errorMessage)) == PCSA_RES_OK) {
//       return string2NSString(transaction);
//    }
//    else {
//        *error = error2NSError(errorMessage, code, @"Unregister node transaction error");
//    }
//    return nil;
//}

+ (nullable NSString*) createTransfer:(MileCsaKeys *)srcKeyPair
                        destPublicKey:(NSString *)destPublicKey
                        transactionId:(uint64_t)transactionId
                               assets:(unsigned short)assets
                               amount:(NSString *)amount
                                error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error{
    
    //milecsa::keys::pair keys;  dumpKeys(&keys, srcKeyPair);
    
    //char *errorMessage = nullptr;
    //char *transaction = nullptr;
    //milecsa_result code;
    
    //    if((code=create_transaction_transfer_assets(&keys,
    //                                          [destPublicKey UTF8String],
    //                                          transactionId,
    //                                          assets,
    //                                          [amount UTF8String],
    //                                          &transaction,
    //                                          &errorMessage)) == PCSA_RES_OK){
    //       return string2NSString(transaction);
    //    }
    //    else {
    //        *error = error2NSError(errorMessage, code, @"Create transfer transaction error");
    //    }
    return nil;
}

@end
