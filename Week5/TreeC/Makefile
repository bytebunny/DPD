CC = gcc
CFLAGS = -O0 -g -Wall -Wextra #-fsanitize=address -fno-omit-frame-pointer
CPPFLAGS = $(shell pkg-config --cflags cunit)
LDFLAGS = $(shell pkg-config --libs cunit)
DEPS = tree.h tests.h

all: tree.exe

tree.exe: tree.o main.o
	$(CC) $(CFLAGS) -o $@ $^

tests.exe: tests_main.o tests.o tree.o
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(wildcard *.o) $(wildcard *.exe)
	$(RM) core $(wildcard core.*)
