#include "c_backend.h"
#include <stdio.h>
#include "parser.h"

static void output_struct(struct_declaration *dec, FILE *stream);

void output(file_contents contents, FILE *stream) {
    linked_iter structs = ll_iter_head(contents.structs);
    while(ll_iter_has_next(&structs)) {
        struct_declaration *dec = ll_iter_next(&structs);
        output_struct(dec, stream);
    }
}

static void output_struct(struct_declaration *dec, FILE *stream) {
    char *name = evaluate(dec->name);
    fprintf(stream, "struct %s; typedef struct %s %s; struct %s {", name, name, name, name);
    linked_iter members = ll_iter_head(dec->members);
    while(ll_iter_has_next(&members)) {
        struct_member *mem = ll_iter_next(&members);
        fprintf(stream, "%s %s;", evaluate(mem->type), evaluate(mem->name));
    }
    fprintf(stream, "};");
}
