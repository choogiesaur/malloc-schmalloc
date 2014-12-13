#include "malloc.h"
#define malloc( x ) myMalloc( x, __FILE__ , __LINE__ )
#define free( x ) myFree( x, __FILE__, __LINE__ )

void non_alloc_free() 
{
	printf("\n--------------------------------");
	printf("\nBegin **freeing pointer not allocated** test. \n\n");

	printf("You should expect to see a denied free \n\n");

	int x;
	free( x );

	printf("\nEnd **freeing porinter not allocated** test.");
	printf("\n------------------------------\n\n");

}

void non_malloc_free()
{
	printf("\n--------------------------------");
	printf("\nBegin **freeing pointers not returned from malloc** test. \n\n");

	printf("You should expect to see a denied free.\n\n");
	
	int p = (char *)malloc( 200 );
	free( p + 10 );

	printf("\nEnd **freeing pointers not returned from malloc** test.");
	printf("\n------------------------------\n\n");


}

void multi_free() 
{	
	printf("\n--------------------------------");
	printf("\nBegin **redundant freeing** test. \n\n");

	printf("You should expect to see an allocation, a free,");
	printf("and then a denied free.\n\n");
	
	char * p= (char *)malloc( 200 );
	free( p );
	free( p );

	printf("\nEnd **redundant freeing** test.");
	printf("\n------------------------------\n\n");
}

void valid() 
{
	printf("\n--------------------------------");
	printf("\nBegin **perfectly valid** test. \n\n");

	printf("You should expect to see normal allocations.\n\n");	

	char * p = (char *)malloc( 100 ); 
	free( p );
	p = (char *) malloc( 100 ); 
	free( p );

	printf("\nEnd **perfectly valid** test.");
	printf("\n------------------------------\n\n");


}

int main(int argc, char const *argv[])
{
	non_alloc_free();
	non_malloc_free();
	multi_free();
	valid();
	return 0;
}
