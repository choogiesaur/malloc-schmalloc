/*
 *memory.h
 */
 
struct mem_block {
	int size;
	int isFree;
	struct mem_block *prev;
	struct mem_block *next;
	void *data;
};
typedef struct mem_block memBlock;

int init_memBlock(memBlock *block, int size);

void printBlock(memBlock *block);
