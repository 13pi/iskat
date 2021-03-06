#include "Filter.h"

#include <boost/regex.hpp>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <fnmatch.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <cerrno>

#include "DateParser.h"
#include "PermParser.h"

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
    if (po.count("uid")) {
        ret.push_back(new UidFilter(po["uid"].as<int>()));
    }
    if (po.count("gid")) {
        ret.push_back(new GidFilter(po["gid"].as<int>()));
    }
    if (po.count("group")) {
        ret.push_back(new GidFilter(po["group"].as<string>()));
    }
    if (po.count("owner")) {
        ret.push_back(new UidFilter(po["owner"].as<string>()));
    }
    if (po.count("older")) {
        ret.push_back(new TimeFilter(po["older"].as<string>(), true));
    }
    if (po.count("newer")) {
        ret.push_back(new TimeFilter(po["newer"].as<string>(), false));
    }
    if (po.count("perm")) {
        ret.push_back(new PermFilter(po["perm"].as<string>()));
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

UidFilter::UidFilter( std::string const & name) {
    struct passwd * user = getpwnam(name.c_str());
    if (user == NULL) {
        throw std::invalid_argument("No user named " + name);
    }
    uid = user->pw_uid;
}

GidFilter::GidFilter( std::string const & name)  {
    struct group * group = getgrnam(name.c_str());
    if (group == NULL) {
        throw std::invalid_argument("No group named " + name);
    }
    gid = group->gr_gid;
}

TimeFilter::TimeFilter ( std::string const & date, bool o) : older(o) {
    timepoint = parse_date(date);
}

bool TimeFilter::operator()(File const & f ) {
    return older ? f.ctime() < timepoint : f.ctime() > timepoint;
}

PermFilter::PermFilter ( std::string const & e) {
    parser_result_t parsed = parse_permissions(e);

    if (!parsed.get<0>())
        throw std::invalid_argument("Invalid mode: " + e);

    mode = parsed.get<1>();
    action = parsed.get<2>();
}

bool PermFilter::operator()(File const & f) {
    switch (action) {
        case '+': return f.permissions() & mode;
        case '/': return f.permissions() & mode;
        case '-': return (f.permissions() & mode) == mode;
        case '=': return f.permissions() == mode;
        default: return false;
    }
}
