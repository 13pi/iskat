#ifndef _WALKER_H
#define _WALKER_H

#include "boost/tuple/tuple.hpp"
#include <stack>
#include "File.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "iskat.h"

struct Walker {

	Walker(std::string const & start, int max_depth, bool fl);
	bool get_next(File&);
	
private:

	typedef boost::tuple<
		std::string,
		DIR*,
		struct dirent* > state_t;

	bool follow_syml;
	int maxlevel;

	DIR *d;
	struct dirent *dent;
	std::string current;
	std::string parent;
	int level;
	struct stat st;
	
	std::stack<state_t> state_st;

	void pop();
	void push();
	bool should_be_visited();

public:
	std::string startedFolder;
};

#endif // _WALKER_H