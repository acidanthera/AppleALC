//
//  Block.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a block of statements
public class Block: Statement, Sequence
{
    /// An array of statements inside this block
    private var statements: [Statement]
    
    /// Initialize an empty block of statements
    public override init()
    {
        self.statements = []
    }
    
    /// Add a statement to this block
    ///
    /// - Parameter statement: A statement
    public func add(statement: Statement)
    {
        self.statements.append(statement)
    }
    
    /// Sequence support for a block of statements
    public func makeIterator() -> IndexingIterator<[Statement]>
    {
        return self.statements.makeIterator()
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
