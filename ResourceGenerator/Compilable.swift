//
//  Compilable.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a resource type that can be compiled to C++ source code
public protocol Compilable
{
    func accept<T: ResourceVisitor>(visitor: T) -> T.Result
    
    /// Get the variable name conveniently during compilation
    var name: String { get }
}
