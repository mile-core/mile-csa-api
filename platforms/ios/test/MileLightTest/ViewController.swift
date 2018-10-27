//
//  ViewController.swift
//  MileLightTest
//
//  Created by lotus mile on 22.08.2018.
//  Copyright © 2018 MILE. All rights reserved.
//

import UIKit
import MileCsaLight

class ViewController: UIViewController {
    
    override func viewDidLoad() {
        super.viewDidLoad()
        do {
            
            let pks = try MileCsaKeys.random();
            
            Swift.print("PKS:  \(pks)")
            
            Swift.print("Validate public: \(MileCsaKeys.validatePublic("2qPBE5ZVqxFSKZyLmuaMdi1dSvbT2HffQP7XrTB6akXoeJz216"))")
            Swift.print("Validate private: \(MileCsaKeys.validatePrivate("9RX5j9APJtu5gSbD2VTrdhMSctcF9rwVFsoiWYawyPsB1HRJ4Eq1KC7f6ourpWaYSzLAk3qRrBiU7tjKAS7M4usFF6eRz"))")
            
            let pks_sph1 = try MileCsaKeys.withSecretPhrase("фраза какая-то")
            let pks_sph2 = try MileCsaKeys.withSecretPhrase("фраза какая-то")
            let pks_sph3 = try MileCsaKeys.withSecretPhrase("фраза какая-то другая")
            
            let pks_sph4 = try MileCsaKeys.fromPrivateKey(pks_sph3.privateKey)
            
            
            Swift.print(" pks with secret phrase1 = \(pks_sph1, pks_sph1==pks_sph2) ")
            Swift.print(" pks with secret phrase2 = \(pks_sph2, pks_sph1.privateKey==pks_sph2.privateKey) ")
            Swift.print(" pks with secret phrase3 = \(pks_sph3, pks_sph1==pks_sph3) ")
            
            Swift.print(" pks with private key phrase4 = \(pks_sph4, pks_sph3==pks_sph4) ")
            
            let dest = pks_sph1.publicKey
            let t = try MileCsaTransaction.transfer(pks,
                                                    destPublicKey: dest,
                                                    blockId: "0",
                                                    transactionId: 0,
                                                    assetCode: 0,
                                                    amount: 1.00,
                                                    fee: 0.0,
                                                    description: nil
                ) as! [String: Any]
            
            let e = try MileCsaTransaction.emission(pks,
                                                    blockId: "0",
                                                    transactionId: 0,
                                                    assetCode: 0,
                                                    fee: 0.0
                ) as! [String: Any]
            
            Swift.print("Wallet transfer: ", t)
            
            Swift.print("Wallet emission: ", e)
            
        }
        catch let error {
            Swift.print("Error: \(error)")
        }
        
    }
    
}

