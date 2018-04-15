//
//  Variable.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a variable declaration
public class Variable: AST
{
    /// The variable type
    public let type: Type
    
    /// The variable identifier
    public let identifier: Identifier
    
    /// Initialize a variable declaration
    ///
    /// - Parameters:
    ///   - type: The variable type
    ///   - identifier: The variable identifier
    public init(type: Type, identifier: Identifier)
    {
        self.type = type
        
        self.identifier = identifier
    }
    
    /// **(Convenient)** Initialize a variable declaration
    ///
    /// - Parameters:
    ///   - type: The variable type
    ///   - name: The variable name
    public convenience init(type: Type, name: String)
    {
        self.init(type: type, identifier: Identifier(name: name))
    }
    
    /// Indicate whether this is a static variable
    public private(set) var isStatic: Bool = false
    
    /// Indicate whether this is a constant
    public private(set) var isConstant: Bool = false
    
    /// Make this variable declaration static
    ///
    /// - Returns: The expression itself for chaining calls.
    public func makeStatic() -> Variable
    {
        self.isStatic = true

        return self
    }
    
    /// Make this expression a constant
    ///
    /// - Returns: The expression itself for chaining calls.
    public func makeConstant() -> Variable
    {
        self.isConstant = true
        
        return self
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
