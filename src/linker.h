#ifndef LINKER_H_
#define LINKER_H_
#include "hashmap.h"
#include "parser.h"
DEFSTRUCT(build);

struct build {
    hash_map *declarations; //string to NULL
    file_contents composite;
};

file_contents start_compile(char *file);
#endif
