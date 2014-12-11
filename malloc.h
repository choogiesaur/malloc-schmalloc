#ifndef MALLOC_H
#define MALLOC_H

/*
 *
 */
struct MemEntry {
    unsigned int size;
    unsigned int free;
    struct MemEntry *next, *prev;
};

/*
 *
 */
void * my_malloc( unsigned int size, char * file, int line );

/*
 *
 */
void my_free( void * p, char * file, int line );

#endif
