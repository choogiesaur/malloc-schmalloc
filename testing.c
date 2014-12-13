#include "malloc.h"
#define malloc(x) myMalloc(x, __FILE__ , __LINE__)
#define calloc(x) myCalloc(x, __FILE__, __LINE__)
#define free(x) myFree(x, __FILE__, __LINE__)

void non_alloc_free() {
	printf("\n---------------------------------------------------------------");
	printf("\n***Start non-allocated pointer free test***\n\n");
	printf("Expected output: denied free\n\n");
	int x;
	free(x);
	printf("\n***End non-allocated pointer free test***\n\n");
	printf("\n---------------------------------------------------------------\n\n");

}

void non_malloc_free() {
	printf("\n---------------------------------------------------------------");
	printf("\n***Start non-malloced pointer free test***\n\n");
	printf("Expected output: denied free\n\n");
	int ptr = (char *)malloc(500);
	free(ptr + 50);
	printf("\n***End non-malloced pointer free test***\n\n");
	printf("\n---------------------------------------------------------------\n\n");
}

void double_malloc_free() {	
	printf("\n---------------------------------------------------------------");
	printf("\n***Start double malloc free test***\n\n");
	printf("Expected output: successful malloc, successful free, denied free\n\n");
	char *ptr = (char *)malloc(500);
	free(ptr);
	free(ptr);
	printf("\n***End double malloc free test***\n\n");
	printf("\n---------------------------------------------------------------\n\n");
}

void double_calloc_free() {	
	printf("\n---------------------------------------------------------------");
	printf("\n***Start double calloc free test***\n\n");
	printf("Expected output: successful calloc, successful free, denied free\n\n");
	char *ptr = (char *)calloc(500);
	free(ptr);
	free(ptr);
	printf("\n***End double calloc free test***\n\n");
	printf("\n---------------------------------------------------------------\n\n");
}

void valid_malloc() {
	printf("\n---------------------------------------------------------------");
	printf("\n***Start valid malloc test***\n\n");
	printf("Expected output: successful malloc, successful free, successful malloc, successful free\n\n");
	char * ptr = (char *)malloc(200); 
	free(ptr);
	ptr = (char *)malloc(200); 
	free(ptr);
	printf("\n***End valid malloc test***\n\n");
	printf("\n---------------------------------------------------------------\n\n");
}

void valid_calloc() {
	printf("\n---------------------------------------------------------------");
	printf("\n***Start valid calloc test***\n\n");
	printf("Expected output: successful calloc, list of bytes (all are 0), successful free\n\n");
	char *ptr = (char *)calloc(10);
	int i;
	for (i = 0; i < 10, i++) {
		printf("%d ", *(ptr + i));
	}
	printf("\n");
	free(ptr);
	printf("\n***End valid calloc test***\n\n");
	printf("\n---------------------------------------------------------------\n\n");
}

int main(int argc, char const *argv[]) {
	non_alloc_free();
	non_malloc_free();
	double_malloc_free();
	double_calloc_free();
	valid_malloc();
	valid_calloc();
	return 0;
}
