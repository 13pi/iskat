#ifndef _TEST_H
#define _TEST_H 

#include <cassert>
#include <iostream>
#include <iomanip>

using namespace std;

#define TEST_MODULES \
	MODULE(file)

#define MODULE(name) void name##_test();
TEST_MODULES
#undef MODULE

#endif // _TEST_H