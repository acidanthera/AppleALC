//
//  ResourceCompiler.swift
//  ResourceGenerator
//
//  Created by FireWolf on 2018-04-13.
//  Copyright © 2018 FireWolf. All rights reserved.
//

import Foundation

public extension Date
{
    public static var current: String
    {
        let formatter = DateFormatter()
        
        formatter.dateFormat = "yyyy.MM.dd HH:mm:ss"
        
        return formatter.string(from: Date())
    }
}

/// Compiles resources provided by users to the equivalent C++ source file
public class ResourceCompiler
{
    /// The resource header
    public static let headerLines = [
    "                                                            ",
    " kern_resources.cpp                                         ",
    " AppleALC                                                   ",
    "                                                            ",
    " Created by ResourceGenerator on \(Date.current).           ",
    " Copyright © 2016-2018 vit9696. All rights reserved.        ",
    "                                                            ",
    " This is an auto-generated file.                            ",
    " Please avoid any direct modifications.                     ",
    "                                                            \n"]
    
    /// Represents compiler errors
    private enum Error: Swift.Error
    {
        /// Failed to convert the resource code
        case failedToConvert
    }
    
    /// A private helper class to compile all resources into an AST
    private class Visitor: ResourceVisitor
    {
        /// The return type of this visitor is `AST?`
        public typealias Result = AST?
        
        /// Stores the compiled resource
        private var sourceCode: SourceCode
        
        /// Stores the name of the kext table
        private var kextTableName: String
        
        /// Records the index in the `kextTable[]` array
        private var indices: [String : Int]
        
        /// Stores all compiled buffers to avoid unnecessary code during compilation
        /// Some patches may have the same sequence of bytes in `Find`/`Replace`;
        /// Some codec modifications may refer to the same layout/platforms file.
        /// This dictionary stores the hash value of the buffer, providing O(1) time
        /// to check whether a buffer is already registered.
        /// The value entry is left to store additional info such as file buffer name.
        private var buffers: [Int : Any?]
        
        /// Stores the current working bundle directory
        private var workingDirectory: URL
        
        /// Initialize a resource compiler visitor
        ///
        /// - Parameter sourceCode: The source code node that stores the compiled resources
        public init(sourceCode: SourceCode)
        {
            self.sourceCode = sourceCode
            
            self.indices = [:]
            
            self.buffers = [:]
            
            self.kextTableName = ""
            
            self.workingDirectory = URL(fileURLWithPath: NSHomeDirectory())
        }
        
        // MARK: Codec Lookup Table
        
        /// Compile a codec lookup table
        ///
        /// - Parameter node: A codec lookup table containing all codec paths
        /// - Returns: `nil`.
        public func visit(node: CodecLookupTable) -> AST?
        {
            sourceCode.append(CommentLine(content: "MARK:- Codec Lookup Section\n"))
            
            // Compile the IORegistryPath for each codec device
            let literals = node.map() { $0.accept(visitor: self) as! Literal }
            
            // Build the `CodecLookupInfo` array
            sourceCode.append(Assign(type: Type.structArrayType(for: "CodecLookupInfo"), name: node.name, value: Literal(contents: literals)))
            
            // Record the number of elements in the array
            sourceCode.append(Assign.const(type: Type.sizeType, name: "ADDPR(codecLookupTableSize)", value: node.count.literal))
            
            return nil
        }
        
        /// Compile a codec lookup info
        ///
        /// - Parameter node: An entry in the codec lookup table
        /// - Returns: The equivalent `CodecLookupInfo` struct literal expression.
        public func visit(node: CodecLookupTable.Entry) -> AST?
        {
            // static const char* tree[] = { "AppleACPIPCI", "HDAU" };
            self.sourceCode.append(Assign.staticConst(type: Type.stringArrayType, name: node.name, value: Literal(contents: node.tree.map({ $0.literal }))))
            
            return Literal(contents: [node.name.identifier,
                                      node.depth.literal,
                                      node.numControllers.literal,
                                      node.detect.literal])
        }
        
        // MARK: Kext Table
        
