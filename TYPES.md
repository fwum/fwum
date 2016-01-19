#Types
##Safer than C
By replacing void* with generics, there is no longer the chance of dereferencing a void* as the wrong type and causing undefined behavior.

##Simpler than Rust
By separating data and behavior, the language decreases the need for complicated polymorphic systems.

##Be as fast as C or C++
Unlike C++, generics in FDL are generated at the end of the compile process, removing the generation of thousands of unnecessary classes to be optimized away later.
