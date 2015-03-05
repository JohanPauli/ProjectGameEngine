/*
	define NDEBUG to dísable assert and logging for release	
*/
#pragma once

#include <cassert>
#include <iostream>
	
#ifndef NDEBUG
	#define LOG(msg) \
		std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl 
#else
	#define LOG(msg);
#endif