//
//  Identifier.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents an identifier expression
public class Identifier: Expression
{
    /// The name of this identifier
    public let name: String
    
    /// Initialize an identifier expression
    ///
    /// - Parameter name: The name of this identifier
    public init(name: String)
    {
        self.name = name
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}

/// Convert a string to an identifier expression conveniently
public extension String
{
    public var identifier: Identifier
    {
        return Identifier(name: self)
    }
}
