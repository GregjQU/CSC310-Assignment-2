


all: memory

clean:
	rm -rf memory memory.o

memory: memory.c memory.h
	gcc -o memory memory.c -Wall

debug: memory.c memory.h
	gcc -o memory -D DEBUG memory.c -Wall
