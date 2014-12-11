#ifndef MALLOC_H
#define MALLOC_H

/*
 * Memory entry struct. Holds the size of the block, whether it is free or not, and pointers
 * to the next and the previous memory entry structs.
 */
struct mem_entry {
    unsigned int size;
    unsigned int free;
    struct mem_entry *prev;
    struct mem_entry *next;
};
typedef struct mem_entry memEntry;

/*
 * Our malloc function. Takes an int containing the size of the chunk to be allocated, a character
 * string for the filename, and an int for the line number. Returns a void pointer on success and
 * NULL on failure.
 */
void * myMalloc(unsigned int size, char * fn, int ln);

/*
 * 
 */
void * myCalloc(unsigned int size, char * file, int line);

/*
 * Our free function. Takes a void pointer to a memory chunk to be freed, a character string
 * for the filename, and an int for the line number. Doesn't return anything but will print
 * errors onto the screen if it encounters any.
 */
void myFree(void * p, char * fn, int ln);

#endif
