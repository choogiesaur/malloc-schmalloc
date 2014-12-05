COMPILER = gcc
CCFLAGS = -g -Wall
all: malloc-schmalloc

debug:
	make DEBUG=TRUE

malloc-schmalloc: main.o memory.o
	$(COMPILER) $(CCFLAGS) -o malloc-schmalloc main.o memory.o

main.o: main.c memory.h
	$(COMPILER) $(CCFLAGS) -c main.c

memory.o: memory.c memory.h
	$(COMPILER) $(CCFLAGS) -c memory.c
		
ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f malloc-shmalloc
	rm -f *.o
