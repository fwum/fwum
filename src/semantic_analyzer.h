#ifndef SEMANTIC_ANALYZER_H_
#define SEMANTIC_ANALYZER_H_
#include "parser.h"
/*
Adds types to all variables, ensures all operator use is legal, etc.
*/
void analyze(file_contents contents);
#endif
