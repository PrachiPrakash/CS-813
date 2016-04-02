/**
 * @file malloc.h
 * @brief This file contains all the memory operations prototype of my library.
 * 
 * This contains all the prototypes of the memory operation with metadata structure.
 * 
 * @author Prakash
 * @bug no knowe bugs
 */
#ifndef MALLOC_H

#define MALLOC_H
#define MAX_SIZE 65536		/*!< this is the max sizeof the heap*/
#define FALSE 0				/*!< this is set when block is not free*/
#define TRUE 1				/*!< this is set when block is free*/

#define ALIGN(size) ((((size-1)/4)*4)+4) /*!< this is fo aliginig the space for 4 bytes*/

typedef struct Meminfo Meminfo;
/**
 * @brief this structure contains the declaratiuon of the metadata structure.
 */
struct Meminfo{
	size_t size;		/*!< Size of the block*/
	Meminfo *next;		/*!< holds the address the address of the next metadata*/
	int free;			/*!< flag to check whether the  block is free*/

};
/**
 * @brief This function allocates memory from the 64KB Array.
 * 
 * This function allocates memory in the 64KB uint_8 array by keeping track
 * of the metadata block followed by the data block for each request and returns
 * NULL if some error happens or it runs out of memory.if there is free block 
 * which is large enough then it can split the blockk into two depending on the 
 * size of the block that will remain after allocation.
 * 
 * @param size size to be allocated.
 * @return void * pointer to the data block.
 */
void *my_malloc(size_t size);	

/** 
 * @brief this function allocates intialized memory in 64 KB Array.
 * 
 * This function creates the initialized memory using malloc first then
 * using memset to intialize it to zero.
 * 
 * @param num number of elements.
 * @param size size of each element.
 * @return void * pointer to the data block.  
 */
 
void *my_calloc(size_t num,size_t size);
/**
 * @brief This function free the memory allocated by my_malloc and my_calloc
 * 
 * This function frees up the block allocated by the allocation functions.
 * if the address is not a part of this memory it simply does nothing.
 * finally if there are adjacent block's it merges them to from a new block.
 * 
 * @param ptr pointer to the memory.
 * @return Void
 */
void my_free(const void *ptr);
/**
 * @brief This function's reallocates the previous allocated chuck with new size.
 * 
 * This ponter to invalid memory is given it return null because to give the user a
 * a error message that it canot be created.
 *if size 0 is passed free is called.if a size less than the cureent size is called then it is 
 * allocated and if possible the block is splitted.
 * 
 * @param ptr pointer to the data
 * @param size new size.
 * @return Void * pointer to the increased size block or null.
 */
void *my_realloc(void *ptr,size_t size);
/**
 * @brief this function returns the current free memory of the heap excluding the metadata.
 * 
 * @param Void
 * @return uint32_t free size of the heap.
 */
uint32_t free_space_in_my_heap(void);

#endif
