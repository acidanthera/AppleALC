//
//  AddressOf.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents an "address of" expression (i.e. &variable)
public class AddressOf: Expression
{
    public let expression: Expression
    
    public init(expression: Expression)
    {
        self.expression = expression
    }
    
    public init(identifier name: String)
    {
        self.expression = name.identifier
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
