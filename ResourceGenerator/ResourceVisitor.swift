//
//  ResourceVisitor.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// A visitor interface for `Compilable` resources
public protocol ResourceVisitor
{
    /// The return type of visiting result
    associatedtype Result
    
    // MARK:- Codec Lookup Table
    
    func visit(node: CodecLookupTable) -> Result
    
    func visit(node: CodecLookupTable.Entry) -> Result
    
    // MARK:- Kext Table
    
    func visit(node: KextTable) -> Result
    
    func visit(node: KextTable.Info) -> Result
    
    // MARK:- Codec Bundle
    
    func visit(node: [CodecBundles]) -> Result
    
    func visit(node: CodecBundles) -> Result
    
    func visit(node: CodecBundle) -> Result
    
    func visit(node: CodecInfo) -> Result
    
    func visit(node: CodecInfo.Files) -> Result
    
    func visit(node: CodecInfo.Files.File) -> Result
    
    // MARK:- Controller Table
    
    func visit(node: ControllerTable) -> Result
    
    func visit(node: ControllerTable.Entry) -> Result
    
    // MARK:- Binary Patch
    
    func visit(node: BinaryPatches) -> Result
    
    func visit(node: BinaryPatch) -> Result
    
    // MARK:- Revisions
    
    func visit(node: Revisions) -> Result
}
