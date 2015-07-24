#include "Walker.h"

#include <unistd.h>

#include <iostream>
#include <cstring>
#include <boost/filesystem.hpp>
#include <stdlib.h>
#include <limits.h>

using std::string;

Walker::Walker(std::string const & start, int md, bool fl) : follow_syml(fl), maxlevel(md), d(NULL) {
    current = start;
    startedFolder = start;
    level = 1;
    if (follow_syml) stat(current.c_str(), &st);
    else lstat(current.c_str(), &st);
    if (S_ISDIR(st.st_mode) && (maxlevel > 0 || maxlevel < 0)) {
        if (!eaccess(current.c_str(), R_OK | X_OK))  {
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
            f = File(current, follow_syml);
            level = -1;
            return true;
        }

        if ((dent = readdir(d))) {
            char const * cur = dent->d_name;
            if (!strcmp(cur, ".") || !strcmp(cur, "..")) continue;

            // if we on root folder  - hide system proc folder
            if (level == 1){
                char resolved_path[PATH_MAX]; 
                realpath(startedFolder.c_str(), resolved_path); 
                if ( !strcmp( resolved_path, "/") && !strcmp(cur, "proc" )) continue;
            }

            

            // create file
            f = File(current, follow_syml);
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
        state_t top = state_st.top();

        parent = top.get<0>();
        d = top.get<1>();
        dent = top.get<2>();

        state_st.pop();
    } 
    else d = NULL;

}

void Walker::push() {
    level += 1;
    state_st.push(boost::make_tuple(parent, d, dent));

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
        else if (eaccess(full_path.c_str(), F_OK)) return false;
        else {
            std::cerr << PROGRAM_NAME << ": `" 
            << full_path 
            << "' Acccess denied" << std::endl;
        }
    }
    return false;
}