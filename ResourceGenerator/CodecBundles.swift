//
//  CodecBundles.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-14.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a collection of `CodecBundle` for a specific vendor
public struct CodecBundles: Compilable
{
    /// The vendor of all bundles
    public let vendor: Vendor
    
    /// An array of codec bundles
    public let bundles: [CodecBundle]
    
    /// Initialize a collection of codec bundles
    ///
    /// - Parameters:
    ///   - vendor: The vendor of all bundles
    ///   - bundles: An array of codec bundles
    /// - Warning: This initializer is private and does not check whether all
    ///            given bundles have the same vendor as the given `vendor`.
    /// - Note: Use `CodecBundles::group(bundles:)` function to group a raw collection of bundles.
    private init(vendor: Vendor, bundles: [CodecBundle])
    {
        self.vendor = vendor
        
        self.bundles = bundles
    }
    
    /// Group the given bundles by the vendor
    ///
    /// - Parameter bundles: An array of codec bundles
    /// - Returns: An array of `CodecBundles` grouped by the vendor.
    ///            All bundles inside a `CodecBundles` are guaranteed to have the same vendor.
    public static func group(bundles: [CodecBundle]) -> [CodecBundles]
    {
        let grouped = [Vendor : [CodecBundle]].init(grouping: bundles, by: { $0.vendor })
        
        return grouped.map({ CodecBundles(vendor: $0.key, bundles: $0.value) })
    }
    
    // MARK: Compilable IMP
    
    private let random: String = String.random8()
    
    public var name: String
    {
        return "codecModBundles\(self.vendor.rawValue)" + random
    }
    
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}

// MARK:- IMP: Collection Support
extension CodecBundles: Collection
{
    public typealias Iterator = IndexingIterator<[CodecBundle]>
    
    public typealias Element = CodecBundle
    
    public typealias Index = Int
    
    public func makeIterator() -> Iterator
    {
        return self.bundles.makeIterator()
    }
    
    public func index(after i: Index) -> Index
    {
        return self.bundles.index(after: i)
    }
    
    public subscript(position: Index) -> Element
    {
        return self.bundles[position]
    }
    
    public var startIndex: Index
    {
        return self.bundles.startIndex
    }
    
    public var endIndex: Index
    {
        return self.bundles.endIndex
    }
}

// MARK:- IMP: Compilable Support for codec bundles of all vendors
extension Array: Compilable where Element == CodecBundles
{
    public var name: String
    {
        return "ADDPR(vendorMods\(String.random8()))"
    }
    
    @discardableResult
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}