        /// Compile a kext table
        ///
        /// - Parameter node: A kext table containing all kexts
        /// - Returns: `nil`
        public func visit(node: KextTable) -> AST?
        {
            sourceCode.append(CommentLine(content: "MARK:- Kexts Section\n"))
            
            self.kextTableName = node.name
            
            self.indices.removeAll()
            
            // Compile the `KextInfo` for each kext and record the kext index in the `kextTable` array
            let literals = node.map()
            {
                entry -> Literal in
                
                self.indices[entry.key] = self.indices.count
                
                return entry.value.accept(visitor: self) as! Literal
            }
            
            // Build the `KextInfo` array
            sourceCode.append(Assign(type: Type.structArrayType(for: "KernelPatcher::KextInfo"), name: node.name, value: Literal(contents: literals)))
            
            // Record the number of elements in the array
            sourceCode.append(Assign.const(type: Type.sizeType, name: "ADDPR(kextTableSize)", value: node.count.literal))
            
            return nil
        }
        
        /// Compile a kext info
        ///
        /// - Parameter node: A kext info
        /// - Returns: The equivalent `KernelPatcher::KextInfo` struct literal expression.
        public func visit(node: KextTable.Info) -> AST?
        {
            // static const char* kextPath[] = { "/S/L/E/AppleHDA.kext/Contents/AppleHDA" };
            self.sourceCode.append(Assign.staticConst(type: Type.stringArrayType, name: node.name, value: Literal(contents: node.paths.map({ $0.literal }))))
            
            return Literal(contents: [node.identifier.literal,
                                      node.name.identifier,
                                      node.numPaths.literal,
                                      Literal(contents: [BooleanLiteral.false, node.isReloadable.literal]),
                                      Literal(contents: [node.detect.literal]),
                                      "KernelPatcher::KextInfo::Unloaded".identifier])
        }
        
        // MARK: Codec Bundle
        
        /// Compile codec bundles for all vendors
        ///
        /// - Parameter node: All codec bundles
        /// - Returns: `nil`
        public func visit(node: [CodecBundles]) -> AST?
        {
            // Compile codec bundles for all vendors
            let literals = node.map({ $0.accept(visitor: self) as! Literal })
            
            sourceCode.append(CommentLine(content: "MARK:- Vendor Section\n"))
            
            // Build the `VendorModInfo` array
            sourceCode.append(Assign(type: Type.structArrayType(for: "VendorModInfo"), name: node.name, value: Literal(contents: literals)))
            
            // Record the number of elements in the array
            sourceCode.append(Assign.const(type: Type.sizeType, name: "ADDPR(vendorModsSize)", value: node.count.literal))
            
            return nil
        }
        
        /// Compile all codec bundles for a vendor
        ///
        /// - Parameter node: A collection of codec bundle of a vendor
        /// - Returns: The equivalent `VendorModInfo` struct literal expression.
        public func visit(node: CodecBundles) -> AST?
        {
            sourceCode.append(CommentLine(content: "MARK:- Codec Section [\(node.vendor.rawValue)]\n"))
            
            // Compile all codec bundles for a vendor
            let literals = node.map({ $0.accept(visitor: self) as! Literal })
            
            // Build the `CodecModInfo` array
            sourceCode.append(Assign.static(type: Type.structArrayType(for: "CodecModInfo"), name: node.name, value: Literal(contents: literals)))
            
            // Build the `VendorModInfo` literal
            return Literal(contents: [node.vendor.rawValue.literal,
                                      node.vendor.id.int16HexLiteral,   // `vendor` is a `uint16_t` type
                                      node.name.identifier,
                                      node.count.literal])
        }
        
        /// Compile a codec modification bundle
        ///
        /// - Parameter node: A bundle that contains the codec modification info
        /// - Returns: The equivalent `CodecModInfo` struct literal expression.
        public func visit(node: CodecBundle) -> AST?
        {
            self.workingDirectory = node.url
            
            return node.info.accept(visitor: self)
        }
        
