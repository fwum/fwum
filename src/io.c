#include "io.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* read_file(FILE* stream)
{
	char* contents = malloc(sizeof(*contents));
	int spot = 0, length = 0;
	const int CHUNK = 1024;
	char current = fgetc(stream);
	while(current != EOF)
	{
		if(spot == length)
		{
			contents = realloc(contents, length + CHUNK);
			length += CHUNK;
		}
		contents[spot] = current;
		current = fgetc(stream);
		spot += 1;
	}
	contents[spot + 1] = '\0';
	return contents;
}
