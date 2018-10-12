//
//  MileWalletBridge.h
//  mile-ios-sdk
//
//  Created by denis svinarchuk on 18.06.2018.
//  Copyright © 2018 Mile Core. All rights reserved.
//


#import <Foundation/Foundation.h>

#define NSMileErrorDomain @"NSMileErrorDomain"

/**
 * Mile wallet pair keys container
 */
@interface MileCsaKeys : NSObject

/**
 * Public key
 */
@property(readonly) NSString * _Nonnull publicKey;

/**
 Private key
 */
@property(readonly) NSString * _Nonnull privateKey;

/**
 * Validate wallet pair
 *
 * @param keys - pair keys
 * @return validation result true or false
 */
+ (BOOL) validate:(nonnull MileCsaKeys*)keys;

/**
 * Validate public key
 *
 * @param key - public key string encoded as base58
 * @return validation result true or false
 */
+ (BOOL) validatePublic:(nonnull NSString*)key;

/**
 * Validate private key
 *
 * @param key - private key string encoded as base58
 * @return validation result true or false
 */
+ (BOOL) validatePrivate:(nonnull NSString*)key;


/**
 * Create new wallet pair: public and private keys
 *
 * @param error - handle error if pair could not be created
 * @return wallet pair keys
 */
+(nullable MileCsaKeys*)Random:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;


/**
 * Create new wallet pair with as secret phrase
 *
 * @param phrase - secret phrase
 * @param error - handle error if pair could not be created
 * @return wallet pair keys
 */
+(nullable MileCsaKeys*)WithSecretPhrase:(nonnull NSString*)phrase
                                   error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;


/**
 * Restore wallet pair from private key
 *
 * @param privateKey - wallet private key
 * @param error - handle error if pair could not be created
 * @return wallet pair keys
 */
+(nullable MileCsaKeys*)FromPrivateKey:(nonnull NSString*)privateKey
                                 error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;

@end


/**
 * Transactions request builder container
 */
@interface MileCsaTransaction : NSObject

/**
 * Prepare body of signed transaction to make transfer from wallet to certain destination address (public key of recepient)
 *
 * @param pair - wallet pair of assets owner
 * @param destPublicKey - destination address
 * @param blockId - current blockchain block id
 * @param transactionId - user defined transaction id, it may be random or be recieved as wallet transaction id from wallet state
 * @param assetCode - transfer asset
 * @param amount - asset amount
 * @param description - user defined description attached to the transfer
 * @param fee - fee proposal
 * @param error - handle error if body of request could not be created
 * @return body of request
 */
+(nullable NSDictionary*) transfer:(nonnull MileCsaKeys *)pair
                 destPublicKey:(nonnull NSString*)destPublicKey
                       blockId:(nonnull NSString*)blockId
                 transactionId:(uint64_t)transactionId
                     assetCode:(unsigned short)assetCode
                        amount:(nonnull NSString*)amount
                        description:(nullable NSString*)description
                        fee:(nullable NSString*)fee
                         error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;

/**
 * Prepare body of signed transaction to make emission from wallet to certain destination address (public key of recepient)
 *
 * @param pair - wallet pair of assets owner
 * @param destPublicKey - destination address
 * @param blockId - current blockchain block id
 * @param transactionId - user defined transaction id, it may be random or be recieved as wallet transaction id from wallet state
 * @param assetCode - transfer asset
 * @param amount - asset amount
 * @param description - user defined description attached to the transfer
 * @param fee - fee proposal
 * @param error - handle error if body of request could not be created
 * @return body of request
 */
+(nullable NSDictionary*) emission:(nonnull MileCsaKeys *)pair
                 destPublicKey:(nonnull NSString*)destPublicKey
                       blockId:(nonnull NSString*)blockId
                 transactionId:(uint64_t)transactionId
                     assetCode:(unsigned short)assetCode
                        amount:(nonnull NSString*)amount
                        description:(nullable NSString*)description
                        fee:(nullable NSString*)fee
                         error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;
@end

