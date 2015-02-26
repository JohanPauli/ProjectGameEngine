/*
	original:
	http://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming
*/
#ifndef __DEBUG_H__
#define __DEBUG_H__

	#include <cassert>
	#include <iostream>

#define LOG(msg) \
    std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl 

#endif