#include <iostream>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include <iomanip>

char const * version = "0.0.2";

namespace po = boost::program_options;

//TODO: not used
struct File {
    File(const std::string&);
    std::string filename;
    struct stat buf;
    /* more fields  */
};

    void walk( const boost::filesystem::path & dir_path,         // in this directory,
               int maxdep ){
      if ( !(exists( dir_path )&&maxdep) ) return;
      boost::filesystem::directory_iterator end_itr; // default construction yields past-the-end
      for ( boost::filesystem::directory_iterator itr( dir_path );
            itr != end_itr;
            ++itr )
      {
        if ( is_directory(itr->status()) )
        {
          //std::cout<<itr->path().leaf();
          walk( itr->path(), maxdep-1);
        }else // see below{

        char *date;
        int ret;
        struct stat buf;

// absolute path !!!
// itr->path().string().c_str();
        if ((ret = stat(itr->path().string().c_str(), &buf))!=0){
            std::cout << "error" << itr->path() << "\r\n";
            fprintf(stderr, "stat failure error .%d", ret);
            // abort();
            return;
          }
        date = asctime(localtime(&buf.st_ctime));

        // JUST FILE NAME !!
     std::cout << std::setw(10) << std::left <<  itr->path().leaf();
     std::cout << std::setw(5) << std::left << buf.st_size  ;
     std::cout << std::setw(10) << std::left << date  ;
     std::cout << std::setw(10) << std::left << buf.st_mode  ;
     std::cout << std::setw(5) << std::left << buf.st_uid  ;
     std::cout << std::setw(5) << std::left << buf.st_gid << "\n" ;


       // std::cout<<itr->path().leaf() << "\t";
         //std::cout << buf.st_size << "\t";
         ///std::cout <<std::setw (25) << std::left <<  date << "\t";
         //std::cout << buf.st_mode << "\t";
        // printf("\n %ld size\n", buf.st_size);
        //printf("\n %s\n", date);
        //printf("\n %d mode\n", buf.st_mode);
        //std::cout << "st_uid" << buf.st_uid << "\t";
        //std::cout << "st_gid" << buf.st_gid << "\t";
        //std::cout <<std::endl;

        }
      }
      return;
    }



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
        std::cout << version << std::endl;
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

    std::cout <<"Name" << std::setw(5) 
    <<"Size"<< std::setw(30) 
     <<"Date " << std::setw(20) 
    <<"Mode "<< std::setw(20) 
    <<"st_uid " << std::setw(20) 
    <<"st_gid \n"  ;
    std::cout << std::setfill(' ') << std::setw(140) << " " << std::endl;
 
    boost::filesystem::path cur_dir(".");
    walk(cur_dir, 7);

    return 0;
}
