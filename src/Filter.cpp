#include "Filter.h"

#include <boost/regex.hpp>

using std::string;
typedef string const & str_cref;

FilterList make_filters(boost::program_options::variables_map & po) {
    FilterList ret;

    if (po.count("type")) {
        ret.push_back(new TypeFilter(po["type"].as<string>()));
    }
    if (po.count("name")) {
        ret.push_back(new NameFilter(po["type"].as<string>()));
    }
    if (ret.size() == 0) {
        ret.push_back(new TrueFilter);
    }
    return ret;
}

TypeFilter::TypeFilter(str_cref s) {
    switch (*s.c_str()) {
        case 'b' : type = BLOCK_DEVICE; return;
        case 'c' : type = CHARACTER_DEVICE; return; 
        case 'd' : type = DIRECTORY; return; 
        case 'p' : type = FIFO; return;
        case 'f' : type = REGULAR_FILE; return;                  
        case 'l' : type = SYMBOLIC_LINK; return;   
        case 's' : type = SOCKET; return;
        default: type = NOT_A_FILE; return;
    }
}


bool NameFilter::operator()(File const &f) {
    boost::regex pattern(name);
    return regex_match(f.name(), pattern);
}