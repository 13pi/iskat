#include "test.h"

int main() {

	// run all tests

	#define MODULE(module) module##_test();
	TEST_MODULES
	#undef MODULE
	return 0;
}