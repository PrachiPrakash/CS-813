/**
 * @file malloc.c
 * @brief contains all the functions of the allocator.
 * 
 * This contains all the implementations of the allocators along with the 
 * varouis static utility function whic can only be accessed inside the file.
 * 
 * @author Prakash
 * @bug No Known bugs.
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "log.h"
#include "malloc.h"

static uint8_t heap[MAX_SIZE];	/*!< this is the max sizeof the heap* 64KB by default*/

static Meminfo *head = NULL;	/*!< intial head pointer for the linked list*/


/**
 * @brief checks whether free block of the mentioned size is available or not.
 * @param s size required.
 * @return Meminfo * pointer to the metadata block.
 */
static Meminfo *get_free_block(size_t s);

/**
 * @brief checks whether a block has sufficient memory.
 * @param last last segment.
 * @param s size.
 * @return int true or false.
 */
static int check_for_enough_space(Meminfo *last,size_t s);

/**
 * @brief memrge to memory block.
 * @param left left block to be merged.
 * @param right right block.
 * @return void.
 */
	
static void merge_free_block(Meminfo *left,Meminfo * right);

static uint32_t free_space = MAX_SIZE;	/*!< free size of the heap*/

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
void *my_malloc(size_t size)
{
	Meminfo *temp = NULL;
	Meminfo *next_ptr = NULL;
	Meminfo *split_ptr = NULL;
	
	size_t s = ALIGN(size);
		
	if(s == 0){
		LOG("malloc:called with 0 arg\n");
		return NULL;
	}
	
	if(free_space < s){
		LOG("No Free space available\n");
		return NULL;
	}
		
	if(head == NULL){
		LOG("malloc:first request\n");
		head = (Meminfo *)heap;
		head->size = s;
		head->free = FALSE;
		head->next = NULL;
		
		free_space -= (sizeof(Meminfo)+s); 
		return (uint8_t *)head+sizeof(Meminfo);	
			
	} 
	
	else{
		LOG("malloc:Normal request\n");
		temp = get_free_block(s);	/*check for the free block*/
		
		/*there is no sufficient hole */
		if(temp-> next == NULL && temp->free == FALSE){
			
			/*The last block is not big enough*/
			if(!check_for_enough_space(temp,s)){
				LOG("malloc:out of heap memory\n");
				return NULL;
			}
			
			/*the last block is big enough*/
			else{
				LOG("malloc:there is free space at the last\n");
				temp->next = (Meminfo *)((uint8_t *)temp+sizeof(Meminfo)+temp->size);
				temp->next->size = s;
				temp->next->free = FALSE;
				temp->next->next = NULL;
				free_space -= (sizeof(Meminfo) + s); 
				return (uint8_t *)(temp->next)+sizeof(Meminfo);
			}
		
		}
		/*there is a hole in the list*/
		else{
			
			LOG("There is a free block which is large enough\n");
			next_ptr = temp->next;
			
			temp -> free = FALSE;
			
			/*if it is the last block*/
			if(!next_ptr){
				temp->size = s;
				free_space -= s;
			}
			else if((uint8_t *)next_ptr-(uint8_t *)temp-sizeof(Meminfo)-s >= 32){
				
				LOG("malloc:the block can be split into two blocks");
				
				temp->size = s;
				split_ptr = (Meminfo *)((uint8_t *)temp + sizeof(Meminfo) + s);
				split_ptr -> size = (uint8_t *)next_ptr -(uint8_t *)split_ptr-sizeof(Meminfo);
				split_ptr->next = next_ptr;
				split_ptr->free = TRUE;
				
				temp->next = split_ptr;
				/*following line should be scrutinized*/
				free_space -= (sizeof(Meminfo) + s);
				 
				
						
			}
			/*the block is not big enough to split*/
			else{
				LOG("malloc:the block is not enough to split");
				free_space -= temp->size;
				
			}
			return (uint8_t *)temp+sizeof(Meminfo);
			
		}
		
		
	}
	
}
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
void *my_calloc(size_t num,size_t size)
{
	size_t sz = num*size;
	size_t s = ALIGN(sz);
	
	if(s > free_space){
		LOG("Requested size is grater than available space");
		return NULL;
	}
		
	void *ptr = my_malloc(s);
	
	if(ptr == NULL){
		LOG("No memory available");
		return NULL;
	}
	
	memset(ptr,0,s);
	
	return ptr;
}
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
void my_free(const void *ptr)
{
	Meminfo *list = head;
	Meminfo *prev = NULL;
	
	while(list && ((uint8_t *)list)+sizeof(Meminfo) != ptr){
		prev = list;
		list = list->next;
	} 
	
	if(!list){
		LOG("the pointer was not found in the list\n");
		return;
	}
	
	else if(list->free == TRUE)
		return;
	
	if(list == head && list->next == NULL){
		LOG("only one last block left\n");
		head = NULL;
		free_space = MAX_SIZE;
		return;
	}
	
	LOG("free:found the pointer!!\n");
	free_space += list->size; 
	
	
	/*if the right block is free */
	if(list->next != NULL && list->next->free == TRUE){
		LOG("free: the right block is free");
		merge_free_block(list,list->next);
		
	}
	
	
	/*if the left block is also free*/
	if(prev != NULL && prev->free == TRUE){
		LOG("free:the left block is  free");
		merge_free_block(prev,list);
	}
	
	list->free = TRUE;
}
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

