//
//  CodecBundle.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-12.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a codec bundle in `Resources`
public struct CodecBundle: Compilable
{
    /// The `Info.plist` inside this codec bundle
    public let info: CodecInfo
    
    /// The bundle URL
    public let url: URL
    
    /// Get the codec vendor
    public var vendor: Vendor
    {
        return self.info.vendor
    }
    
    /// Initialize a codec bundle at the given URL
    ///
    /// - Parameter url: The bundle URL
    /// - Returns: `nil` if the codec bundle at the given URL is not valid or the `Info.plist` is corrupted.
    public init?(url: URL)
    {
        // Guard: The codec bundle must contain a valid `Info.plist` file
        guard let info = CodecInfo.decode(from: url.appendingPathComponent("Info.plist")) else
        {
            print("CodecBundle::init() Error: The codec bundle at \"\(url.path)\" is not valid.")
            
            return nil
        }
        
        self.info = info
        
        self.url = url
    }
    
    // MARK: Compilable IMP
    
    public var name: String = "Unused" // unused by the resource compiler
    
    @discardableResult
    public func accept<T>(visitor: T) -> T.Result where T : ResourceVisitor
    {
        return visitor.visit(node: self)
    }
}
