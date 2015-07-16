#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

char const * VERSION = "0.0.2";

namespace po = boost::program_options;

int main(int argc, char *argv[]) {

    using std::string;
    using po::value;

    po::variables_map pasred_options;
    bool follow_syml = false;
    int max_depth;


    po::options_description desc("General options");
    desc.add_options()
        ("help,h", "Print a summary of the command-line usage of iskat and exit.")
        ("version,v", "Print the iskat version number and exit.")
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
        ("owner,o", value<string>(), "File is owned by o")
        ("group,G", value<string>(), "File belongs to group g")
        ("newer,n", value<string>(), "File modified after time point")
        ("older,O", value<string>(), "File modified before time point")

    ;

    po::options_description action_options("actions");
    action_options.add_options()
        ("print,P", "Print the full file name")
    ;

    desc.add(filter_options).add(action_options);


    try {
        po::store(po::command_line_parser(argc, argv).options(desc).run(), pasred_options);
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
    if (pasred_options.count("max-depth")) {
        max_depth = pasred_options["max-depth"].as<int>();
    } 
    else {
        max_depth = -1;
    }
    if (pasred_options.count("follow-syml")) {
        follow_syml = true;
    }

    std::cout << max_depth << follow_syml << std::endl;

    return 0;
}