void *my_realloc(void *ptr,size_t size)
{
	size_t s = ALIGN(size);
	Meminfo *list = NULL;
	Meminfo *ref = NULL;
	void *ptr_new = NULL;
	
	if(ptr == NULL){
		LOG("reallopc:called with NULL\n");
		return my_malloc(s);
	}
	
	else if(s == 0){
		LOG("realloc:cvalled with zero parameter");
		my_free(ptr);
		return NULL;
	}
	
	list = head;
	
	while(list && ((uint8_t *)list)+sizeof(Meminfo) != ptr)
		list = list->next;
	
	if(!list){
		LOG("reaclloc:the block not there to realloc");
		return NULL;
	}
	/*when realloc want's less size*/
	else if(s <= list->size){
		
		LOG("realloc:request for less than the original size.\n");
		
		if(list->next != NULL){
			
			if((uint8_t *)(list->next)-((uint8_t *)list+sizeof(Meminfo))-s >= 32){
				LOG("SPLIT the block\n");
				ref = (Meminfo *)((uint8_t *)list+sizeof(Meminfo));
				ref->free = TRUE;
				ref->next = list->next;
				ref->size = (uint8_t *)(list->next)-((uint8_t *)list+sizeof(Meminfo))-s-sizeof(Meminfo);
				list->next =ref;
				free_space = free_space+(list->size -s)-sizeof(Meminfo);
				list->size = s;
				return ptr;
			}
			
			else{
				LOG("NO Split");
				free_space += (list->size-s);
				list->size = s;
				return ptr;
			}
		}
		
		else{
			list->size = s;
			free_space += (list->size-s);
			list->size = s;
			return ptr;
		}
		
	}
	
	else{
		LOG("Demanded More space\n");
		ptr_new = my_malloc(s);
		if(!ptr){
			LOG("No Space\n");
			return ptr;
		}
		else{ 
			memcpy(ptr_new,ptr,s);
			my_free(ptr);
			return ptr_new;
		}
	}
	
	
	
}

/**
 * @brief this function returns the current free memory of the heap excluding the metadata.
 * 
 * @param Void
 * @return uint32_t free size of the heap.
 */
uint32_t free_space_in_my_heap(void)
{
		return free_space;
}
/**
 * @brief checks whether free block of the mentioned size is available or not.
 * @param s size required.
 * @return Meminfo * pointer to the metadata block.
 */
static Meminfo *get_free_block(size_t s)
{
	Meminfo *temp = head;
	Meminfo *prev =  NULL;
	
	while(temp != NULL && (temp->size < s || temp->free == FALSE  )){
		prev = temp;
		temp = temp->next;
	}
	
	/*when there is  no free block*/
	if(temp == NULL){
		LOG("NO FREE BLOCK found\n");
		return prev;
	}
	/*when we get an free block*/
	else{
		LOG("free block found\n");
		return temp;
	}
}
/**
 * @brief checks whether a block has sufficient memory.
 * @param last last segment.
 * @param s size.
 * @return int true or false.
 */
static int check_for_enough_space(Meminfo *last,size_t s)
{
	uint8_t *addr = (uint8_t *)last+2*sizeof(Meminfo)+last->size+s;
	
	if(addr <= ((uint8_t *)head + MAX_SIZE-1))
		return TRUE;
	else
		return FALSE;
} 

/**
 * @brief memrge to memory block.
 * @param left left block to be merged.
 * @param right right block.
 * @return void.
 */

static void merge_free_block(Meminfo *left,Meminfo *right)
{
	left->size += (right->size+sizeof(Meminfo));
	left->next = right->next;
	left->free = TRUE;
	free_space += sizeof(Meminfo);
}

