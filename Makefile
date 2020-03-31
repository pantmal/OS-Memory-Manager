CC=g++

CFLAGS=-c 

all: main

main: main.o Hash.o Semaphores.o SharedMemory.o
	$(CC) -o main main.o Hash.o Semaphores.o SharedMemory.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

Semaphores.o: Semaphores.cpp
	$(CC) $(CFLAGS) Semaphores.cpp

Shared Memory.o: SharedMemory.cpp 
	$(CC) $(CFLAGS) SharedMemory.cpp

clean:
	rm -rf *o main


