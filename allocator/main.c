/**
 * @file main.c
 * @brief this is the dirver program for the lib functions
 * 
 * this file provides a testcase to call the function in the library
 * and print heap size after some operation for verification.
 * 
 * @author Prakash
 * @bug No Known Bugs
 * 
 */
#include <stdio.h>
#include <stdint.h> 
#include "malloc.h"

/**
 *@brief this main method for the driver program.
 * 
 * In this implementation we have assumed that heap size is 16k which
 * is defined in the malloc.h file and each metadata block takes 24 bytes.
 * First it creates 4 integer memory followed by 200 charactes and 4 integers 
 * using my_calloc.So the total free memory will be
 * 65536-(16*2+200+24*3) =  65232
 * After that c is realloced to 100 bytes which causes a split in the 200 byte block 100 block
 * which is used and and 100 bloc which is free 
 * So the size shhould be 65332 bytes but the actual free space will be
 * 65308 because the 100 byte free block has covered a metadata of 24bytes.
 * Now when free on c is called then it frees the 100 byte block for c and it is
 * merged with the next 100 byte block.So the size now is 65442.
 * Now  when free on a is called it free 16 bytes and it menges with the next 200 byte
 * free block getting the total memory size to 65472 bytes.  
 */
int main()
{
	int i=0;
	int *a = (int *)my_malloc(sizeof(int)*4);
	char *c = (char *)my_malloc(200);
	int *b = (int *)my_calloc(4,sizeof(int));
	
	printf("the intialized values aftyer calloc are:");
	for(i=0; i<4; i++)
		printf("%d\t",b[i]);
	
	printf("\n");
	
	c = (char *)my_realloc(c,100);
	my_free(c);
	
	printf("\nthe free space after the above operatiions are:%d\n",free_space_in_my_heap());
	
	my_free(a);
	
	printf("\nThe free space after these operations are:%d\n",free_space_in_my_heap());
	
	return 0;
	
	
}
