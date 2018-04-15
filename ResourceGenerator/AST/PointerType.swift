//
//  PointerType.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a pointer type (i.e. `Pointee*`)
public class PointerType: Type
{
    public let pointeeType: Type
    
    public init(pointeeType: Type)
    {
        self.pointeeType = pointeeType
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
