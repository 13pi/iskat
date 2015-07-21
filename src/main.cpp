#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include "Filter.h"
#include "Walker.h"

#include "iskat.h"

char const * PROGRAM_NAME = "iskat";

namespace po = boost::program_options;

char const * VERSION = "0.2.0";

void walk(std::string const & start_dir, po::variables_map & po) {
    int max_depth = -1;
    bool follow_syml = false;

    if (po.count("max-depth")) {
        max_depth = po["max-depth"].as<int>();
    } 
    if (po.count("follow-syml")) {
        follow_syml = true;
    }

    Walker w(start_dir, max_depth, follow_syml);
    FilterList filters = make_filters(po);

    File current(start_dir);

    while(w.get_next(current)){

        bool pass_filters = true;

        filter_it cur = filters.begin();
        filter_it end = filters.end();

        for (;cur != end; ++cur) {
            pass_filters &= (*cur)(current);
        }

        if (pass_filters) {
            std::cout << current.name() << std::endl;
        }
    }

}

int main(int argc, char *argv[]) {

    using std::string;
    using po::value;

    po::variables_map pasred_options;

    po::options_description desc("General options");
    desc.add_options()
        ("help,h", "Print a summary of the command-line usage of iskat and exit.")
        ("version,v", "Print the iskat version number and exit.")
        ("start,S", value<string>()->default_value("."), "Starting path")
        ("max-depth,D", value<int>(), "Descend  at  most levels (a non-neg"
        "ative integer) levels of directories below the commandline arguments.")
        ("follow-syml,F", "Follow symbolic links.")
    ;

    po::options_description filter_options("tests");
    filter_options.add_options()
        ("type,t", value<string>(), "File is of type t")
        ("permissions,p", value<string>(), "File's permission bits are exactly mode")
        ("uid,u", value<int>(), "File's numeric user ID is n")
        ("gid,g", value<int>(), "File's numeric group ID is n")
        ("owner,u", value<string>(), "File is owned by o")
        ("group,G", value<string>(), "File belongs to group g")
        ("newer,N", value<string>(), "File modified after time point")
        ("older,O", value<string>(), "File modified before time point")
        ("name,n", value<string>(), "File name")
        ("size,s", value<std::vector<string>>()->composing(), "File size")

    ;

    po::options_description action_options("actions");
    action_options.add_options()
        ("print,P", "Print the full file name")
        ("exec,E", "execute command")
    ;

    desc.add(filter_options).add(action_options);

    po::positional_options_description positional_args;
    positional_args.add("start", 1);


    try {
        po::store(po::command_line_parser(argc, argv)
            .options(desc)
            .positional(positional_args)
            .run(), pasred_options);

        po::notify(pasred_options);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    if (pasred_options.count("help")) {
        std::cout << desc;
        return 0;
    }
    if (pasred_options.count("version")) {
        std::cout << VERSION << std::endl;
        return 0;
    }

    try {
        walk(pasred_options["start"].as<string>(), pasred_options);
    } 
    catch (std::exception & e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
