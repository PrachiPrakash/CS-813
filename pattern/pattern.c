/**
 * @file pattern.c
 * @brief this file contain's all the implementations of the pattern operations.
 * 
 * @author Prakash
 * @bug no known bugs.
 */
#include <stdio.h>
#include "pattern.h"
#include "log.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static int get_lcm(int,int);

/**
 * @brief fills all the bytes between the start and end with zeroes.
 * 
 * This function calculates the number of bytes between the start and the end address
 * and assigns the value zero to it.
 * 
 * @param start_addr start address of the sequence.
 * @param end_addr end address of the sequence. 
 * @return void
 */
void init_zero(char *start_addr,char *end_addr)
{
		uint32_t num =0;
		uint32_t i =0;
		if(end_addr < start_addr){
			LOG("init:sned addr smaller then the start addr.");
			return;
		}
		
		num = end_addr - start_addr;
		
		for(i=0; i<=num; i++)
			*(start_addr+i) = 0;
		
		
}
/**
 * @brief fills all the bytes between the start and the end with specified bit pattern
 * 
 * This function calculates the length of the pattern and computes the lcm
 * of 8 and the pattern length to get the byte after which the pattern is repeated.
 *  Then it precomputes the all the bytes ultil which the bit pattern does not get 
 * repeated.After computing the unique bytes then it assigns the complete range with this 
 * bytes using modulo operation's on them.
 * 
 * @param start_addr start address of the sequence.
 * @param end_addr end address of the sequemnce.
 * @return Void
 * 
 */		
void fill_pattern(char *start_addr,char *end_addr,char *pattern)
{
		int len = 0;		/*hold the length of the pattern*/
		int k = 0;			/*hold current pos in the pattern*/
		int i =0,j=0;		/*index variable*/
		int num = 0;		/*number of bytes*/
		int lcm = 0;		/*lcm of 8 and pattern*/
		char *uniq = NULL;
		
		if(end_addr < start_addr){
			LOG("No possible to fill range");
			return;
		}
		
		num = end_addr-start_addr+1;
		len = strlen(pattern);
		lcm = get_lcm(8,len)/8;
		uniq = (char *)malloc(lcm*sizeof(char));
		
		for(i=0; i<lcm; i++){
			*(uniq+i) = 0;
			for(j=0; j<8; j++){
				*(uniq+i) = uniq[i]|((pattern[k]-48)<<(7-j));
				k = (k+1)%len;
			}
			
		}
		
		k = 0;
		
		for(i=0; i<num; i++){
			start_addr[i] = uniq[k];
			k = (k+1)%lcm;
		}
		
		
}
/**
 * @brief return bytes address where the sequence does not match the pattern.
 * 
 * As fill_patern this compute the unique sequence of the pattern and then checks whether
 * all the bytes in the given sequence are following the sequence using the modulo operation.
 * 
 * @param start_addr start address of the sequence.
 * @param end_addr end address of the sequence.
 * @return the byte where the match fails in the sequence. 
 */
char *search_pattern(char *start_addr,char *end_addr,char *pattern)
{
		int len = 0;		/*hold the length of the pattern*/
		int k = 0;			/*hold current pos in the pattern*/
		int i =0,j=0;		/*index variable*/
		int num = 0;		/*number of bytes*/
		char *uniq = NULL;
		int lcm  =0;
		
		if(end_addr < start_addr){
			LOG("No possible to fill range");
			return NULL;
		}
		
		num = end_addr-start_addr+1;
		len = strlen(pattern);
		lcm = get_lcm(8,len)/8;
		uniq = (char *)malloc(lcm*sizeof(char));
		
		for(i=0; i<lcm; i++){
			*(uniq+i) = 0;
			for(j=0; j<8; j++){
				*(uniq+i) = uniq[i]|((pattern[k]-48)<<(7-j));
				k = (k+1)%len;
			}
			
		}
		
		k = 0;
		for(i=0; i<num; i++){
			if(start_addr[i] != uniq[k])
				return start_addr+i;
			k = (k+1)%lcm;
		}
		
		
		
		return NULL;
}
/**
 * @brief this function return the lcm of two numbers using the euclid's algorithm.
 * 
 * This function cannot accessed outside the file.
 * 
 * @param a the first number.
 * @param b the second Number.
 * @return lcm of a and b.
 */

static int get_lcm(int a, int b)
{
	int r = a%b;
	int prod = a * b;
	
	while(r != 0){
			a = b;
			b = r;
			r = a%b;
	}
	
	return prod/b;
}
