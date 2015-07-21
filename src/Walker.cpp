#include "Walker.h"

#include <unistd.h>

#include <iostream>
#include <cstring>

using std::string;

Walker::Walker(std::string const & start, int md, bool fl) : follow_syml(fl), maxlevel(md), d(NULL) {
	current = start;
	level = 1;
	if (follow_syml) stat(current.c_str(), &st);
	else lstat(current.c_str(), &st);
	if (S_ISDIR(st.st_mode) && (maxlevel > 0 || maxlevel < 0)) {
		if (!eaccess(current.c_str(), R_OK | X_OK))	 {
			parent = current;
			if (parent.at(parent.length() - 1) != '/') parent += "/";
			d = opendir(current.c_str());
		} else {
			std::cerr << PROGRAM_NAME << ": `" 
			<< current 
			<< "' Acccess denied" << std::endl;
		}

	}
}

bool Walker::get_next(File & f) {

	while (level >= 0) {

		if (d == NULL) {
			f = File(current);
			level = -1;
			return true;
		}

		if ((dent = readdir(d))) {
			char const * cur = dent->d_name;
			if (!strcmp(cur, ".") || !strcmp(cur, "..")) continue;

			// create file
			f = File(current);

			if (should_be_visited()) push();
			else current = parent + cur;
			return true;
		} 
		else pop();
	}

	return false;
}

void Walker::pop() {
	closedir(d);
	if (--level) {	
		dent = dent_st.top();
		d = dir_st.top();
		parent = parent_st.top();

		dent_st.pop();
		dir_st.pop();
		parent_st.pop();
	} 
	else d = NULL;

}

void Walker::push() {
	level += 1;
	parent_st.push(parent);
	dir_st.push(d);
	dent_st.push(dent);

	parent += dent->d_name;
	current = parent;
	if (parent.at(parent.length() - 1) != '/') parent += "/";
	d = opendir(parent.c_str());
}

bool Walker::should_be_visited() {
	string full_path;

	if (maxlevel > 0 && level == maxlevel) return false;
	full_path = parent + dent->d_name;
	if (follow_syml) stat(full_path.c_str(), &st);
	else lstat(full_path.c_str(), &st);
	if (S_ISDIR(st.st_mode)) {
		if (!eaccess(full_path.c_str(), R_OK | X_OK )) return true; 
		else {
			std::cerr << PROGRAM_NAME << ": `" 
			<< full_path 
			<< "' Acccess denied" << std::endl;
		}
	}
	return false;
}