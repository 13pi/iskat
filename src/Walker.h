#ifndef _WALKER_H
#define _WALKER_H

#include "File.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

struct Walker {

	Walker(std::string const & start, int max_depth, bool fl);
	bool get_next(std::string & path);
	void skip_deep();

private:
	fs::recursive_directory_iterator cur;
	fs::recursive_directory_iterator end;

	bool follow_syml;
	int max_depth;
};

#endif // _WALKER_H