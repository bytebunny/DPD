CC = gcc
CFLAGS = -O0 -g -Wall -Wextra #-fsanitize=address -fno-omit-frame-pointer

all: tree.exe

tree.exe: tree.o main.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(wildcard *.o) $(wildcard *.exe)
	$(RM) core $(wildcard core.*)
