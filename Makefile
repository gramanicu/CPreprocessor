# Copyright 2021 Grama Nicolae
# The windows makefile

# The values for different values are hardcoded,
# as nmake has less builtin functions (wildcard, etc..). The
# values are taken from the linux makefile.

# Copyright 2021 Grama Nicolae
# The winodws makefile.

# Project directories
SRC_DIR = src/
BIN_DIR = bin/
LIB_DIR = lib/

# Executable name and path
EXE = so-cpp.exe

# Libraries information (build, components)
LIB_MAKE_BUILD = GNUmakefile all
LIB_MAKE_CLEAN = GNUmakefile clean
LIBS = -ldata

# Compilation parameters
CFLAGS = /nologo /W4 /EHsc /Za -I$(LIB_DIR)
OBJS = src/main.obj src/cpreprocessor.obj

# Build the program
build: build_libs build_app
	@$(MAKE) -s -C $(LIB_DIR) -f $(LIB_MAKE_CLEAN)
	@rm -rf $(OBJS)

# Compile the executable
build_app: $(OBJS)
	@mkdir -p $(BIN_DIR)
	@$(CC) -o $(EXE) $^ $(CFLAGS) $(LIBS) -L$(LIB_DIR)

# Build the libraries
build_libs:
	@$(MAKE) -s -C $(LIB_DIR) -f $(LIB_MAKE_BUILD)

# Create the object files
src/main.obj: src/main.c
	@$(CC) -o $@ -c $< $(CFLAGS)

src/main.obj: src/main.c
	@$(CC) -o $@ -c $< $(CFLAGS)



# Remove object files and executables
clean:
	@$(MAKE) -s -C $(LIB_DIR) -f $(LIB_MAKE_CLEAN)
	@rm -rf $(BIN_DIR) $(OBJS)