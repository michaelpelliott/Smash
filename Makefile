# --------------------------------------
# makefile for p4, smash, part 1
# Usage
#    make all      Build the smash executable
#    make clean    Remove the build artifacts
#	 make debug	   Build a debug ready version for gdb
#
# Author
#    Michael Elliott 10/2/2019
# 
# --------------------------------------

# ---- Define names of all the object files in the project
	OBJS = smash.o commands.o history.o

# ---- Define the name of the resulting released product
	EXE = smash

# ---- Define the name of the compiler to be used
	CC = gcc

# ---- Define the options passed by make to the compiler
	CFLAGS = -Wall -std=c99

# Arrange for phony target all to rebuild the dependency file, rules.d
all: rules.d $(EXE)

# phony target for debug
debug: CFLAGS += -g -O0 
debug: rules.d $(EXE)

# phony target for valgrind
valgrind: debug
	valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all $(EXE)
# Rebuild rules.d if it is out of date with any *.c or *.h file using gcc -MM
rules.d: $(wildcard *.c) $(wildcard *.h)
	$(CC) -MM $(wildcard *.c) >rules.d

# incorporate the auto-generated dependencies from rules.d into this Makefile
-include rules.d

# ---- Rules for linking the executable product
$(EXE): $(OBJS)
	$(CC) $(DEBUGFLAGS) $^ -o $@

clean: 
	rm -f $(OBJS) $(EXE) *.d *~
