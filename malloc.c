#include "malloc.h"
#include "sorted-list.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

SortedListPtr sl;

void myFree(void * p, char * fn, int ln){
	memEntry *ptr;
	memEntry *before;	//memEntry for the previous block
	memEntry *after;	//memEntry for the next block
    
	if(sl == NULL) {
		printf(KRED "Error: No malloced/calloced memory in the list. Failed to free.\n" KNRM);
		return;
	} if(SLFind(sl, p) == NULL) {
		printf(KRED "Error: This memory pointer was never malloced/calloced in the list. Failed to free.\n" KNRM);
		return;
	}
    
	ptr = (memEntry *)((char *)p - sizeof(memEntry));

	if((before = ptr->prev) != 0 && before->isFree){
		before->size += sizeof(memEntry) + ptr->size;	// Combining with the previous memEntry free block
		before->next  = 	ptr->next;
		ptr->isFree   = 		1;
		before->next  = 	ptr->next;
		if(ptr->next != 0){
			ptr->next->prev = before;
		}

		SLRemove(sl, p);
	} else {   
		if (ptr->isFree == 0) {
		    SLRemove(sl, p);
		    ptr->isFree = 1;
		    before = ptr;
		    printf(KGRN "Freed block 0x%x\n" KNRM, p);
		} else {
			printf(KRED "Error: Attempted to double free a pointer. Failed to free.\n" KNRM);
		}
	}
	if((after = ptr->next) != 0 && after->isFree){
		before->size += sizeof(memEntry) + after->size;	// // Merging with the next memEntry free block
		before->next = after->next;
		before->isFree = 1;
		if(after->next != 0) {
			after->next->prev = before;
		}
		
		SLRemove(sl, p);
	}
}

void * myMalloc(unsigned int size, char * fn, int ln) {
	void 			*ret_ptr;	//Return pointer
	
	memEntry 		*ptr;
	memEntry 		*after;
	
	static memEntry 	*head = 0;	//points to front of memEntry list
	static memEntry 	*tail = 0;	//points to back of memEntry list
    
	ptr = head;
	while(ptr != 0)
	{
		if(ptr->isFree == 0 || (ptr->size < size)) {	//This block is not free or is too small.
			ptr = ptr->next;
		} else if(ptr->size < (size + sizeof(memEntry))) {	//This block is not big enough to cut up.
			ptr->isFree = 0;
			ret_ptr = (char *)ptr + sizeof(memEntry);
			// Adding this block to the list of blocks.
			SLInsert(sl, ret_ptr);
			printf(KGRN "Alloc\'d block 0x%x\n" KNRM, ret_ptr);
			return ret_ptr;
		} else { // Creating the split-up block to come after our newly allocated block.
			after = (memEntry *)((char *)ptr + sizeof(memEntry) + size); //offset from where ptr was; accounts for size of a mementry and the size of the block
			after->prev = ptr;
			after->next = ptr->next;
			if(ptr->next != 0){
				ptr->next->prev = after;
			}
			ptr->next = after;
			after->size = ptr->size - sizeof(memEntry) - size;
			after->isFree = 1;
			ptr->size = size;
			ptr->isFree = 0;
			if (ptr == tail) {
				tail = after;
			}
			ret_ptr = (char *)ptr + sizeof(memEntry);
			// Adding this block to the list of blocks.
			SLInsert(sl, ret_ptr);
			printf(KGRN "Alloc\'d block 0x%x\n" KNRM, ret_ptr);
			return ret_ptr;
		}
	}
	ptr = (memEntry *)sbrk(size + sizeof(memEntry)); //Extending the heap by (size + sizeof(memEntry) bytes if there isn't enough space in any free blocks
	if(ptr == (void *)-1) {
		printf(KRED "Error: Malloc failed in file %s at line %d\n" KNRM, fn, ln);
		return 0;
	} else if(tail == 0) { // tail is null, adds first one
		ptr->prev = 0;
		ptr->next = 0;
		ptr->size = size;
		ptr->isFree = 0;
		head = ptr;
		tail = ptr;
		ret_ptr = (char *)ptr + sizeof(memEntry);
		sl = SLCreate(ptrcmp);
		// Adding this block to the list of blocks.
		SLInsert(sl, ret_ptr);
		printf(KGRN "Alloc\'d block 0x%x\n" KNRM, ret_ptr);
		return ret_ptr;
	} else	{ // otherwise add to existing list
		ptr->prev = tail;
		ptr->next = tail->next;
		ptr->size = size;
		ptr->isFree = 0;
		tail->next = ptr;
		tail = ptr;
		ret_ptr = (char *)ptr + sizeof(memEntry);
		// Adding this block to the list of blocks.
		SLInsert(sl, ret_ptr);
		printf(KGRN "Alloc\'d block 0x%x\n" KNRM, ret_ptr);
		return ret_ptr;
	}
	printf(KRED "Error: Malloc failed in file %s at line %d\n" KNRM, fn, ln);
	return 0;
}

void * myCalloc(unsigned int size, char * fn, int ln){
	void *ptr = myMalloc(size,fn,ln);
	if(ptr == 0) {
		printf(KRED "Error: Calloc failed in file %s at line %d\n" KNRM, fn, ln);
		return 0;
	}
	memset((char *)ptr, 0, size);
	printf(KGRN "Calloc\'d block 0x%x\n" KNRM, ptr);
	return ptr;
}

/*returns a random*/
void * bogo_malloc(char *favorite_color) {
		void *ptr;
		int flibbity[strlen(favorite_color)];
		int floobity[strlen(favorite_color)];
		int i;
		for(i = 0; i < strlen(favorite_color); i++) {
			flibbity[i] = (int)(favorite_color[i]) * 1337;
			floobity[i] = (int)(flibbity[i] / rand());
			ptr += floobity[i];
		}
		if((int)ptr > 10000000){
			int flabbity = ((int) ptr) % 10000000;
			ptr = (void *) flabbity;
		}
		ptr = (void *) rand();
		return ptr;
}

int ptrcmp(void * ptr1, void * ptr2){
	return ptr1 - ptr2;
}

void pretty_colors(){
	int i;
	for(i = 0; i < 20; i++){
		if(i%2 == 0){
			printf(KGRN "~" KNRM, ret_ptr);
		}else{
			printf(KRED "~" KNRM, ret_ptr);
		}
	}
	printf("\n");
	return;
}
