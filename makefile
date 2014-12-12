COMPILER = gcc
CCFLAGS = -g -Wall
all: malloc-schmalloc

debug:
	make DEBUG=TRUE

malloc-schmalloc: malloc.o testing.o
	$(COMPILER) $(CCFLAGS) -o malloc-schmalloc malloc.o testing.o

malloc.o: malloc.c malloc.h
	$(COMPILER) $(CCFLAGS) -c malloc.c

#
#main.o: main.c memory.h
#	$(COMPILER) $(CCFLAGS) -c main.c

#memory.o: memory.c memory.h
#	$(COMPILER) $(CCFLAGS) -c memory.c

testing.o: testing.c
	$(COMPILER) $(CCFLAGS) -c testing.c
		
ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f malloc-shmalloc
	rm -f *.o
