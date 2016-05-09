#include "io.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Reads a file in its entirety
Intended to be immune to buffer overflows
Will hang forever if given stdin, for example, so don't do that
*/
//TODO: Identify issue with reading from files
char* read_file(FILE* stream) {
	char* contents = new(contents);
	int spot = 0, length = 0;
	const int CHUNK = 1024; //size of the chunk to read at a time
	char current = fgetc(stream);
	while(current != EOF) {
		//Out of space in the array
		if(spot == length) {
			contents = realloc(contents, length + CHUNK);
			length += CHUNK;
		}
		contents[spot] = current;
		current = fgetc(stream);
		spot += 1;
	}
	contents[spot + 1] = '\0'; //Ensure the string is null-terminated
	return contents;
}
