//
//  CodecInfo.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents the codec info (i.e. ../Resources/<CodecFolder>/Info.plist)
/// (e.g. id, layouts, platforms, patches, etc.)
public struct CodecInfo: PropertyListSerializable, Compilable
{
    /// Records all files provided by patch authors
    /// (e.g. layoutX.xml.zlib and PlatformsX.xml.zlib)
    public struct Files: PropertyListSerializable, Compilable
    {
        /// Represents the layout/platforms file info provided by patch authors
        public struct File: PropertyListSerializable, Compilable
        {
            /// All keys used in a file entry
            private enum CodingKeys: String, CodingKey
            {
                case id = "Id"
                
                case fileName = "Path"
                
                case minKernel = "MinKernel"
                
                case maxKernel = "MaxKernel"
            }
            
            /// The layout id
            public let id: Int
            
            /// The file name of the requested data
            public let fileName: String
            
            /// The minimum kernel version required by this layout/platforms id (`0` unless specified)
            public let minKernel: Int
            
            /// The maximum kernel version required by this layout/platforms id (`0` unless specified)
            public let maxKernel: Int
            
            /// A custom decoding implementation because the `MinKernel` and `MaxKernel` keys are optional and have default values
            public init(from decoder: Decoder) throws
            {
                let container = try decoder.container(keyedBy: CodingKeys.self)
                
                self.id = try container.decode(.id)
                
                self.fileName = try container.decode(.fileName)
                
                self.minKernel = try container.decode(.minKernel, default: 0)
                
                self.maxKernel = try container.decode(.maxKernel, default: 0)
            }
            
            // MARK: Compilable IMP
            
            public var name: String = "file" + String.random8()
            
            @discardableResult
            public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
            {
                return visitor.visit(node: self)
            }
        }
        
        /// Convenient type definition for `Layout`
        public typealias Layout = File
        
        /// Convenient type definition for `Platform`
        public typealias Platform = File
        
        /// All keys used in the files entry
        private enum CodingKeys: String, CodingKey
        {
            case layouts = "Layouts"
            
            case platforms = "Platforms"
        }
        
        /// All layout definitions (i.e. layoutX.xml.zlib)
        public let layouts: [Layout]
        
        /// All platforms definitions (i.e. PlatformsX.xml.zlib)
        public let platforms: [Platform]
        
        /// Custom decoding implementation to provide the fallback support
        public init(from decoder: Decoder) throws
        {
            let container = try decoder.container(keyedBy: CodingKeys.self)
            
            // Although `Layouts` and `Platforms` keys are strictly required,
            // if the patch author forgets to include thess two keys,
            // fallback to empty arrays to avoid errors.
            
            self.layouts = try container.decode(.layouts, default: [])
            
            self.platforms = try container.decode(.platforms, default: [])
        }
        
        /// Get the number of layouts conveniently
        public var numLayouts: Int
        {
            return self.layouts.count
        }
        
        /// Get the number of platforms conveniently
        public var numPlatforms: Int
        {
            return self.platforms.count
        }
        
        // MARK: Compilable IMP
        
        /// Indirectly used by the variable names for `layouts` and `platforms`
        /// so that both variable names have the same suffix
        public var name: String = String.random8()
        
        /// Get the variable name for `layouts` conveniently
        public var nameLayouts: String
        {
            return "layouts" + self.name
        }
        
        /// Ge the variable name for `platforms` conveniently
        public var namePlatforms: String
        {
            return "platforms" + self.name
        }
        
        @discardableResult
        public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
        {
            return visitor.visit(node: self)
        }
    }
    
    /// All keys used in the Info.plist file
    private enum CodingKeys: String, CodingKey
    {
        case vendor = "Vendor"
        
        case codecName = "CodecName"
        
        case id = "CodecID"
        
        case files = "Files"
        
        case patches = "Patches"
        
        case revisions = "Revisions"
    }
    
    /// The vendor of this codec
    public let vendor: Vendor
    
    /// The name of this codec
    public let codecName: String
    
    /// The codec id in decimal
    public let id: Int
    
    /// All files provided by patch authors
    public let files: Files
    
    /// All binary patches applied for this codec
    public let patches: BinaryPatches
    
    /// All supported revisions (optional)
    public let revisions: Revisions
    
    /// Custom decoding implementation to provide the fallback support
    public init(from decoder: Decoder) throws
    {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        
        self.vendor = try container.decode(.vendor)
        
        self.codecName = try container.decode(.codecName)
        
        self.id = try container.decode(.id)
        
        self.files = try container.decode(.files)
        
        // Fallback to an empty array if the patch author does not provide any patches
        self.patches = try BinaryPatches(patches: container.decode(.patches, default: []))
        
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
