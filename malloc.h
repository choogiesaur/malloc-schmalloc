#ifndef MALLOC_H
#define MALLOC_H

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
 * Our free function. Takes a void pointer to a memory chunk to be freed, a character string
 * for the filename, and an int for the line number. Doesn't return anything but will print
 * errors onto the screen if it encounters any.
 */
void myFree(void * p, char * fn, int ln);

/*
 * Malloc function. takes in: int for the size of the requested block (size), string for the filename (fn)
 * and an int for the line # (ln). This function should return the pointer to the data on success and 0
 * on failure.
 */
void * myMalloc(unsigned int size, char * fn, int ln);

/* <<< EXTRA CREDIT!!! >>>
 * This function calls malloc, but with the added functionality of setting all bytes to zero.
 * Returns the pointer to the data on success and 0 on failure.
 */
void * myCalloc(unsigned int size, char * fn, int ln);

/*????????????????????*/
void * bogo_malloc(char *favorite_color);

/*
 * This is a comparator function to be used by our SortedList.
 */
int ptrcmp(void * ptr1, void * ptr2);

#endif
