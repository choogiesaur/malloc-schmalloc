COMPILER = gcc
CCFLAGS = -g -Wall
all: malloc

debug:
	make DEBUG=TRUE

malloc: malloc.o sorted-list.o testing.o
	$(COMPILER) $(CCFLAGS) -o pa6 malloc.o sorted-list.o testing.o

malloc.o: malloc.c malloc.h
	$(COMPILER) $(CCFLAGS) -c malloc.c

sorted-list.o: sorted-list.c sorted-list.h
	$(COMPILER) $(CCFLAGS) -c sorted-list.c

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
	rm -f pa6
	rm -f *.o
