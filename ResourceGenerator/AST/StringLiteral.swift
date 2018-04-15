//
//  StringLiteral.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a string literal expression
public class StringLiteral: Expression
{
    /// The content of this string literal
    public let content: String
    
    /// Initialize a string literal expression
    ///
    /// - Parameter content: The literal content
    public init(content: String)
    {
        self.content = content
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}

/// Convert a string to a literal expression conveniently
extension String: LiteralExpressible
{
    /// Get the literal expression of this string
    public var literal: Expression
    {
        return StringLiteral(content: self)
    }
}
