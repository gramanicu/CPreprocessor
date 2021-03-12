# Copyright 2021 Grama Nicolae
# The windows makefile

# Compilation variables
CFLAGS = /MD /Wall /Wextra /O2 /std:c11
OBJ = $(SRC:.cpp=bin/.obj)
EXE = so-cpp.exe

# Code Styling
CSFILES = */*.c */*.h */*/.c */*/.h

