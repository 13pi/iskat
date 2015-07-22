#ifndef _WALKER_H
#define _WALKER_H

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
	bool follow_syml;
	int maxlevel;

	DIR *d;
	struct dirent *dent;
	std::string current;
	std::string parent;
	int level;
	struct stat st;

	std::stack<std::string> parent_st;
	std::stack<DIR*> dir_st;
	std::stack<struct dirent*> dent_st;

	void pop();
	void push();
	bool should_be_visited();

};

#endif // _WALKER_H