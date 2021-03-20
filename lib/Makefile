# Copyright 2021 Grama Nicolae
# The windows makefile for the libraries

# Libraries names and object files
LIB_NAME_DATA = libdata.lib
LIB_OBJ_DATA = pair.obj list.obj hashmap.obj

# Compilation parameters
CC = cl
CFLAGS = /nologo /W4 /MD /TC /D_CRT_SECURE_NO_DEPRECATE -/LIBPATH.

all: build_libdata

build_libdata: $(LIB_OBJ_DATA)
	@lib /out:$(LIB_NAME_DATA) $(LIB_OBJ_DATA)
	@rm -f $(LIB_OBJ_DATA)

pair.o: pair/pair.c
	@$(CC) /out:$@ /c $< $(CFLAGS)

list.o: list/list.c
	@$(CC) /out:$@ /c $< $(CFLAGS)

hashmap.o: hashmap/hashmap.c
	@$(CC) /out:$@ /c $< $(CFLAGS)

clean:
	@rm -f $(LIB_OBJ_DATA) $(LIB_NAME_DATA)