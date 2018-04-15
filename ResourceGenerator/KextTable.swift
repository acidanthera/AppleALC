//
//  KextTable.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-06.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a table containing kexts info (i.e. Kexts.plist)
public struct KextTable: Compilable
{
    /// Represents the detailed info about a kext
    public struct Info: PropertyListSerializable, Compilable
    {
        /// All keys used in a piece of kext info
        private enum CodingKeys: String, CodingKey
        {
            case detect = "Detect"
            
            case identifier = "Id"
            
            case paths = "Paths"
            
            case isReloadable = "Reloadable"
        }
        
        /// An optional switch to detect the kext or not (false by default)
        public let detect: Bool
        
        /// The bundle identifier of the kext
        public let identifier: String
        
        /// Paths to the kext
        public let paths: [String]
        
        /// An optional switch to allow the kext to unload and get patched again (false by default)
        public let isReloadable: Bool
        
        /// A custom decoding implementation because the `Detect` and `Reloadable` keys are optional and have default values
        public init(from decoder: Decoder) throws
        {
            let container = try decoder.container(keyedBy: CodingKeys.self)
            
            self.detect = try container.decode(.detect, default: false)
            
            self.identifier = try container.decode(.identifier)
            
            self.paths = try container.decode(.paths)
            
            self.isReloadable = try container.decode(.isReloadable, default: false)
        }
        
        /// Get the number of paths conveniently
        public var numPaths: Int
        {
            return self.paths.count
        }
        
        // MARK: Compilable IMP
        
        /// Get the name of kext path conveniently
        /// This is used to name the kext executable path during resource compilation
        public var name: String = "kextPath" + String.random8()
        
        @discardableResult
        public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
        {
            return visitor.visit(node: self)
        }
    }
    
    /// Key/Value pairs describing all defined kexts in this table
    public let entries: [String : Info]
    
    /// Decode a kext table from the given data
    ///
    /// - Parameter data: The contents of a valid `Kexts.plist` file
    public init?(from data: Data)
    {
        let decoder = PropertyListDecoder()
        
        guard let entries = try? decoder.decode([String : Info].self, from: data) else
        {
            print("KextTable::init() Error: Failed to decode the table from the given data.")
            
            return nil
        }
        
        self.entries = entries
    }
    
    /// Decode a kext table from the given plist file
    ///
    /// - Parameter plist: The path to a valid `Kexts.plist` file
    public init?(from plist: String)
    {
        self.init(from: URL(fileURLWithPath: plist))
    }
    
    /// Decode a kext table from the given plist file
    ///
    /// - Parameter plist: The path URL to a valid `Kexts.plist` file
    public init?(from plist: URL)
    {
        guard let data = try? Data(contentsOf: plist) else
        {
            print("KextTable::init() Error: Failed to read the given plist file at \"\(plist.path)\".")
            
            return nil
        }
        
        self.init(from: data)
    }
    
    // MARK: Compilable IMP

    public var name: String = "ADDPR(kextTable\(String.random8()))"
    
    @discardableResult
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}

// MARK:- IMP: Collection Support
extension KextTable: Collection
{
    public typealias Iterator = DictionaryIterator<String, Info>
    
    public typealias Element = (key: String, value: Info)
    
    public typealias Index = Dictionary<String, Info>.Index
    
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
    
    public func makeIterator() -> Iterator
    {
        return self.entries.makeIterator()
    }
}
