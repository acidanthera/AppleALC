//
//  Vendor.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-06.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents all supported HDA vendors
public enum Vendor: String, Codable
{
    case amd = "AMD"
    
    case analogDevices = "AnalogDevices"
    
    case cirrusLogic = "CirrusLogic"
    
    case conexant = "Conexant"
    
    case creative = "Creative"
    
    case idt = "IDT"
    
    case intel = "Intel"
    
    case nvidia = "NVIDIA"
    
    case realtek = "Realtek"
    
    case via = "VIA"
    
    /// Get the vendor id in decimal number
    ///
    /// - Note:
    /// Values are obtained from the `Vendors.plist` file.
    public var id: Int
    {
        switch self
        {
        case .amd:
            return 4098
            
        case .analogDevices:
            return 4564
            
        case .cirrusLogic:
            return 4115
            
        case .conexant:
            return 5361
            
        case .creative:
            return 4354
            
        case .idt:
            return 4381
            
        case .intel:
            return 32902
            
        case .nvidia:
            return 4318
            
        case .realtek:
            return 4332
            
        case .via:
            return 4358
        }
    }
    
    /// Get the vendor id in hex string
    public var hexID: String
    {
        return "0x" + String(self.id, radix: 16, uppercase: true)
    }
}
