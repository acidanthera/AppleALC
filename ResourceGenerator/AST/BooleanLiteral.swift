//
//  BooleanLiteral.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a boolean literal expression
public class BooleanLiteral: Expression
{
    /// The boolean value of this literal
    public let value: Bool
    
    /// Initialize a boolean literal
    ///
    /// - Parameter value: The boolean value
    public init(value: Bool)
    {
        self.value = value
    }
    
    /// Get the shared `true` literal
    public static let `true` = BooleanLiteral(value: true)
    
    /// Get the shared `false` literal
    public static let `false` = BooleanLiteral(value: false)
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}

/// Convert a boolean to a literal expression conveniently
extension Bool: LiteralExpressible
{
    /// Get the literal expression of this bool
    public var literal: Expression
    {
        return self ? BooleanLiteral.true : BooleanLiteral.false
    }
}
