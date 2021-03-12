# Copyright 2021 Grama Nicolae
# The linux makefile for the libraries

# Libraries names and object files
LIB_NAME_DATA = libdata.a
LIB_OBJ_DATA = pair.o list.o hashmap.o

# Compilation parameters
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -O2 -std=c99 -I.

all: build_libdata

build_libdata: $(LIB_OBJ_DATA)
	@ar rc $(LIB_NAME_DATA) $(LIB_OBJ_DATA)
	@rm -f $(LIB_OBJ_DATA)

pair.o: pair/pair.c
	@$(CC) -o $@ -c $< $(CFLAGS)

list.o: list/list.c
	@$(CC) -o $@ -c $< $(CFLAGS)

hashmap.o: hashmap/hashmap.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -f $(LIB_OBJ_DATA) $(LIB_NAME_DATA)