        /// Compile a codec modification info
        ///
        /// - Parameter node: A codec modification info inside a codec bundle
        /// - Returns: The equivalent `CodecModInfo` struct literal expression.
        public func visit(node: CodecInfo) -> AST?
        {
            // Compile all layout and platform files
            node.files.accept(visitor: self)
            
            // Build the `KextPatch` array for all codec patches
            node.patches.accept(visitor: self)
            
            // Build the revisions array literal
            let revisions = node.revisions.accept(visitor: self) as! Expression
            
            // Build the `CodecModInfo` literal
            return Literal(contents: [node.codecName.literal,
                                      node.id.int16HexLiteral,  // `codec` is a `uint16_t` type
                                      revisions,
                                      node.numRevs.literal,
                                      node.files.namePlatforms.identifier,
                                      node.files.numPlatforms.literal,
                                      node.files.nameLayouts.identifier,
                                      node.files.numLayouts.literal,
                                      node.patches.name.identifier,
                                      node.numPatches.literal])
        }
        
        /// Compile all requested files
        ///
        /// - Parameter node: A files entry inside a codec modification table
        /// - Returns: `nil`.
        public func visit(node: CodecInfo.Files) -> AST?
        {
            // A closure that compiles a `File` to its equivalent `CodecModInfo::File` literal
            let file2Literal: (CodecInfo.Files.File) -> Literal = { $0.accept(visitor: self) as! Literal }
            
            // Compile layout files for each codec
            let layoutLiterals = node.layouts.map(file2Literal)
            
            // Compile platform files for each codec
            let platformLiterals = node.platforms.map(file2Literal)
            
            // Build the `CodecModInfo::File` array for layout files
            sourceCode.append(Assign.staticConst(type: Type.structArrayType(for: "CodecModInfo::File"), name: node.nameLayouts, value: Literal(contents: layoutLiterals)))
            
            // Build the `CodecModInfo::File` array for platform files
            sourceCode.append(Assign.staticConst(type: Type.structArrayType(for: "CodecModInfo::File"), name: node.namePlatforms, value: Literal(contents: platformLiterals)))
            
            return nil
        }
        
        /// Compile the requested file
        ///
        /// - Parameter node: A file entry required by the codec modification
        /// - Returns: The equivalent `CodecModInfo::File` struct literal on success.
        /// - Warning: This method uses a null pointer expression as a fallback if failed to read the requested file.
        public func visit(node: CodecInfo.Files.File) -> AST?
        {
            var numBytes = 0
            
            var bufferName = node.name // variable name of the given file buffer in the source code
            
            // Guard: The requested file is readable
            if let data = try? Data(contentsOf: self.workingDirectory.appendingPathComponent(node.fileName))
            {
                // Guard: Check whether the requested file is already compiled
                if let name = self.buffers[data.hashValue] as? String
                {
                    // Use the previous compiled file buffer
                    bufferName = name
                }
                else
                {
                    // This is a new file buffer
                    sourceCode.append(Assign.staticConst(type: Type.bytesType, name: node.name, value: data.literal))
                    
                    // Save the buffer variable name
                    self.buffers[data.hashValue] = node.name
                }
                
                numBytes = data.count
            }
            else
            {
                // Error: Fallback to a null pointer expression
                sourceCode.append(Assign.staticConst(type: Type.bytesType, name: node.name, value: NullPointer.null))
                
                print("ResourceCompilerVisitor::visit() Error: Failed to read the requested file \"\(node.fileName)\" in \"\(self.workingDirectory.path)\".")
            }
            
            // Build the `CodecModInfo::File` literal
            return Literal(contents: [bufferName.identifier,
                                      numBytes.literal,
                                      node.minKernel.literal,
                                      node.maxKernel.literal,
                                      node.id.literal])
        }
        
        // MARK: Controller Table
        
        /// Compile a controller modification table
        ///
        /// - Parameter node: A controller table
        /// - Returns: `nil`.
        public func visit(node: ControllerTable) -> AST?
        {
            sourceCode.append(CommentLine(content: "MARK:- Controllers Section\n"))
            
            // Compile the `ControllerModInfo` for each controller
            let literals = node.map() { $0.accept(visitor: self) as! Literal }
            
            // Build the `ControllerModInfo` array
            sourceCode.append(Assign(type: Type.structArrayType(for: "ControllerModInfo"), name: node.name, value: Literal(contents: literals)))
            
            // Record the number of elements in the array
            sourceCode.append(Assign.const(type: Type.sizeType, name: "ADDPR(controllerTableSize)", value: node.count.literal))
            
            return nil
        }
        
