/**
 * @file pattern.h
 * @brief prototypes for all the pattern function's.
 * 
 * @author Prakash
 * @bug no known bugs
 * 
 */
#ifndef PATTERN_H
#define PATTERN_H

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
void init_zero(char *start_addr,char *end_addr);
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
void fill_pattern(char *start_addr,char *end_addr,char *pattern);
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
char *search_pattern(char *start_addr,char *end_addr,char *pattern);
#endif
