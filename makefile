COMPILER = gcc
CCFLAGS = -g -Wall

all: mymalloc

test: all 
	./pa6

mymalloc: malloc.o sorted-list.o testing.o
	$(COMPILER) $(CCFLAGS) -o pa6 malloc.o sorted-list.o testing.o

malloc.o: malloc.c malloc.h
	$(COMPILER) $(CCFLAGS) -c malloc.c

sorted-list.o: sorted-list.c sorted-list.h
	$(COMPILER) $(CCFLAGS) -c sorted-list.c

testing.o: testing.c
	$(COMPILER) $(CCFLAGS) -c testing.c

clean:
	rm -f pa6
	rm -f *.o
