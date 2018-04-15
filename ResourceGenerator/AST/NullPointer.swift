//
//  NullPointer.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a null pointer (nullptr) expression
public class NullPointer: Expression
{
    public static let null = NullPointer()
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
