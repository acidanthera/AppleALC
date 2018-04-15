//
//  Assign.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents an **EXPRESS** assign statement
/// An express assign statement combines the variable declaration and value assignment
public class Assign: Statement
{
    /// The variable declaration
    public let variable: Variable
    
    /// The variable value to be assigned
    public let value: Expression
    
    /// Initialize an **expression** assign statement
    ///
    /// - Parameters:
    ///   - variable: The variable declaration
    ///   - value: The value to be assigned to the `variable`
    public init(variable: Variable, value: Expression)
    {
        self.variable = variable
        
        self.value = value
    }
    
    /// **(Convenient)** Initialize an **express** assign statement
    ///
    /// - Parameters:
    ///   - type: The variable type
    ///   - identifier: The variable identifier
    ///   - value: The value to be assigned to this variable
    public convenience init(type: Type, identifier: Identifier, value: Expression)
    {
        self.init(variable: Variable(type: type, identifier: identifier), value: value)
    }
    
    /// **(Convenient)** Initialize an **express** assign statement
    ///
    /// - Parameters:
    ///   - type: The variable type
    ///   - name: The variable name
    ///   - value: The value to be assigned to this variable
    public convenience init(type: Type, name: String, value: Expression)
    {
        self.init(variable: Variable(type: type, name: name), value: value)
    }
    
    /// **(Convenient)** Create an *express* static const assign statement
    ///
    /// - Parameters:
    ///   - type: The variable type
    ///   - name: The variable name
    ///   - value: The value to be assigned to this variable
    /// - Returns: The assign statement
    public static func staticConst(type: Type, name: String, value: Expression) -> Assign
    {
        return Assign(variable: Variable(type: type, name: name).makeStatic().makeConstant(), value: value)
    }
    
    /// **(Convenient)** Create an *express* static assign statement
    ///
    /// - Parameters:
    ///   - type: The variable type
    ///   - name: The variable name
    ///   - value: The value to be assigned to this variable
    /// - Returns: The assign statement
    public static func `static`(type: Type, name: String, value: Expression) -> Assign
    {
        return Assign(variable: Variable(type: type, name: name).makeStatic(), value: value)
    }
    
    /// **(Convenient)** Create an *express* const assign statement
    ///
    /// - Parameters:
    ///   - type: The variable type
    ///   - name: The variable name
    ///   - value: The value to be assigned to this variable
    /// - Returns: The assign statement
    public static func const(type: Type, name: String, value: Expression) -> Assign
    {
        return Assign(variable: Variable(type: type, name: name).makeConstant(), value: value)
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
