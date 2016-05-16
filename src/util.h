#ifndef UTIL_H_
#define UTIL_H_
#include <stdlib.h>
//Forward declare a struct
#define DEFSTRUCT(name) struct name; typedef struct name name
//Allocates memory for a variable on the heap
#define new(name) malloc(sizeof(*(name)))
//Creates an array of a certain size on the heap
#define array(name, len) malloc(sizeof(*(name)) * len)
char *string(int length);
#endif
