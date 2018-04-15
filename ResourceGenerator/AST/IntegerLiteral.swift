//
//  IntegerLiteral.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents an integer literal expression
public class IntegerLiteral: Expression
{
    /// The value of this integer literal
    public let value: Int
    
    /// Initialize an integer literal expression
    ///
    /// - Parameter value: The integer value
    public init(value: Int)
    {
        self.value = value
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}

/// Convert an integer to a literal expression conveniently
extension Int: LiteralExpressible
{
    /// Get the literal expression of this integer
    public var literal: Expression
    {
        return IntegerLiteral(value: self)
    }
}
