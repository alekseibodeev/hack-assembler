# make: build HackAssembler executable program
# make clean: clean-up all built files

# define compiler for C program
CC = gcc

# define the compiler flags
CFLAGS = -Wall -Werror

all: assembler

assembler: main.c assembler.c assembler.h parser.o code.o helpers.o table.o
	$(CC) $(CFLAGS) -o HackAssembler main.c assembler.c parser.o code.o helpers.o table.o

code.o: code.c code.h
	$(CC) $(CFLAGS) -c code.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c helpers.c

table.o: table.c table.h
	$(CC) $(CFLAGS) -c table.c

clean:
	rm HackAssembler *.o
