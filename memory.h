/*
 *memory.h
 */
 
struct mem_block {
	int size
	struct mem_block *next;
	void *data;
};
typedef struct mem_block memBlock;