        /// Compile a controller modification entry
        ///
        /// - Parameter node: An entry inside a controller table
        /// - Returns: The equivalent `ControllerModInfo` struct literal expression.
        public func visit(node: ControllerTable.Entry) -> AST?
        {
            node.patches.accept(visitor: self)
            
            let revisions = node.revisions.accept(visitor: self) as! Expression
            
            // Build the `ControllerModInfo` literal
            return Literal(contents: [node.description.literal,
                                      node.vendor.id.hexLiteral,
                                      node.device.hexLiteral,
                                      revisions,
                                      node.numRevs.literal,
                                      node.platform.literal,
                                      node.model.cppValue.identifier,
                                      node.patches.name.identifier,
                                      node.numPatches.literal])
        }
        
        // MARK: Binary Patch
        
        /// Compile an array of binary patches
        ///
        /// - Parameter node: An array of binary patched wrapped in a `BinaryPatches` instance
        /// - Returns: `nil`.
        public func visit(node: BinaryPatches) -> AST?
        {
            // Compile the `KextPatch` literal for each binary patch
            let literals = node.map() { $0.accept(visitor: self) as! Literal }
            
            // Build the `KextPatch` array
            sourceCode.append(Assign.staticConst(type: Type.structArrayType(for: "KextPatch"), name: node.name, value: Literal(contents: literals)))
            
            return nil
        }
        
        /// Compile a binary patch
        ///
        /// - Parameter node: A binary patch
        /// - Returns: The equivalent `KextPatch` struct literal expression.
        public func visit(node: BinaryPatch) -> AST?
        {
            var findBufferName = node.nameFind
            
            var replBufferName = node.nameRepl
            
            // Guard: Check whether the `Find` buffer is already compiled
            if let name = self.buffers[node.find.hashValue] as? String
            {
                // `Find` buffer is already compiled
                findBufferName = name
            }
            else
            {
                // This is a new `Find` buffer
                sourceCode.append(Assign.staticConst(type: Type.bytesType, name: node.nameFind, value: node.find.literal))
                
                self.buffers[node.find.hashValue] = node.nameFind
            }
            
            // Guard: Check whether the `Repl` buffer is already compiled
            if let name = self.buffers[node.replace.hashValue] as? String
            {
                // `Replace` buffer is already compiled
                replBufferName = name
            }
            else
            {
                // This is a new `Replace` buffer
                sourceCode.append(Assign.staticConst(type: Type.bytesType, name: node.nameRepl, value: node.replace.literal))
                
                self.buffers[node.replace.hashValue] = node.nameRepl
            }
            
            // Retrieve the index in the `kextTable` array
            let array = AddressOf(identifier: self.kextTableName)
            
            guard let index = self.indices[node.executable] else
            {
                fatalError("ResourceCompilerVisitor::visit() Fatal Error: \"\(node.executable)\" should already be registered in the kext table.")
            }
            
            // Build a `KernelPatcher::LookupPatch` literal
            let lookupLiteral = Literal(contents: [ArrayLookup(array: array, index: index),
                                                   findBufferName.identifier,//node.nameFind.identifier,
                                                   replBufferName.identifier,//node.nameRepl.identifier,
                                                   node.size.literal,
                                                   node.count.literal])
            
            // Build a `KextPatch` literal
            return Literal(contents: [lookupLiteral, node.minKernel.literal, node.maxKernel.literal])
        }
        
        // MARK: Revisions
        
        /// Compile an array of revisions
        ///
        /// - Parameter node: An array of revisions wrapped in a `Revisions` instance
        /// - Returns: The identifier expression if the revisions array is not empty, `nullptr` expression otherwise.
        public func visit(node: Revisions) -> AST?
        {
            // Compile the revisions (an identifier expression; nullptr if empty)
            var revisions: Expression = NullPointer.null
            
            if !node.isEmpty
            {
                // static const uint32_t revisions[] = { 0x100918, };
                sourceCode.append(Assign.staticConst(type: ArrayType(elementType: Type.uint32Type), name: node.name, value: Literal(contents: node.map({ $0.hexLiteral }))))
                
                revisions = node.name.identifier
            }
            
            return revisions
        }
    }
    
