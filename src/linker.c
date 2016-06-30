#include "linker.h"
#include "hashmap.h"
#include "parser.h"
#include "linked_list.h"
#include "slice.h"
#include "util.h"
#include <string.h>
#include <stdio.h>
#include "io.h"

static void continue_build(build *current, char *filename, file_contents newest);
static file_contents get_contents(char *filename);
static char *resolve_name(slice name);

file_contents start_compile(char *file) {
   build current; 
   current.declarations = hm_new_eqfunc(&slice_eq_voidptr);
   current.composite = get_contents(file);
   continue_build(&current, file, current.composite);
   return current.composite;
}

static void continue_build(build *current, char *filename, file_contents newest) {
    slice name_slice = new_slice(filename);
    slice *boxed_slice = new(boxed_slice);
    *boxed_slice = name_slice;
    hm_put(current->declarations, slice_hash(name_slice), boxed_slice, NULL);
    linked_iter iterator = ll_iter_head(newest.imports);
    while(ll_iter_has_next(&iterator)) {
        import_declaration *dec = ll_iter_next(&iterator);
        char *filename = resolve_name(dec->name);
        slice filename_slice = new_slice(filename);
        slice *boxed_filename = new(boxed_filename);
        *boxed_filename = filename_slice;
        if(!hm_has(current->declarations, slice_hash(filename_slice), &(filename_slice))) {
            file_contents contents = get_contents(filename);
            ll_concat(current->composite.structs, contents.structs);
            ll_concat(current->composite.functions, contents.functions);
            hm_put(current->declarations, slice_hash(filename_slice), boxed_filename, NULL);
            continue_build(current, filename, contents);
        }
    }
}

static file_contents get_contents(char *filename) {
    FILE* file = fopen(filename, "r");
    char *contents = read_file(file);
    fclose(file);
    parse_source source = start_parse(contents, filename);
    file_contents parsed = parse(source);
    return parsed;
}

static char *resolve_name(slice name) {
    char *resolved = string(name.len + 5);
    for(int i = 0; i < name.len; i++)
        resolved[i] = name.data[i];
    strcat(resolved + (name.len), ".fwum");
    return resolved;
}
