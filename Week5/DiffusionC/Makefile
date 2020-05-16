CC = gcc
MPICC = mpicc
CFLAGS = -g -O0 -Wall -Wextra

all: diffusion.exe diffusion_serial.exe

diffusion.exe: diffusion.o
	$(MPICC) $(CFLAGS) -o $@ $<

diffusion.o: diffusion.c
	$(MPICC) $(CFLAGS) -c -o $@ $<

diffusion_serial.exe: diffusion_serial.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(wildcard *.o) $(wildcard *.exe)
	$(RM) core $(wildcard core.*)
