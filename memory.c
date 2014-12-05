/*
 *memory.c
 */
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

int init_memBlock(memBlock *block, int size){
	block->size = size;
	block->isFree = 1;
	block->prev = NULL;
	block->next = NULL;
	block->data = NULL;
	return 1; //1 if successful
}

void printBlock(memBlock *block){
	printf("=== PRINTING MEMBLOCK ===\n");
	printf("size: %d\n", block->size);
	printf("isFree: %d\n", block->isFree);
	printf("prev: 0x%lx\n", (long)block->prev);
	printf("next: 0x%lx\n", (long)block->next);
	printf("data: 0x%lx\n", (long)block->data);	
}
