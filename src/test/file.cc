#include "test.h"
#include "../File.h"

void file_test() {
	File f("testtt.o");

	std::cout << f.gid() << std::endl;
	std::cout << f.uid() << std::endl;
	std::cout << f.size() << std::endl;
	std::cout << f.type() << std::endl;
	std::cout << std::oct << f.permissions() << std::endl;
}
