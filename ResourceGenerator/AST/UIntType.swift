//
//  UIntType.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents an `uintX_t` type
public class UIntType: Type
{
    /// The number of bits required to store this unsigned integer
    public let numBits: Int
    
    /// Initialize an unsigned integer type
    ///
    /// - Parameter numBits: The number of bits required to store the int
    public init(numBits: Int)
    {
        self.numBits = numBits
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
