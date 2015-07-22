#include "Filter.h"

#include <boost/regex.hpp>

#include <fnmatch.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

using std::string;
using std::cout;
using std::endl;


typedef string const & str_cref;

FilterList make_filters(boost::program_options::variables_map & po, size_list_t & sl) {
    FilterList ret;

    if (po.count("type")) {
        ret.push_back(new TypeFilter(po["type"].as<string>()));
    }
    if (po.count("name")) {
        ret.push_back(new NameFilter(po["name"].as<string>()));
    }
    if (po.count("size")) {
        make_size_filters(sl, ret);
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
    return !fnmatch(pattern.c_str(), f.name().c_str(), 0) ;
}

void make_size_filters(size_list_t & size_list, FilterList & ret) {
    auto n = size_list.size();
    
    const boost::regex pattern("(\\+|-)?(\\d+)(b|K|M|G)?");
    boost::smatch m;

    for (size_t i = 0; i < n; ++i) {
        std::string & text = size_list[i];
        if (regex_match(text, m, pattern)) {
            
            string sign = string(m[1].first, m[1].second);
            string value = string(m[2].first, m[2].second);
            string suffix = string(m[3].first, m[3].second);

            off_t ivalue = stoi(value);
            bool gt = true;

            if (suffix.empty()) suffix = "b";
            if (sign == "-") gt = false;

            switch (suffix[0]) {
                case 'K': ivalue *= 1000; break;
                case 'M': ivalue *= 1000*1000; break;
                case 'G': ivalue *= 1000*1000*1000; break;
                default: break;
            }

            ret.push_back(new SizeFilter(ivalue, gt));
        }
        else {
            std::ostringstream s;
            s << "Invalid argument for --size: " << text;
            throw std::invalid_argument(s.str());
        }
    }
}

bool SizeFilter::operator()(File const &f) {
    if (gt) return f.size() >= size;
    return f.size() <= size;
}

