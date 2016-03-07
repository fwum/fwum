#ifndef UTIL_H_
#define UTIL_H_
#define DEFSTRUCT(name) struct name; typedef struct name name
#define new(name) malloc(sizeof(*name))
#define array(name, len) malloc(sizeof(*name) * len)
#endif
