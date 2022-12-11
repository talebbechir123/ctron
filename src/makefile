 # the compiler: gcc for C program, define as g++ for C++
CC = gcc

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -Wno-nullability-completeness  
BUILD_DIR := build
  # the build target executable:
S = Server
C = Client
all: $(S) $(C)

$(S): $(S).c
		$(CC) $(CFLAGS) -o $(S) $(S).c -lncurses
$(C): $(C).c
		$(CC) $(CFLAGS) -o $(C) $(C).c  -lncurses
clean:
		$(RM) -r $(S) $(C) *.dSYM *.o