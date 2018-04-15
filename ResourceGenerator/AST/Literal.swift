//
//  Literal.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a general literal expression (e.g. array literal, struct literal)
public class Literal: Expression
{
    /// Contents of this literal expression
    /// e.g. An array of integers; Values for struct members
    public internal(set) var contents: [Expression]
    
    /// Initialize an empty literal expression
    public override init()
    {
        self.contents = []
    }
    
    /// Initialize a literal expression using the given contents
    public init(contents: [Expression])
    {
        self.contents = contents
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}

// MARK:- IMP: Collection Support
extension Literal: RandomAccessCollection
{
    public typealias Iterator = IndexingIterator<[Expression]>
    
    public typealias Element = Expression
    
    public typealias Index = Int
    
    public func makeIterator() -> Iterator
    {
        return self.contents.makeIterator()
    }
    
    public func index(after i: Index) -> Index
    {
        return self.contents.index(after: i)
    }
    
    public subscript(position: Index) -> Element
    {
        return self.contents[position]
    }
    
    public var startIndex: Index
    {
        return self.contents.startIndex
    }
    
    public var endIndex: Index
    {
        return self.contents.endIndex
    }
}
