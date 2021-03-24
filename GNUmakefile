# Copyright 2021 Grama Nicolae
# The linux makefile.
# Project directories

# Executable name and path
EXE = so-cpp

# Libraries information (build, components)
# Compilation parameters
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -O2 -std=c89
OBJS = src/main.o src/cpreprocessor.o src/pair.o src/list.o src/hashmap.o

# Test arguments
TEST_ARGS = -oout.txt in.txt

# Code Styling
CSFILES = src/*

# Memory check params
MFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

# Build the program
build: $(OBJS)
	$(info Building executable...)
	@$(CC) -o $(EXE) $^ $(CFLAGS)
	rm $(OBJS)

# Create the object files
%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

# Run the binary
run: clean build
	./$(EXE) $(TEST_ARGS)
	
# Checks the memory for leaks
memory:clean build
	valgrind $(MFLAGS) ./$(EXE) $(TEST_ARGS)

# Automatic coding style, in my personal style
beauty:
	@cp code_styles/personal .clang-format
	@clang-format -i -style=file $(CSFILES)
	
# Automatic coding style, using the required coding style
beauty_req:
	@cp code_styles/linux .clang-format
	@clang-format -i -style=file $(CSFILES)
	@cp code_styles/personal .clang-format

# Remove object files and executables
clean:
	@rm -rf $(EXE) $(OBJS)

# Debuggin makefile
print-% :
	@echo $* = $($*)

archive: clean beauty_req
	zip -FSr CProcessor.zip ./src Makefile GNUmakefile README.md
	@$(MAKE) -s beauty

check: build
	cp $(EXE) checker/
	@$(MAKE) -s -C checker -f Makefile.checker