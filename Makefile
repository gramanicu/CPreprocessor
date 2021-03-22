# Copyright 2021 Grama Nicolae
# The windows makefile

# Project directories
SRC_DIR = src/

# Executable name and path
EXE = so-cpp.exe

# Compilation parameters
CC = cl
LINK = link
CFLAGS = /nologo /W4 /MD /TC /D_CRT_SECURE_NO_DEPRECATE
OBJS = src/main.obj src/cpreprocessor.obj src/pair.obj src/list.obj src/hashmap.obj

# Build the program
build: $(OBJS)
	$(LINK) /out:$(EXE) $**

# Create the object files
src/main.obj: src/main.c
	$(CC) /Fe$@ /c src/main.c $(CFLAGS)
	
src/cpreprocessor.obj: src/cpreprocessor.c
	$(CC) /Fe$@ /c src/cpreprocessor.c $(CFLAGS)
	
src/pair.obj: src/pair.c
	$(CC) /Fe$@ /c src/pair.c $(CFLAGS)
	
src/list.obj: src/list.c
	$(CC) /Fe$@ /c src/list.c $(CFLAGS)
	
src/hashmap.obj: src/hashmap.c
	$(CC) /Fe$@ /c src/hashmap.c $(CFLAGS)

# Remove object files and executables
clean:
	del $(EXE) $(OBJS)