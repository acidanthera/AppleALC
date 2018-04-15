//
//  Revisions.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-14.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// A wrapper over an array of `Revision` (= Int) to simplify the compilation
public struct Revisions: PropertyListSerializable, Compilable
{
    /// An array of revisions
    public let revisions: [Int]
    
    public init(revisions: [Int])
    {
        self.revisions = revisions
    }
    
    /// Get the variable name of the revisions array conveniently
    public var name: String = "revisions" + String.random8()
    
    @discardableResult
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}

// MARK:- IMP: Collection Support
extension Revisions: Collection
{
    public typealias Iterator = IndexingIterator<[Int]>
    
    public typealias Element = Int
    
    public typealias Index = Int
    
    public func makeIterator() -> Iterator
    {
        return self.revisions.makeIterator()
    }
    
    public func index(after i: Index) -> Index
    {
        return self.revisions.index(after: i)
    }
    
    public subscript(position: Index) -> Element
    {
        return self.revisions[position]
    }
    
    public var startIndex: Index
    {
        return self.revisions.startIndex
    }
    
    public var endIndex: Index
    {
        return self.revisions.endIndex
    }
}
