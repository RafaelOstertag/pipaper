#ifndef __DEBUG_HH
#define __DEBUG_HH

#ifndef NDEBUG
#include <iostream>

#define DEBUG_OUT(x) std::cerr << "DEBUG: " << x << "\n"

#else
#define DEBUG_OUT(x) static_cast<void>(0)
#endif

#endif
