#Top-level
Read until there is a semicolon (TOPLEVEL STATEMENT) or closing bracket (TOPLEVEL DECLARATION)

##Toplevel statement
Check the first word:

- `namespace` = namespace declaration, read until semicolon and strip whitespace as parameter
	- Can only be one per file
	- Must be the first line in a file

- `import` OR `using` = import/using statement, read until semicolon and strip whitespace as parameter
	- Must be after `namespace` declaration (if it appears) and before any TOPLEVEL DECLARATION

##Toplevel Declaration
Check the first word:

- `struct` = go to (STRUCT)

- parse a function
