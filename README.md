malloc-schmalloc
================

implementation of malloc() and free() with error detection.
Malloc/Free with Error Detection – Amar Bakir and Firas Sattar

Design
This program implements a simulated version of malloc/free with additional error checking. 

We use a memEntry struct to keep logs of all malloc'd pointers. Each memEntry struct records if the block is free, the block size, a pointer to the previous memEntry, and a pointer to the next memEntry.

We kept a SortedList (from PA2) of memEntry structs in ascending order based on pointer values. As we malloc'd new blocks, they were added to this sorted list. This allowed us to check for pointers that were never malloc'd and for pointers that had already been freed (two of the general test cases). If a block was large enough to be split (if it had more than enough room to accommodate the memEntry and data size), we split the block up to leave over an extra free block.

Our free uses the SortedList to check for malloc'd blocks. If it does not exist within the list then the appropriate error is shown on the screen. Similarly, if a block is already free then an error is also displayed to show the function failed. If the block does exist, then this function simply frees it and attempts to merge it with the previous and next blocks if they are free.

Calloc uses malloc to allocate memory and then simply memset the data block to 0. This way all the error checking in malloc is applied to calloc and the added functionality of setting the returned bits is preserved.

EXTRA CREDIT:

calloc() - [5 points]: Our ingenious calloc function calls our even more ingenious malloc function, and then memsets the bytes to zero. Code reusability!

Trying to free() a pointer that's not in the heap [5 points]:
Our free checks the SortedList of memEntry structs, which will contain every pointer that was malloc'd. If the pointer is not in the SortedList, myFree() will refuse to free it.

free()ing some pointer that was not returned by previous malloc()/realloc() call [10 points]: Same as above. It runs through the SortedList and checks if the pointer was actually returned by malloc/calloc before attempting to free.



Using sbrk() to extend the heap [5 points]: 
If there wasn't enough room for the new malloc'd block, we used sbrk to extend the size of the memory to accommodate the new data block and its memEntry struct.

bogo_malloc() - [infinite points]: does some mumbo jumbo and returns a randomish pointer. (*** this is a joke :) ***)
