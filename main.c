#include <stdlib.h>
#include <stdio.h>
//#include "main.h"
#include "memory.h"

int main(int argc, char **argv){
	printf("hello\n");
	
	memBlock *testblock = malloc(sizeof(memBlock));
	init_memBlock(testblock, 100);
	printBlock(testblock);
	
	return 1; //1 if successful
}
