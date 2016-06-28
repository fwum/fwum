C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
DBG_FILES := $(addprefix dbg/,$(notdir $(C_FILES:.c=.o)))
LD_FLAGS :=
CC_FLAGS := -Wall -Wfatal-errors -Werror -pedantic -std=c99 -Wextra -Wdouble-promotion -Wunused-parameter -Wunused -Wuninitialized
CC := gcc

run: bin/ obj/ bin/out
	@bin/./out example.fwum example.c
	@gcc example.c -o example

test: clean add_tests bin/ obj/ bin/out
	@bin/./out example.fwum example.c

add_tests:
	$(eval CC_FLAGS := $(CC_FLAGS) -DDO_TESTS)

debug:bin/ dbg/ bin/dbg
	@gdb bin/dbg

bin/:
	@mkdir bin

obj/:
	@mkdir obj

dbg/:
	@mkdir dbg

clean:
	@rm -rf obj/
	@rm -rf bin/
	@rm -rf dbg/

bin/dbg: $(DBG_FILES)
	@$(CC) $(LD_FLAGS) -g -o $@ $^

bin/out: $(OBJ_FILES)
	@$(CC) $(LD_FLAGS) -o $@ $^

dbg/%.o: src/%.c
	@$(CC) $(CC_FLAGS) -g -c -o $@ $<

obj/%.o: src/%.c
	@$(CC) $(CC_FLAGS) -c -o $@ $<
