#include	"malloc.h"
#include   	"sorted-list.h"
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>
#include	<unistd.h>

SortedListPtr sl;
// = SLCreate();

//#define malloc(x) myMalloc(x, __FILE__, __LINE__)
//#define free(x) myFree(x, __FILE__, __LINE__)

void * myMalloc(unsigned int size, char * fn, int ln) {
	void 			*ret_ptr;
	
	memEntry 		*ptr;
	memEntry 		*successor;
	
	static memEntry 	*head = 0; //points to front of memEntry list
	static memEntry 	*tail = 0; //points to back of memEntry list
    
	ptr = head;
	while ( ptr != 0 )
	{
		if ( ptr->size < size ) {	//This block is too small.
			ptr = ptr->next;
		} else if ( !ptr->isFree ) {	//This block is not free.
			ptr = ptr->next;
		} else if ( ptr->size < (size + sizeof(memEntry)) ) {	//This block is not big enough to cut up.
			ptr->isFree = 0;
			ret_ptr = (char *)ptr + sizeof(memEntry);
			
			SLInsert(sl, ret_ptr);
			return ret_ptr;
		} else {
			successor = (memEntry *)((char *)ptr + sizeof(memEntry) + size); //offset from where ptr was; accounts for size of a mementry and the size of the block
			successor->prev = ptr;
			successor->next = ptr->next;
			
			if(ptr->next != 0){
				ptr->next->prev = successor;
			}
			
			ptr->next = successor;
			successor->size = ptr->size - sizeof(memEntry) - size;
			successor->isFree = 1;
			ptr->size = size;
			ptr->isFree = 0;
			tail = (ptr == tail) ? successor : tail;
			ret_ptr = (char *)ptr + sizeof(memEntry);
			
			SLInsert(sl, ret_ptr);
			return ret_ptr;
		}
	}
	if ( (ptr = (memEntry *)sbrk( sizeof(memEntry) + size )) == (void *)-1 ) {
		return 0;
	} else if ( tail == 0 ) { // tail is null, adds first one
		ptr->prev = 0;
		ptr->next = 0;
		ptr->size = size;
		ptr->isFree = 0;
		head = tail = ptr;
		ret_ptr = (char *)ptr + sizeof(memEntry);
		sl = SLCreate(ptrcmp);
		
		SLInsert(sl, ret_ptr);
		return ret_ptr;
	} else	{ // otherwise add to existing list
		ptr->prev = tail;
		ptr->next = tail->next;
		ptr->size = size;
		ptr->isFree = 0;
		tail->next = ptr;
		tail = ptr;
		ret_ptr = (char *)ptr + sizeof(memEntry);
		
		SLInsert(sl, ret_ptr);
		return ret_ptr;
	}
	printf("Error: Malloc failed in file %s at line %d\n", fn, ln);
	return 0;
}

void * myCalloc(unsigned int size, char * fn, int ln){
	void *ptr = myMalloc(size,fn,ln);
	if (ptr == 0) {
		printf("Error: Calloc failed in file %s at line %d\n", fn, ln);
		return 0;
	}
	memset((char *)ptr, 0, size);
	return ptr;
}

void myFree(void * p, char * fn, int ln){
	memEntry *ptr;
	memEntry *before;
	memEntry *after;
    
	if (sl == NULL) {
		printf("No memory was ever malloced. \n");
		return;
	} if(SLFind(sl, p) == NULL) {
		printf("This memory was never malloced\n");
		return;
	}
    
	ptr = (memEntry *)((char *)p - sizeof(memEntry));

	if ( (before = ptr->prev) != 0 && before->isFree ){
		before->size += sizeof(memEntry) + ptr->size;	// merge with before
		
		before->next  = 	ptr->next;
		//begin added
		ptr->isFree = 		1;
		before->next  = 	ptr->next;
		if(ptr->next != 0){
			ptr->next->prev = before;
		}
		//end added
		SLRemove(sl, p);
		//printf( "Freeing block %#x merging with beforeecessor new size is %d.\n", p, before->size );
	}
	else{   
        if (ptr->isFree == 0) {
        	//printf( "BKR freeing block %#x.\n", p );
        	SLRemove(sl, p);
            ptr->isFree = 1;
            before = ptr;
        } else printf("you're double freeing. denied. \n");
	}
	if ( (after = ptr->next) != 0 && after->isFree ){
		before->size += sizeof(memEntry) + after->size;	// merge with successor
		before->next = after->next;
		//begin added
		before->isFree = 1;
        
		if(after->next != 0) {
			after->next->prev = before;
		}
		//end added

		SLRemove(sl, p);
		//printf( "freeing block %#x merging with successor new size is %d.\n", p, before->size );
	}
}

int ptrcmp(void * ptr1, void * ptr2){
	return ptr1 - ptr2;
}
