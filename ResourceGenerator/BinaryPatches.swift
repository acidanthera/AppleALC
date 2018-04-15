//
//  BinaryPatches.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-14.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// A wrapper over an array of `BinaryPatch` to simplify the compilation
public struct BinaryPatches: PropertyListSerializable, Compilable
{
    /// An array of patches
    public let patches: [BinaryPatch]
    
    /// Initialize a collection of binary patches
    ///
    /// - Parameter patches: An array of patches; empty by default.
    public init(patches: [BinaryPatch] = [])
    {
        self.patches = patches
    }
    
    /// Get the name of the `KextPatch` array for this entry conveniently
    public var name: String = "patches" + String.random8()
    
    @discardableResult
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}

// MARK:- IMP: Collection Support
extension BinaryPatches: Collection
{
    public typealias Iterator = IndexingIterator<[BinaryPatch]>
    
    public typealias Element = BinaryPatch
    
    public typealias Index = Int
    
    public func makeIterator() -> Iterator
    {
        return self.patches.makeIterator()
    }
    
    public func index(after i: Index) -> Index
    {
        return self.patches.index(after: i)
    }
    
    public subscript(position: Index) -> Element
    {
        return self.patches[position]
    }
    
    public var startIndex: Index
    {
        return self.patches.startIndex
    }
    
    public var endIndex: Index
    {
        return self.patches.endIndex
    }
}
