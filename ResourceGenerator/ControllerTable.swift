//
//  ControllerTable.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-06.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a table containing controllers info (Controllers.plist)
public struct ControllerTable: Compilable
{
    /// Represents an entry in this table
    /// Each entry is a piece of controller info
    public struct Entry: PropertyListSerializable, Compilable
    {
        /// All keys used in a piece of controller info
        private enum CodingKeys: String, CodingKey
        {
            case device = "Device"
            
            case description = "Name"
            
            case patches = "Patches"
            
            case vendor = "Vendor"
            
            case model = "Model"
            
            case platform = "Platform"

            case revisions = "Revisions"
        }
        
        /// The device id of the controller (in decimal)
        public let device: Int
        
        /// The descriptive name of the controller
        public let description: String
        
        /// All patches to be applied to the controller
        public let patches: BinaryPatches
        
        /// The vendor of the controller
        public let vendor: Vendor
        
        /// The optional computer model (`any` unless specified)
        public let model: ComputerModel
        
        /// The optional ig-platform-id (in decimal, `0` unless specified)
        public let platform: Int
        
        /// All supported revisions (optional)
        public let revisions: Revisions
        
        /// A custom decoding implementation because the `Model` and `Platform` keys are optional and have default values
        public init(from decoder: Decoder) throws
        {
            let container = try decoder.container(keyedBy: CodingKeys.self)
            
            self.device = try container.decode(.device)
            
            self.description = try container.decode(.description)
            
            // Fallback to an empty array if the patch author does not provide any patches
            self.patches = try BinaryPatches(patches: container.decode(.patches, default: []))
            
            self.vendor = try container.decode(.vendor)
            
            self.model = try container.decode(.model, default: .any)
            
            self.platform = try container.decode(.platform, default: 0)            
            
            self.revisions = try Revisions(revisions: container.decode(.revisions, default: []))
        }
        
        /// Get the number of revisions conveniently
        public var numRevs: Int
        {
            return self.revisions.count
        }
        
        /// Get the number of patches conveniently
        public var numPatches: Int
        {
            return self.patches.count
        }
        
        // MARK: Compilable IMP
        
        public var name: String = "Unused" // unused by the resource compiler
        
        @discardableResult
        public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
        {
            return visitor.visit(node: self)
        }
    }

    /// An array of controller items in this table
    public let entries: [Entry]
    
    /// Decode a controller info table from the given data
    ///
    /// - Parameter data: The contents of a valid `Controllers.plist` file
    public init?(from data: Data)
    {
        let decoder = PropertyListDecoder()
        
        guard let entries = try? decoder.decode([Entry].self, from: data) else
        {
            print("ControllerTable::init() Error: Failed to decode the table from the given data.")
            
            return nil
        }
        
        self.entries = entries
    }
    
    /// Decode a controller info table from the given plist file
    ///
    /// - Parameter plist: The path to a valid `Controllers.plist` file
    public init?(from plist: String)
    {
        self.init(from: URL(fileURLWithPath: plist))
    }
    
    /// Decode a controller info table from the given plist file
    ///
    /// - Parameter plist: The path URL to a valid `Controllers.plist` file
    public init?(from plist: URL)
    {
        guard let data = try? Data(contentsOf: plist) else
        {
            print("ControllerTable::init() Error: Failed to read the given plist file at \"\(plist)\".")
            
            return nil
        }
        
        self.init(from: data)
    }

    // MARK: Compilable IMP
    
    public var name: String = "ADDPR(controllerTable\(String.random8()))"
    
    @discardableResult
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}

// MARK:- IMP: Collection Support
extension ControllerTable: Collection
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
