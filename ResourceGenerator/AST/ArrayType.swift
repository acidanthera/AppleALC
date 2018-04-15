//
//  ArrayType.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a generic array type
public class ArrayType: Type
{
    public let elementType: Type
    
    public init(elementType: Type)
    {
        self.elementType = elementType
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
