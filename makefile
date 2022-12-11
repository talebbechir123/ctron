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
SRC = ./src/
BIN = ./bin/
SUBDIRS = src bin
all: $(SRC)$(S).c $(SRC)$(C).c
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC)$(S).c -o $(BIN)$(S) -lncurses
	$(CC) $(CFLAGS) $(SRC)$(C).c -o $(BIN)$(C) -lncurses
clean_all:
	cd $(SRC)  && rm -rf *.dSYM
	cd $(BIN) && rm -rf *.dSYM
clean:
		$(RM) -r $(S) $(C) *.dSYM
