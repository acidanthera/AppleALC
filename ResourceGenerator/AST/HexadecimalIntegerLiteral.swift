//
//  HexadecimalIntegerLiteral.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a hexadecimal integer literal expression
public class HexadecimalIntegerLiteral: IntegerLiteral
{
    /// The number of bits
    public let width: Int
    
    /// Initialize a hexadecimal integer literal
    ///
    /// - Parameters:
    ///   - value: The integer value
    ///   - width: The number of bits
    public init(value: Int, width: Int = 8)
    {
        self.width = width
        
        super.init(value: value)
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}

/// Convert a byte (UInt8) to a literal expression conveniently
extension UInt8: LiteralExpressible
{
    /// Get the literal expression of this integer
    public var literal: Expression
    {
        return HexadecimalIntegerLiteral(value: Int(self))
    }
}

/// Convert a sequence of bytes (Data) to a literal expression conveniently
extension Data: LiteralExpressible
{
    /// Get the literal expression of the data
    public var literal: Expression
    {
        return BytesLiteral(contents: [UInt8](self).map({ $0.literal }))
    }
}

/// Convert an integer to a hexadecimal integer literal expression conveniently
extension Int
{
    /// Get the hexadecimal integer literal expression of this integer
    public var hexLiteral: Expression
    {
        return HexadecimalIntegerLiteral(value: self, width: 32)
    }
    
    /// Get the int16 hex integer literal expression of this integer
    public var int16HexLiteral: Expression
    {
        return HexadecimalIntegerLiteral(value: self, width: 16)
    }
}
