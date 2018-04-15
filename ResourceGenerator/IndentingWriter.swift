//
//  IndentingWriter.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a writer type that supports indentation
public protocol IndentingWriter
{
    /// Print the contents
    func print(_ contents: String)
    
    /// Print the contents
    func print(_ contents: CustomStringConvertible)
    
    /// Print the contents with a linefeed
    func println(_ contents: String)
    
    /// Print the contents with a linefeed
    func println(_ contents: CustomStringConvertible)
    
    /// Indent the writer
    func indent()
    
    /// Outdent the writer
    func outdent()
}
