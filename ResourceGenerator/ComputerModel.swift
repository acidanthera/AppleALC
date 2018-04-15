//
//  ComputerModel.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-07.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents all supported computer platforms
public enum ComputerModel: String, Codable
{
    case desktop = "Desktop"
    
    case mobile = "Laptop"
    
    case any = "Any"
    
    /// Get the equivalent C++ value
    public var cppValue: String
    {
        switch self
        {
        case .desktop:
            return "WIOKit::ComputerModel::ComputerDesktop"
            
        case .mobile:
            return "WIOKit::ComputerModel::ComputerLaptop"
            
        case .any:
            return "WIOKit::ComputerModel::ComputerAny"
        }
    }
}
