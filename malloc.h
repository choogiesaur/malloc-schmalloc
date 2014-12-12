#ifndef MALLOC_H
#define MALLOC_H

#define MEMSIZE 5000

struct mem_block {
    int size;
    char data[MEMSIZE];
};
typedef struct mem_block memBlock;

/*
 * Memory entry struct. Holds the size of the block, whether it is free or not, and pointers
 * to the next and the previous memory entry structs.
 */
struct mem_entry {
    unsigned int isFree;
    unsigned int size;
    struct mem_entry *prev;
    struct mem_entry *next;
};
typedef struct mem_entry memEntry;

/*
 *malloc function. takes in: int for the size of the requested block (size), string for the filename (fn)
 *and an int for the line #. this fxn should return NULL if there is an error.
 */
void * myMalloc(unsigned int size, char * fn, int ln);

/* ~!@#$%^&*()_-+= <<< EXTRA CREDIT!!! >>> =+-_)(*&^%$#@!~
 * this function calls malloc, but with the added functionality of setting all bytes to zero.
 */
void * myCalloc(unsigned int size, char * fn, int ln);

/*
 * Our free function. Takes a void pointer to a memory chunk to be freed, a character string
 * for the filename, and an int for the line number. Doesn't return anything but will print
 * errors onto the screen if it encounters any.
 */
void myFree(void * p, char * fn, int ln);

/*
 *this is a comparator function to be used by our SortedList.
 */
int ptrcmp(void * ptr1, void * ptr2); //might have to change arg names back
#endif
