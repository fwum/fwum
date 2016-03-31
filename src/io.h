#ifndef IO_H_
#define IO_H_
#include <stdio.h>
/*
Reads a file in its entirety
Intended to be immune to buffer overflows
Will hang forever if given stdin, for example, so don't do that
*/
char* read_file(FILE* stream);
#endif
