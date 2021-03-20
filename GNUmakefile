# Copyright 2021 Grama Nicolae
# The linux makefile.
# It follows this template - https://gist.github.com/keeferrourke/fe72476a8dd8c4c02ff18eaed74e1de0

# Project directories
SRC_DIR = src/
LIB_DIR = lib/

# Executable name and path
EXE = so-cpp

# Libraries information (build, components)
LIB_MAKE_BUILD = GNUmakefile all
LIB_MAKE_CLEAN = GNUmakefile clean
LIBS = -ldata

# Compilation parameters
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -O2 -std=c89 -I$(LIB_DIR) -DDEBUG
OBJS = src/main.o src/cpreprocessor.o

# Test arguments
TEST_ARGS = -Isrc -I lib -DDEBUG -DDEBUG1=\"ON\" -D LINUX -D LINUX='"TRUE"' -oout.txt in.txt

# Code Styling
CSFILES = src/* lib/*.h lib/*/*.h lib/*/*.c

# Memory check params
MFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

# Build the program
build: build_libs build_app
	$(info Deleting object files and libraries...)
	@$(MAKE) -s -C $(LIB_DIR) -f $(LIB_MAKE_CLEAN)
	@rm -rf $(OBJS)

# Compile the executable
build_app: $(OBJS)
	$(info Building executable...)
	@$(CC) -o $(EXE) $^ $(CFLAGS) $(LIBS) -L$(LIB_DIR)

# Build the libraries
build_libs:
	$(info Building libraries...)
	@$(MAKE) -s -C $(LIB_DIR) -f $(LIB_MAKE_BUILD)
	$(info Building object files...)

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
	@$(MAKE) -s -C $(LIB_DIR) -f $(LIB_MAKE_CLEAN)
	@rm -rf $(EXE) $(OBJS)

# Debuggin makefile
print-% :
	@echo $* = $($*)