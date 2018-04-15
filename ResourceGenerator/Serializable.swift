//
//  Serializable.swift
//  WWWolfMagic
//
//  Created by FireWolf on 27/07/2017.
//  Revised by FireWolf on 10/04/2018.
//  Copyright © 2017 FireWolf. All rights reserved.
//

import Foundation

/// Represents a type that can be encoded to and decoded from raw data
public protocol Serializable
{
    /// Encode `self` into data
    ///
    /// - Returns: The encoded data on success, `nil` otherwise.
    func encode() -> Data?
    
    /// Encode `self` into a file
    ///
    /// - Parameter file: Save `self` to the `file` path
    /// - Throws: An error if failed to encode `self` or store the encoded data.
    func encode(to file: String) throws
    
    /// Encode `self` into a file
    ///
    /// - Parameter url: Save `self` to the given `url`
    /// - Throws: An error if failed to encode `self` or store the encoded data.
    func encode(to url: URL) throws
    
    /// Decode from the given file
    ///
    /// - Parameter file: A file path to the encoded data
    /// - Returns: An instance on success, `nil` otherwise.
    static func decode(from file: String) -> Self?
    
    /// Decode from the given url
    ///
    /// - Parameter url: A url to the encoded data
    /// - Returns: An instance on success, `nil` otherwise.
    static func decode(from url: URL) -> Self?
    
    /// Decode from the given data
    ///
    /// - Parameter data: The encoded data
    /// - Returns: An instance on success, `nil` otherwise.
    static func decode(from data: Data) -> Self?
}
