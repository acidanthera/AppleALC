//
//  String+Padding.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-14.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

public extension String
{
    public func leftPadding(_ character: Character, to width: Int) -> String
    {
        guard width > self.count else
        {
            return self
        }
        
        return String(repeating: character, count: width - self.count).appending(self)
    }
}
