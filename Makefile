# Copyright 2021 Grama Nicolae
# The windows makefile

# Project directories
SRC_DIR = src/

# Executable name and path
EXE = so-cpp.exe

# Compilation parameters
CC = cl
LINK = link
CFLAGS = /nologo /W4 /MD /D_CRT_SECURE_NO_DEPRECATE
OBJS =src/pair.obj src/list.obj src/hashmap.obj src/main.obj src/cpreprocessor.obj 

# Build the program
build: $(OBJS)
	link /out:$(EXE) $**

# Create the object files
src/main.obj: src/main.c
	$(CC) $(CFLAGS) /Fo$@ /c src/main.c
	
src/cpreprocessor.obj: src/cpreprocessor.c
	$(CC) $(CFLAGS) /Fo$@ /c src/cpreprocessor.c
	
src/pair.obj: src/pair.c
	$(CC) $(CFLAGS) /Fo$@ /c src/pair.c
	
src/list.obj: src/list.c
	$(CC) $(CFLAGS) /Fo$@ /c src/list.c
	
src/hashmap.obj: src/hashmap.c
	$(CC) $(CFLAGS) /Fo$@ /c src/hashmap.c

# Remove object files and executables
clean:
	del $(EXE) $(OBJS)