//
//  CodecLookupTable.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-06.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a table containing the codec lookup info (i.e. CodecLookup.plist)
public struct CodecLookupTable: Compilable
{
    /// Represents an entry in the table
    /// Each entry is a piece of codec lookup info
    public struct Entry: PropertyListSerializable, Compilable
    {
        /// All keys used in the CodecLookup.plist file
        private enum CodingKeys: String, CodingKey
        {
            case comment = "Comment"
            
            case tree = "Tree"
            
            case detect = "Detect"
            
            case numControllers = "controllerNum"
        }
        
        /// Comment on this codec lookup info (optional)
        public let comment: String
        
        /// The IORegistry path tree to the codec
        public let tree: [String]
        
        /// An optional switch to detect the codec or not (false by default)
        public let detect: Bool
        
        /// The number of controllers
        public let numControllers: Int
        
        /// A custom decoding implementation because the `Detect` key is optional and has a default value
        public init(from decoder: Decoder) throws
        {
            let container = try decoder.container(keyedBy: CodingKeys.self)
            
            self.comment = try container.decode(.comment, default: "N/A")
            
            self.tree = try container.decode(.tree)
            
            self.numControllers = try container.decode(.numControllers)
            
            self.detect = try container.decode(.detect, default: false)
        }
        
        /// Get the depth of IORegistryPath tree conveniently
        public var depth: Int
        {
            return self.tree.count
        }
        
        // MARK: Compilable IMP
        
        /// Get the name of IORegistryPath conveniently
        /// This is used to name the controller acpi path during resource compilation
        public var name: String = "tree" + String.random8()
        
        @discardableResult
        public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
        {
            return visitor.visit(node: self)
        }
    }
    
    /// An array of codec lookup entries in this table
    public let entries: [Entry]
    
    /// Decode a codec lookup table from the given data
    ///
    /// - Parameter data: The contents of a valid `CodecLookup.plist` file
    public init?(from data: Data)
    {
        let decoder = PropertyListDecoder()
        
        guard let entries = try? decoder.decode([Entry].self, from: data) else
        {
            print("CodecLookupTable::init() Error: Failed to decode the table from the given data.")
            
            return nil
        }
        
        self.entries = entries
    }
    
    /// Decode a codec lookup table from the given plist file
    ///
    /// - Parameter plist: The path to a valid `CodecLookup.plist` file
    public init?(from plist: String)
    {
        self.init(from: URL(fileURLWithPath: plist))
    }
    
    /// Decode a codec lookup table from the given plist file
    ///
    /// - Parameter plist: The path URL to a valid `CodecLookup.plist` file
    public init?(from plist: URL)
    {
        guard let data = try? Data(contentsOf: plist) else
        {
            print("CodecLookupTable::init() Error: Failed to read the given plist file at \"\(plist.path)\".")
            
            return nil
        }
        
        self.init(from: data)
    }
    
    // MARK: Compilable IMP
    
    public var name: String = "ADDPR(codecLookupTable\(String.random8()))"

    @discardableResult
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}

// MARK:- IMP: Collection Support
extension CodecLookupTable: Collection
{
    public typealias Iterator = IndexingIterator<[Entry]>
    
    public typealias Element = Entry
    
    public typealias Index = Int
    
    public func makeIterator() -> Iterator
    {
        return self.entries.makeIterator()
    }
    
    public func index(after i: Index) -> Index
    {
        return self.entries.index(after: i)
    }
    
    public subscript(position: Index) -> Element
    {
        return self.entries[position]
    }
    
    public var startIndex: Index
    {
        return self.entries.startIndex
    }
    
    public var endIndex: Index
    {
        return self.entries.endIndex
    }
}
