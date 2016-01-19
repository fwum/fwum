#Memory Model
##Safer than C
Pointers must either be unique or constant, allowing the language to handle freeing memory without any runtime cost.

Instead, pointers can be moved with `acquire` or dereferenced and copied.

##Simpler than Rust
Moving is not automatic, preventing astonishing behavior for newer or careless users.

##Be as fast as C or C++
All memory checks are done at compile time, meaning there is no runtime cost.
