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

#define BUG printf("%i\n", __LINE__);

void * myMalloc(unsigned int size, char * file, int line){
	static memEntry 	*root = 0;
	static memEntry 	*last = 0;
	memEntry 		*p;
	memEntry 		*succ;
	void 			*ret;
    
	p = root;
	while ( p != 0 )
	{
		if ( p->size < size )
		{
			p = p->next;					// too small
		}
		else if ( !p->isFree )
		{
			p = p->next;					// in use
		}
		else if ( p->size < (size + sizeof(memEntry)) )
		{
			p->isFree = 0;					// too small to chop up
			ret = (char *)p + sizeof(memEntry);
			SLInsert(sl, ret);
			return ret;
		}
		else
		{
			succ = (memEntry *)((char *)p + sizeof(memEntry) + size);
			succ->prev = p;
			succ->next = p->next;
			//p->next->prev = succ;
			//begin add
			if(p->next != 0)
				p->next->prev = succ;
			//end add
			p->next = succ;
			succ->size = p->size - sizeof(memEntry) - size;
			succ->isFree = 1;
			p->size = size;
			p->isFree = 0;
			last = (p == last) ? succ : last;
			ret = (char *)p + sizeof(memEntry);
			SLInsert(sl, ret);
			return ret;
		}
	}
	if ( (p = (memEntry *)sbrk( sizeof(memEntry) + size )) == (void *)-1 )
	{
		return 0;
	}
	else if ( last == 0 )				// first block created
	{
		//printf( "making first chunk size %d\n", size );
		p->prev = 0;
		p->next = 0;
		p->size = size;
		p->isFree = 0;
		root = last = p;
		ret = (char *)p + sizeof(memEntry);
		sl = SLCreate(ptrcmp);
		SLInsert(sl, ret);
		return ret;
	}
	else						// other blocks appended
	{
		//printf( "making another chunk size %d\n", size );
		p->prev = last;
		p->next = last->next;
		p->size = size;
		p->isFree = 0;
		last->next = p;
		last = p;
		ret = (char *)p + sizeof(memEntry);
		SLInsert(sl, ret);
		return ret;
	}
	return 0;
}

void * myCalloc(unsigned int size, char * file, int line){
	void *ptr = myMalloc(size,file,line);
	memset((char *)ptr, 0, size);
	return;
}

void myFree(void * p, char * file, int line){
	memEntry *ptr;
	memEntry *pred;
	memEntry *succ;
    
	if (sl == NULL) {
		printf("No memory was ever malloced. \n");
		return;
	}

	if(SLSearch(sl, p) == NULL) {
		printf("This memory was never malloced\n");
		return;
	}
    
	ptr = (memEntry *)((char *)p - sizeof(memEntry));

	if ( (pred = ptr->prev) != 0 && pred->isFree ){
		pred->size += sizeof(memEntry) + ptr->size;	// merge with predecessor
		
		pred->next  = 	ptr->next;
		//begin added
		ptr->isFree = 		1;
		pred->next  = 	ptr->next;
		if(ptr->next != 0)
			ptr->next->prev = pred;
		//end added
		SLRemove(sl, p);
		//printf( "BKR freeing block %#x merging with predecessor new size is %d.\n", p, pred->size );
	}
	else{   
        if (ptr->isFree == 0) {
        	//printf( "BKR freeing block %#x.\n", p );
        	SLRemove(sl, p);
            ptr->isFree = 1;
            pred = ptr;
        } else printf("you're double freeing. denied. \n");
	}
	if ( (succ = ptr->next) != 0 && succ->isFree ){
		pred->size += sizeof(memEntry) + succ->size;	// merge with successor
		pred->next = succ->next;
		//begin added
		pred->isFree = 1;
        
		if(succ->next != 0) {
			succ->next->prev = pred;
		}
		//end added

		SLRemove(sl, p);
		//printf( "freeing block %#x merging with successor new size is %d.\n", p, pred->size );
	}
}

int ptrcmp(void * ptr1, void * ptr2){
	return ptr1 - ptr2;
}
