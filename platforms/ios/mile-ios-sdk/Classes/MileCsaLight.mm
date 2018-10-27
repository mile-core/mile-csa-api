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

inline NSError *error2NSError(const std::string &errorMessage, milecsa::result code, NSString *description){
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
- (const milecsa::keys::Pair&) get_pair;
@end

@implementation MileCsaKeys
{
    milecsa::keys::Pair _keysPair;
}

- (const milecsa::keys::Pair&)get_pair{
    return _keysPair;
}

- (NSString *) publicKey {
    return [NSString stringWithUTF8String: _keysPair.get_public_key().encode().c_str()];
}

- (NSString *) privateKey {
    return [NSString stringWithUTF8String: _keysPair.get_private_key().encode().c_str()];
}

+ (BOOL) validate:(MileCsaKeys *)keys {
    return milecsa::keys::Pair::Validate(keys->_keysPair);
}

+ (BOOL) validatePublic:(NSString *)key {
    return milecsa::keys::Pair::ValidatePublicKey([key UTF8String]);
}

+ (BOOL) validatePrivate:(NSString *)key {
    return milecsa::keys::Pair::ValidatePrivateKey([key UTF8String]);
}

- (nonnull instancetype) initWith:(const milecsa::keys::Pair &)pair {
    self = [super init];
    if (self) {
        _keysPair = pair;
    }
    return self;
}

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
                                            const std::string &result) mutable -> void {
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
                                                                     const std::string &result) mutable -> void {
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
                                                                             const std::string &result) mutable -> void {
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

@implementation MileCsaTransaction

+(nullable NSDictionary*) base_transfer:(int)type
                            wallet_pair:(MileCsaKeys *)wallet_pair
                          destPublicKey:(NSString *)destPublicKey
                                blockId:(NSString *)blockId
                          transactionId:(uint64_t)transactionId
                              assetCode:(unsigned short)assetCode
                                 amount:(float)amount
                                    fee:(float)fee
                            description:(NSString *)description
                                  error:(NSError *__autoreleasing  _Null_unspecified *)error{
    
    auto _error = [error](milecsa::result code, std::string result) mutable -> void {
        *error = error2NSError(result, code, @"Transfer error");
    };
    
    auto pair = [wallet_pair get_pair];
    
    uint256_t ui;
    
    if(!StringToUInt256([blockId UTF8String], ui, false)) {
        *error = error2NSError("block could not be converted to uint256_t", milecsa::result::FAIL, @"Transfer error");
    }
    
    std::string desc = description == nil ? "" : [description UTF8String];
    
    milecsa::transaction::JsonRequest transaction;
    
    using  transfer = milecsa::transaction::JsonTransfer;
    using  emission = milecsa::transaction::JsonEmission;
    
    switch (type) {
        case 0:
            transaction = transfer::CreateRequest(
                                                  pair,
                                                  [destPublicKey UTF8String],
                                                  ui,
                                                  transactionId,
                                                  milecsa::assets::TokenFromCode(assetCode),
                                                  amount,
                                                  fee,
                                                  desc,
                                                  _error);
            break;
        case 1:
            transaction = emission::CreateRequest(
                                                  pair,
                                                  ui,
                                                  transactionId,
                                                  milecsa::assets::TokenFromCode(assetCode),
                                                  fee,
                                                  _error);
            break;
        default:
            *error = error2NSError("Unknown transaction", milecsa::result::UNKNOWN, @"Request builder error");
            return nil;
    }
    
    if(transaction){
        if (auto trx = transaction->get_body()){
            NSData *data = [[NSString stringWithUTF8String:trx->dump().c_str()]
                            dataUsingEncoding:NSUTF8StringEncoding];
            return  [NSJSONSerialization JSONObjectWithData:data
                                                    options:kNilOptions
                                                      error:error];
        }
    }
    return nil;
}


+(nullable NSDictionary*) transfer:(MileCsaKeys *)wallet_pair
                     destPublicKey:(NSString *)destPublicKey
                           blockId:(NSString *)blockId
                     transactionId:(uint64_t)transactionId
                         assetCode:(unsigned short)assetCode
                            amount:(float)amount
                               fee:(float)fee
                       description:(NSString *)description
                             error:(NSError *__autoreleasing  _Null_unspecified *)error{
    
    return [self base_transfer: 0
                   wallet_pair:wallet_pair
                 destPublicKey:destPublicKey
                       blockId:blockId
                 transactionId:transactionId
                     assetCode:assetCode
                        amount:amount
                           fee:fee
                   description:description
                         error:error];
    
}


+(nullable NSDictionary*) emission:(MileCsaKeys *)wallet_pair
                           blockId:(NSString *)blockId
                     transactionId:(uint64_t)transactionId
                         assetCode:(unsigned short)assetCode
                               fee:(float)fee
                             error:(NSError *__autoreleasing  _Null_unspecified *)error{
    return [self base_transfer: 1
                   wallet_pair:wallet_pair
                 destPublicKey:@""
                       blockId:blockId
                 transactionId:transactionId
                     assetCode:assetCode
                        amount:0
                           fee:fee
                   description:@""
                         error:error];
}

@end
