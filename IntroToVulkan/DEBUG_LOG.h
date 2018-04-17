#pragma once

#include <iostream>

#if NDEBUG
	
#else
	#define VERBOSE_LOGGING
#endif

#ifdef VERBOSE_LOGGING
#define DEBUG_LOG(x) std::cout << (x) << std::endl
#define DEBUG_LOG_CONTINUOUS(x) std:: cout << (x)
#else
#define DEBUG_LOG(x) 
#define DEBUG_LOG_CONTINUOUS(x) 
#endif
