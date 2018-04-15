//
//  AST.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-10.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents an abstract syntax tree node
public /* abstract */ class AST: CustomStringConvertible
{
    public func accept<T: ASTVisitor>(visitor: T) -> T.Result
    {
        fatalError("AST::accept() Fatal Error: All non-abstract subclasses must override this method.")
    }
    
    public var description: String
    {
        let writer = StringWriter()
        
        let printer = PrettyPrinter(writer: writer)
        
        self.accept(visitor: printer)
        
        return writer.toString()
    }
}
