#TODO

##Parser

- Parse ~~basic control flow such as `if`, `while`, and~~ `for`

- Parse operators with precedence

- Parse export top-level blocks

- Better parser errors

- Better documentation for the parser

- Fix widespread memory leaks


##Semantic Analyzer

- Check for undeclared variables or functions

- Retrieve type of a given expression

- Type inference for bindings

- Generate errors for mismatched types

##Standard Library

- Basic wrappers around C standard library functions currently used in the compiler

##General Backend

- Parse AST back from a string

- Create object files with manifests of the contents

- Retrieve object files on compiler search path

##C Backend

- Conversion of AST to C code

##Tests

- Basic unit tests for the parser

- Basic unit tests for the symbol table
