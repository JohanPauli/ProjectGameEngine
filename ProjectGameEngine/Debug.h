/*
	possible alternative:
	http://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming

	define NDEBUG to dísable assert and logging for release	
*/
#ifndef __DEBUG_H__
#define __DEBUG_H__

	#include <cassert>
	#include <iostream>
	
	#ifndef NDEBUG
		#define LOG(msg) \
			std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl 
	#else
		#define LOG(msg);
	#endif


#endif