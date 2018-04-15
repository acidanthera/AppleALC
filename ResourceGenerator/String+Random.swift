//
//  String+Random.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

public extension String
{
    public static func random4() -> String
    {
        return UUID().uuidString.substring(to: 4)
    }
    
    public static func random8() -> String
    {
        return UUID().uuidString.substring(to: 8)
    }
}
