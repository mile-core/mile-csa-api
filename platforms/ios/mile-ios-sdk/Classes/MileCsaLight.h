//
//  MileWalletBridge.h
//  mile-ios-sdk
//
//  Created by denis svinarchuk on 18.06.2018.
//  Copyright © 2018 Mile Core. All rights reserved.
//


#import <Foundation/Foundation.h>

#define NSMileErrorDomain @"NSMileErrorDomain"

@interface MileCsaKeys : NSObject
@property(readonly) NSString * _Nonnull publicKey;
@property(readonly) NSString * _Nonnull privateKey;

+ (BOOL) validate:(nonnull MileCsaKeys*)keys;
+ (BOOL) validatePublic:(nonnull NSString*)key;
+ (BOOL) validatePrivate:(nonnull NSString*)key;

+(nullable MileCsaKeys*)Random:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;

+(nullable MileCsaKeys*)WithSecretPhrase:(nonnull NSString*)phrase
                                               error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;

+(nullable MileCsaKeys*)FromPrivateKey:(nonnull NSString*)privateKey
                                             error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;

@end

@interface MileCsa : NSObject

+(nullable NSString*) createTransfer:(nonnull MileCsaKeys *)srcKeyPair
                       destPublicKey:(nonnull NSString*)destPublicKey
                       
                       transactionId:(uint64_t)transactionId
                              assets:(unsigned short)asstes amount:(nonnull NSString*)amount
                               error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;

+(nullable NSString*) registerNode:(nonnull MileCsaKeys *)keyPair
                           address:(nonnull NSString*)address
                     transactionId:(uint64_t)transactionId
                             error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;

+(nullable NSString*) unregisterNode:(nonnull MileCsaKeys *)keyPair
                       transactionId:(uint64_t)transactionId
                               error:(NSError *_Null_unspecified __autoreleasing *_Null_unspecified)error;
@end

