//
//  String+SubString.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

public extension String
{
    public func substring(from start: Int = 0, to end: Int) -> String
    {
        let startIndex = self.index(self.startIndex, offsetBy: start)
        
        let endIndex = self.index(self.startIndex, offsetBy: end)
        
        return String(self[startIndex..<endIndex])
    }
}
