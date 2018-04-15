//
//  CommentLine.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-14.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a comment line (i.e. // This is a comment)
/// Strictly speaking, this should not be a AST node.
/// Just for the convenience of adding a comment.
public class CommentLine: Statement
{
    /// The comment content
    public let content: String
    
    public init(content: String)
    {
        self.content = content
    }
    
    public override func accept<T>(visitor: T) -> T.Result where T : ASTVisitor
    {
        return visitor.visit(node: self)
    }
}
