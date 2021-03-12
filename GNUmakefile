# Copyright 2021 Grama Nicolae
# The linux makefile

# Compilation variables
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g -O2 -std=c11
SRC = src/main.c
OBJ = $(SRC:.cpp=bin/.o)
EXE = bin/so-cpp
LIB = lib

# Code Styling
CSFILES = */*.c */*.h */*/.c */*/.h

build: $(OBJ)
	@$(CC) -o $(EXE) $^ $(CFLAGS) -I$(LIB) ||:

