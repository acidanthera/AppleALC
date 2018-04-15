//
//  ArrayLookup.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents an array lookup expression (i.e. array[index])
public class ArrayLookup: Expression
{
    public let array: Expression
    
    public let index: Expression
    
    public init(array: Expression, index: Expression)
    {
        self.array = array
        
        self.index = index
    }
    
    public convenience init(array: Expression, index: Int)
    {
        self.init(array: array, index: index.literal)
    }
    
    public convenience init(array: String, index: Int)
    {
        self.init(array: array.literal, index: index.literal)
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
