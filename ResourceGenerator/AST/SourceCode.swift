//
//  SourceCode.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents the C++ source code (i.e. kern_resources.cpp)
public class SourceCode: AST, MutableCollection, RangeReplaceableCollection
{
    /// Statements owned by the source code
    private var statements: [Statement]
    
    /// Initialize an empty source code node
    public override required init()
    {
        self.statements = []
    }
    
    // MARK:- Collection IMP
    
    public typealias Iterator = IndexingIterator<[Statement]>

    public typealias Element = Statement
    
    public typealias Index = Int
    
    public var startIndex: Int
    {
        return self.statements.startIndex
    }
    
    public var endIndex: Int
    {
        return self.statements.endIndex
    }
    
    public subscript(position: Int) -> Statement
    {
        get
        {
            return self.statements[position]
        }
        
        set(newValue)
        {
            self.statements[position] = newValue
        }
    }
    
    public func index(after i: Int) -> Int
    {
        return self.statements.index(after: i)
    }
    
    public func makeIterator() -> Iterator
    {
        return self.statements.makeIterator()
    }
    
    public func append(_ newElement: Statement)
    {
        self.statements.append(newElement)
    }
    
    public func append<S>(contentsOf newElements: S) where S : Sequence, SourceCode.Element == S.Element
    {
        self.statements.append(contentsOf: newElements)
    }
    
    public required init<S>(_ elements: S) where S : Sequence, SourceCode.Element == S.Element
    {
        self.statements = elements as! [Element]
    }
    
    // MARK:- Visitor
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
