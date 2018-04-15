//
//  main.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-06.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

guard CommandLine.arguments.count == 3 else
{
    print("Usage: ResourceGenerator <ResourceDirectory> <OutputPath>")
    
    exit(-1)
}

let directory = CommandLine.arguments[1]

let output = CommandLine.arguments[2]

print("Working Directory: \(directory)")

print("Output C++ file: \(output)")

guard let compiler = ResourceCompiler(workingDirectory: directory) else
{
    print("Error: Failed to create the resource compiler.")
    
    exit(-1)
}

do
{
    try compiler.compile(to: output)
}
catch
{
    print(error)
}

print("Done")
