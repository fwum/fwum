#TODO

##Parser

- Allow nested parens in expressions

- Parse ~~basic control flow such as `if`, `while`, and~~ `for`

- Parse operators with precedence

- Parse export top-level blocks

- Better parser errors

- Better documentation for the parser

- Fix widespread memory leaks


##Semantic Analyzer

- A symbol table type and functions to interact with it

	- Add a symbol

	- Check if a symbol exists

	- Get the type of a symbol

- Check for undeclared variables or functions

- Retrieve type of a given expression

- Type inference for bindings

- Generate errors for mismatched types

##Standard Library

- Basic wrappers around C standard library functions currently used in the compiler

##C Backend

- Conversion of AST to C code
