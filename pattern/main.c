/**
 * @file main.c 
 * @brief This file contains the driver program to test all the pattern method's.
 * 
 * This driver program creates an static array of 1024 bytes and give the user an
 * menu system to test all the Pattern functions on the 1024 byte array.
 * 
 * @author Prakash
 * @bug No known bugs.
 */
#include <stdio.h>
#include "pattern.h"
#include <stdint.h>
#include <stdlib.h>

char ch[1024];
enum {ZERO_FILL,PATTERN_FILL,PATTERN_SEARCH,EXIT};

int main()
{
	int choice =0;			/*for choice*/
	int byte =0;		/*for no of bytes*/
	char pattern[30];
	char *ind;
	
	do{
		printf("1.Zero Fill\n");
		printf("2.Pattern Fill\n");
		printf("3.Pattern Search\n");
		printf("4.Exit\n");
		
		printf("Enter your choice:");
		scanf("%d",&choice);
		choice--;
		switch(choice){
			
			case ZERO_FILL:
				printf("enter no of bytes in the 1024 byte array:");
				scanf("%d",&byte);
				printf("Zero intialized\n");
				break;
				
			case PATTERN_FILL:
				printf("enter the pattern:");
				scanf("%s",pattern);
				fflush(stdin);
				printf("enetr no of bytes:");
				scanf("%d",&byte);
				fill_pattern(ch,ch+byte-1,pattern);
				printf("filled pattern successfully..\n");
				break;
			
			case PATTERN_SEARCH:
				printf("enter the pattern:");
				scanf("%s",pattern);
				fflush(stdin);
				ind = search_pattern(ch, ch+1023,pattern);
				if(ind == NULL)
					printf("the entire space matches\n");
				else
					printf("the search fails at index %llu\n",(uint64_t)(ind-ch));
				break;
			case EXIT:
				return 0;
			default:
				printf("Invalid choice!!\n");
		}
	}while(choice != EXIT);
}
