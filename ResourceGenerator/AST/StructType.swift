//
//  StructType.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a struct type
public class StructType: Type
{
    /// The struct name
    public let name: String
    
    /// Initialize a struct type
    ///
    /// - Parameter name: The name of the struct
    public init(name: String)
    {
        self.name = name
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
