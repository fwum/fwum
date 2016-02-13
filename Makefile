C_FILES := $(wildcard src/*.c)
OBJ_FILES := $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
LD_FLAGS :=
CC_FLAGS := -Wall -Werror -pedantic -std=c99
CC := gcc
run: out
	./out example.fwum

debug: dbg
	gdb dbg

clean:
	./rmsafe.sh obj/*
	./rmsafe.sh out*
	./rmsafe.sh output*
	./rmsafe.sh dbg*

dbg: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -g -o $@ $^

out: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c -o $@ $<
