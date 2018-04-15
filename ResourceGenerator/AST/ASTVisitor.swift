//
//  ASTVisitor.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// A visitor interface for abstract syntax tree nodes
public protocol ASTVisitor
{
    /// The return type of visiting result
    associatedtype Result
    
    // MARK:- General
    
    func visit(node: Variable) -> Result
    
    func visit(node: SourceCode) -> Result
    
    // MARK:- Expressions
    
    func visit(node: Literal) -> Result
    
    func visit(node: BytesLiteral) -> Result
    
    func visit(node: BooleanLiteral) -> Result
    
    func visit(node: IntegerLiteral) -> Result
    
    func visit(node: HexadecimalIntegerLiteral) -> Result
    
    func visit(node: StringLiteral) -> Result
    
    func visit(node: Identifier) -> Result
    
    func visit(node: ArrayLookup) -> Result
    
    func visit(node: AddressOf) -> Result
    
    func visit(node: NullPointer) -> Result
    
    // MARK:- Statements
    
    func visit(node: Block) -> Result
    
    func visit(node: Assign) -> Result
    
    func visit(node: Include) -> Result
    
    func visit(node: CommentLine) -> Result
    
    // MARK:- Types
    
    func visit(node: ArrayType) -> Result
    
    func visit(node: CharType) -> Result
    
    func visit(node: PointerType) -> Result
    
    func visit(node: SizeType) -> Result
    
    func visit(node: StructType) -> Result
    
    func visit(node: UIntType) -> Result
}
