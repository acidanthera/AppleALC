//
//  PrettyPrinter.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

public class PrettyPrinter: ASTVisitor
{
    /// The return type of PrettyPrinter is Void
    public typealias Result = Void
    
    /// Produce the output to this writer
    private var writer: IndentingWriter
    
    /// Initialize a pretty printer
    ///
    /// - Parameter writer: The writer that accumulates printed contents
    public init(writer: IndentingWriter)
    {
        self.writer = writer
    }
    
    // MARK:- General
    
    public func visit(node: Variable) -> Void
    {
        if node.isStatic
        {
            self.writer.print("static ")
        }
        
        if node.isConstant
        {
            self.writer.print("const ")
        }
        
        node.type.accept(visitor: self)
        
        self.writer.print(" ")
        
        node.identifier.accept(visitor: self)
        
        if node.type is ArrayType
        {
            self.writer.print("[]")
        }
    }
    
    public func visit(node: SourceCode) -> Void
    {
        for statement in node
        {
            statement.accept(visitor: self)
        }
    }
    
    // MARK:- Expressions
    
    public func visit(node: Literal) -> Void
    {
        self.writer.println("{")
        
        self.writer.indent()
        
        for index in 0..<node.count
        {
            node[index].accept(visitor: self)
            
            if index != node.endIndex - 1
            {
                self.writer.println(", ")
            }
        }
        
        self.writer.outdent()
        
        self.writer.println("")
        
        self.writer.print("}")
    }
    
    public func visit(node: BytesLiteral) -> Void
    {
        self.writer.println("{")
        
        self.writer.indent()
        
        for index in 0..<node.count
        {
            node[index].accept(visitor: self)
            
            if index + 1 != node.endIndex
            {
                (index + 1) % 16 == 0 ? self.writer.println(", ") : self.writer.print(", ")
            }
        }
        
        self.writer.outdent()
        
        self.writer.println("")
        
        self.writer.print("}")
    }
    
    public func visit(node: BooleanLiteral) -> Void
    {
        self.writer.print(node.value)
    }
    
    public func visit(node: IntegerLiteral) -> Void
    {
        self.writer.print(node.value)
    }
    
    public func visit(node: HexadecimalIntegerLiteral) -> Void
    {
        self.writer.print("0x" + String(node.value, radix: 16, uppercase: true).leftPadding("0", to: node.width / 4))
    }
    
    public func visit(node: StringLiteral) -> Void
    {
        self.writer.print("\"\(node.content)\"")
    }
    
    public func visit(node: Identifier) -> Void
    {
        self.writer.print(node.name)
    }
    
    public func visit(node: ArrayLookup) -> Void
    {
        node.array.accept(visitor: self)
        
        self.writer.print("[")
        
        node.index.accept(visitor: self)
        
        self.writer.print("]")
    }
    
    public func visit(node: AddressOf) -> Void
    {
        self.writer.print("&")
        
        node.expression.accept(visitor: self)
    }
    
    public func visit(node: NullPointer) -> Void
    {
        self.writer.print("nullptr")
    }
    
    // MARK:- Statements
    
    public func visit(node: Block) -> Void
    {
        for statement in node
        {
            statement.accept(visitor: self)
        }
    }
    
    public func visit(node: Assign) -> Void
    {
        node.variable.accept(visitor: self)
        
        self.writer.print(" = ")
        
        node.value.accept(visitor: self)
        
        self.writer.println(";")
        
        self.writer.println("")
    }
    
    public func visit(node: Include) -> Void
    {
        self.writer.println("#include \"\(node.header)\"")
        
        self.writer.println("")
    }
    
    public func visit(node: CommentLine) -> Void
    {
        self.writer.print("// ")
        
        self.writer.println(node.content)
    }
    
    // MARK:- Types
    
    public func visit(node: ArrayType) -> Void
    {
        node.elementType.accept(visitor: self)
    }
    
    public func visit(node: CharType) -> Void
    {
        self.writer.print("char")
    }
    
    public func visit(node: PointerType) -> Void
    {
        node.pointeeType.accept(visitor: self)
        
        self.writer.print("*")
    }
    
    public func visit(node: SizeType) -> Void
    {
        self.writer.print("size_t")
    }
    
    public func visit(node: StructType) -> Void
    {
        self.writer.print(node.name)
    }
    
    public func visit(node: UIntType) -> Void
    {
        self.writer.print("uint\(node.numBits)_t")
    }
}
