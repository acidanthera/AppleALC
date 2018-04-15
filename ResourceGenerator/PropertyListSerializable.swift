//
//  PropertyListSerializable.swift
//  WWWolfMagic
//
//  Created by FireWolf on 27/07/2017.
//  Revised by FireWolf on 10/04/2018.
//  Copyright © 2017 FireWolf. All rights reserved.
//

import Foundation

public extension KeyedDecodingContainer
{
    public func decode<T: Decodable>(_ key: Key) throws -> T
    {
        return try self.decode(T.self, forKey: key)
    }
    
    public func decode<T: Decodable>(_ key: Key, default value: T) throws -> T
    {
        return try self.decodeIfPresent(T.self, forKey: key) ?? value
    }
}

/// Represents a type that can be encoded to and decoded from raw plist data (xml/binary plist)
public protocol PropertyListSerializable: Codable, Serializable
{
    /// Encode `self` into a plist format
    func encode() -> [String : Any]?
    
    /// Decode from the given plist dictionary
    static func decode(from plist: [String : Any]) -> Self?
}

extension PropertyListSerializable
{
    /// Default implementation to encode `self` into data
    public func encode() -> Data?
    {
        let encoder = PropertyListEncoder()
        
        encoder.outputFormat = .xml
        
        return try? encoder.encode(self)
    }
    
    /// Default implementation to encode `self` into a plist
    public func encode() -> [String : Any]?
    {
        guard let encoded: Data = self.encode() else
        {
            return nil
        }
        
        guard let plist = try? PropertyListSerialization.propertyList(from: encoded, options: [], format: nil) else
        {
            return nil
        }
        
        return plist as? [String : Any]
    }
    
    /// Default implementation to encode `self` into a file
    public func encode(to file: String) throws
    {
        return try self.encode(to: URL(fileURLWithPath: file))
    }
    
    /// Default implementation to encode `self` into a file
    public func encode(to url: URL) throws
    {
        let encoder = PropertyListEncoder()
        
        encoder.outputFormat = .xml
        
        try encoder.encode(self).write(to: url)
    }
    
    /// Default implementation to decode from the given plist dictionary
    public static func decode(from plist: [String : Any]) -> Self?
    {
        guard let data = try? PropertyListSerialization.data(fromPropertyList: plist, format: .xml, options: 0) else
        {
            return nil
        }
        
        return self.decode(from: data)
    }
    
    /// Default implemenation to decode from the given data
    public static func decode(from data: Data) -> Self?
    {
        let decoder = PropertyListDecoder()
        
        return try? decoder.decode(self, from: data)
        
//        do
//        {
//            return try decoder.decode(self, from: data)
//        }
//        catch
//        {
//            print("PlistSerializable::decode() Error: \(error)")
//
//            return nil
//        }
    }
    
    /// Default implementation to decode from the given file
    public static func decode(from file: String) -> Self?
    {
        return self.decode(from: URL(fileURLWithPath: file))
    }
    
    /// Default implementation to decode from the given url
    public static func decode(from url: URL) -> Self?
    {
        guard let data = try? Data(contentsOf: url) else
        {
            return nil
        }
        
        return self.decode(from: data)
    }
}
