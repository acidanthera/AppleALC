//
//  LiteralExpressible.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

/// Represents a type than can be expressed as a literal expression
public protocol LiteralExpressible
{
    var literal: Expression { get }
}
