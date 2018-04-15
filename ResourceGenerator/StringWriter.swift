//
//  StringWriter.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a simple string writer with indentation supported
public class StringWriter: IndentingWriter
{
    /// The internal storage
    private var buffer: String
    
    /// The current indentation level
    private var indentation: Int
    
    /// The current column number
    private var column: Int
    
    /// Initialize a string writer
    public init()
    {
        self.buffer = String()
        
        self.indentation = 0
        
        self.column = 0
    }
    
    /// Get the contents of this writer
    public func toString() -> String
    {
        return self.buffer
    }
    
    // MARK:- IndentingWriter IMP
    
    public func print(_ contents: String)
    {
        if self.column == 0
        {
            for _ in 0..<self.indentation
            {
                self.buffer.append("    ")
            }
            
            self.column = self.indentation * 4
        }
        
        self.buffer.append(contents)
        
        self.column += contents.count
        
        if let lastIndex = contents.range(of: "\n", options: .backwards)?.lowerBound.encodedOffset
        {
            self.column -= lastIndex + 1
        }
    }
    
    public func print(_ contents: CustomStringConvertible)
    {
        self.print(contents.description)
    }
    
    public func println(_ contents: String)
    {
        self.print(contents)
        
        self.buffer.append("\n")
        
        self.column = 0
    }
    
    public func println(_ contents: CustomStringConvertible)
    {
        self.println(contents.description)
    }
    
    public func indent()
    {
        self.indentation += 1
    }
    
    public func outdent()
    {
        self.indentation -= 1
    }
}