    /// The working directory that contains valid `CodecLookup.plist`, `Controllers.plist`, `Kexts.plist` and codec bundles
    private let workingDirectory: URL
    
    /// The codecs lookup table defined in the working directory
    private let codecs: CodecLookupTable
    
    /// The controllers table defined in the working directory
    private let controllers: ControllerTable
    
    /// The kexts table defined in the working directory
    private let kexts: KextTable
    
    /// Initialize a resource compiler
    ///
    /// - Parameter workingDirectory: URL of the working directory that contains required resources.
    /// - Returns: `nil` if the compiler fails to decode the required tables in the given directory.
    public init?(workingDirectory: URL)
    {
        guard let codecs = CodecLookupTable(from: workingDirectory.appendingPathComponent("CodecLookup.plist")) else
        {
            print("ResourceCompiler::init() Error: Failed to parse the codecs table.")
            
            return nil
        }
        
        guard let controllers = ControllerTable(from: workingDirectory.appendingPathComponent("Controllers.plist")) else
        {
            print("ResourceCompiler::init() Error: Failed to parse the controllers table.")
            
            return nil
        }
        
        guard let kexts = KextTable(from: workingDirectory.appendingPathComponent("Kexts.plist")) else
        {
            print("ResourceCompiler::init() Error: Failed to parse the kexts table.")
            
            return nil
        }
        
        self.codecs = codecs
        
        self.controllers = controllers
        
        self.kexts = kexts
        
        self.workingDirectory = workingDirectory
    }
    
    /// **(Convenient)** Initialize a resource compiler
    ///
    /// - Parameter workingDirectory: A path to the working directory that contains required resources.
    /// - Returns: `nil` if the compiler fails to decode the required tables in the given directory.
    public convenience init?(workingDirectory path: String)
    {
        self.init(workingDirectory: URL(fileURLWithPath: path))
    }
    
    /// Compile the resources to the given file
    ///
    /// - Parameter path: A path to the destination file.
    /// - Throws: `ResourceCompiler.Error` if failed to compile the resources.
    /// - Note: This method does not throw any error right now. (future)
    public func compile(to path: String) throws
    {
        try self.compile(to: URL(fileURLWithPath: path))
    }
    
    /// Compile the resources to the given file
    ///
    /// - Parameter file: URL of the destination file.
    /// - Throws: `ResourceCompiler.Error` if failed to compile the resources.
    /// - Note: This method does not throw any error right now. (future)
    public func compile(to file: URL) throws
    {
        let sourceCode = SourceCode()
        
        // Import the header
        sourceCode.append(contentsOf: ResourceCompiler.headerLines.map({ CommentLine(content: $0) }))
        
        sourceCode.append(Include(header: "kern_resources.hpp"))
        
        // Create a resource builder
        let builder = Visitor(sourceCode: sourceCode)
        
        self.codecs.accept(visitor: builder)
        
        self.kexts.accept(visitor: builder)
        
        self.parseCodecBundles().accept(visitor: builder)
        
        self.controllers.accept(visitor: builder)
        
        // Write to the file
        guard let data = sourceCode.description.data(using: .utf8) else
        {
            throw Error.failedToConvert
        }
        
        try? data.write(to: file)
    }
    
    /// Parse all codec bundles at the current working directory
    ///
    /// - Returns: Codec bundles grouped by the vendor.
    private func parseCodecBundles() -> [CodecBundles]
    {
        let keys: [URLResourceKey] = [.isDirectoryKey, .isPackageKey]
        
        let urls = try! FileManager.default.contentsOfDirectory(at: self.workingDirectory, includingPropertiesForKeys: keys, options: .skipsHiddenFiles).filter()
        {
            url -> Bool in
            
            // Filter out all non codec bundles as much as possible
            let values = try! url.resourceValues(forKeys: Set(keys))
            
            return values.isDirectory! && !values.isPackage!
        }
        
        let bundles = urls.compactMap({ CodecBundle(url: $0) })
        
        return CodecBundles.group(bundles: bundles)
    }
}
