//
//  BinaryPatch.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a general binary patch
public struct BinaryPatch: PropertyListSerializable, Compilable
{
    /// Represents an invalid patch error
    public enum Error: Swift.Error
    {
        case invalidPatch
        
        public var localizedDescription: String
        {
            switch self
            {
            case .invalidPatch:
                return "The number of bytes in `Find` and `Replace` must be the same."
            }
        }
    }
    
    /// All keys used in a kext patch
    private enum CodingKeys: String, CodingKey
    {
        case find = "Find"
        
        case replace = "Replace"
        
        case count = "Count"
        
        case executable = "Name"
        
        case minKernel = "MinKernel"
        
        case maxKernel = "MaxKernel"
    }
    
    /// An array of bytes to find
    public let find: Data
    
    /// An array of bytes to replace
    public let replace: Data
    
    /// The number of occurrences of the byte sequence
    public let count: Int
    
    /// The executable name of the kext
    public let executable: String
    
    /// The minimum kernel version required to apply this patch (`0` unless specified)
    public let minKernel: Int
    
    /// The maximum kernel version required to apply this patch (`0` unless specified)
    public let maxKernel: Int
    
    /// A custom decoding implementation because the `MinKernel` and `MaxKernel` keys are optional and have default values
    public init(from decoder: Decoder) throws
    {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        
        self.find = try container.decode(.find)
        
        self.replace = try container.decode(.replace)
        
        self.count = try container.decode(.count)
        
        self.executable = try container.decode(.executable)
        
        self.minKernel = try container.decode(.minKernel, default: 0)
        
        self.maxKernel = try container.decode(.maxKernel, default: 0)
        
        // Guard: The number of bytes in `Find` and `Replace` must be the same
        guard self.find.count == self.replace.count else
        {
            throw Error.invalidPatch
        }
    }
    
    /// Get the size of the patch conveniently
    public var size: Int
    {
        return self.find.count
    }
    
    // MARK: Compilable IMP
    
    public var name: String = "Unused" // unused by the resource compiler
    
    /// Get the variable name of `find` conveniently during compilation
    public var nameFind: String
    {
        return "patchBuffer\(self.find.hashValue)"
    }
    
    /// Get the variable name of `repl` conveniently during compilation
    public var nameRepl: String
    {
        return "patchBuffer\(self.replace.hashValue)"
    }
 
    @discardableResult
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}
