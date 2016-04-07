C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
LD_FLAGS :=
CC_FLAGS := -Wall -Wfatal-errors -Werror -pedantic -std=c99 -Wextra -Wdouble-promotion -Wunused-parameter -Wunused -Wuninitialized
CC := gcc
run: obj/ bin/ bin/out
	bin/./out example.fwum

debug: bin/dbg
	gdb bin/dbg

bin/:
	mkdir bin

obj/:
	mkdir obj

clean:
	rm -r obj/
	rm -r bin/

bin/dbg: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -g -o $@ $^

bin/out: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<
