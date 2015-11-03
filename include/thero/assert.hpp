#pragma once
#include <iostream>
#include <csignal>

#ifndef SIGTRAP
#define SIGTRAP 5
#endif

#ifndef EMSCRIPTEN
#define TH_HALT raise(SIGTRAP)
#else
#define TH_HALT exit(1)
#endif

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#ifndef NDEBUG
#   define TH_ASSERT(condition, message) \
    do\
    { \
        if(!(condition))\
        { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
            << " function " << __func__ << " line " << __LINE__ << ": " << message << std::endl; \
            TH_HALT; \
        } \
    } while (false)
#else
#   define TH_ASSERT(condition, message) do { } while (false)
#endif

//raise(SIGTRAP);
//std::exit(EXIT_FAILURE);
