COMPILER = gcc
CCFLAGS = -g -Wall
all: malloc-schmalloc

debug:
	make DEBUG=TRUE

#book-orders: book-orders.o customer-database.o consumer-database.o queue.o
#	$(COMPILER) $(CCFLAGS) -o book-orders book-orders.o customer-database.o consumer-database.o queue.o -lpthread

#book-orders.o: book-orders.c book-orders.h
#	$(COMPILER) $(CCFLAGS) -c book-orders.c
	
#customer-database.o: customer-database.c customer-database.h
#	$(COMPILER) $(CCFLAGS) -c customer-database.c

#consumer-database.o: consumer-database.c consumer-database.h
#	$(COMPILER) $(CCFLAGS) -c consumer-database.c

#queue.o: queue.c queue.h
#	$(COMPILER) $(CCFLAGS) -c queue.c

malloc-schmalloc: main.o memory.o
	$(COMPILER) $(CCFLAGS) -o malloc-schmalloc main.o memory.o

main.o: main.c main.h memory.h
	$(COMPILER) $(CCFLAGS) -c main.c

memory.o: memory.c memory.h
	$(COMPILER) $(CCFLAGS) -c memory.c
		
ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f malloc-shmalloc
	rm -f *.o
