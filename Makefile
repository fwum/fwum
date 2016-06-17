C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
DBG_FILES := $(addprefix dbg/,$(notdir $(C_FILES:.c=.o)))
LD_FLAGS :=
CC_FLAGS := -Wall -Wfatal-errors -Werror -pedantic -std=c99 -Wextra -Wdouble-promotion -Wunused-parameter -Wunused -Wuninitialized -DDO_TESTS
CC := gcc
run: build
	@bin/./out example.fwum example.c
	@$(CC) example.c -o example
	@./example

example: run
	gcc example.c -o bin/example
	./bin/example

build: obj/ bin/ bin/out

debug: dbg/ bin/dbg
	@gdb bin/dbg

bin/:
	@mkdir bin

obj/:
	@mkdir obj

dbg/:
	@mkdir dbg

clean:
	@rm -r obj/
	@rm -r bin/
	@rm -r dbg/

bin/dbg: $(DBG_FILES)
	@$(CC) $(LD_FLAGS) -g -o $@ $^

bin/out: $(OBJ_FILES)
	@$(CC) $(LD_FLAGS) -o $@ $^

dbg/%.o: src/%.c
	@$(CC) $(CC_FLAGS) -g -c -o $@ $<

obj/%.o: src/%.c
	@$(CC) $(CC_FLAGS) -c -o $@ $<
