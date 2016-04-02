/**
 * @file log.h
 * @brief This file contains all the debug macros.
 * 
 * This debug macros when enabled in debug mode print 
 * the debug messages into the stderr.
 * 
 * @author Prakash
 * @bug No known Bugs.
 */
 
 /**
  * @mainpage Pattern Assignment
  * @section Running the setup
  * @subsection step1 STEP 1:either use make compile(which run the production versiion) or maked debug which runs the debug version.
  * @subsection step2 STEO 2: run ./main to load the program into the main memory.
  * 
  */
#ifndef LOG_H
	#define LOG_H
	#ifdef DEBUG
		#define LOG(msg) fprintf(stderr,"[DEBUG] %s %d: %s",__FILE__,__LINE__,msg)
	#else
		#define LOG(msg)
#endif
#endif
