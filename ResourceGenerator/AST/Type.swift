//
//  Type.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a type node in the AST
public class Type: AST
{
    // MARK: Get a type conveniently
    
    /// A map containing all custom struct types created by the frontend
    private static var structTypes = [String : Type]()
    
    /// Get a custom struct type
    ///
    /// - Parameter name: The name of the custom type
    /// - Returns: The requested type.
    public static func structType(for name: String) -> Type
    {
        if let type = self.structTypes[name]
        {
            return type
        }
        
        let newType = StructType(name: name)
        
        self.structTypes[name] = newType
        
        return newType
    }
    
    /// Get a custom struct array type
    ///
    /// - Parameter name: The name of the custom type
    /// - Returns: The requested struct array type.
    public static func structArrayType(for name: String) -> Type
    {
        return ArrayType(elementType: Type.structType(for: name))
    }
    
    /// Get the `uint8_t` type
    public static let uint8Type = UIntType(numBits: 8)
    
    /// Get the `uint16_t` type
    public static let uint16Type = UIntType(numBits: 16)
    
    /// Get the `uint32_t` type
    public static let uint32Type = UIntType(numBits: 32)
    
    /// Get the `size_t` type
    public static let sizeType = SizeType()
    
    /// Get the `char` type
    public static let charType = CharType()
    
    /// Get the `char*` type
    public static let stringType = PointerType(pointeeType: Type.charType)
    
    /// Get the `char*` array type
    public static let stringArrayType = ArrayType(elementType: Type.stringType)
    
    /// Get the `uint8_t` bytes array type
    public static let bytesType = ArrayType(elementType: Type.uint8Type)
}
