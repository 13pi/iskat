#include "DateParser.h"

#include <iostream>
#include <sstream>
#include <boost/date_time.hpp>
#include <stdexcept>

namespace bt = boost::posix_time;

static std::locale const formats[] = {
std::locale(std::locale::classic(),new bt::time_input_facet("%Y-%m-%d %H:%M:%S")),
std::locale(std::locale::classic(),new bt::time_input_facet("%Y/%m/%d %H:%M:%S")),
std::locale(std::locale::classic(),new bt::time_input_facet("%d.%m.%Y %H:%M:%S")),
std::locale(std::locale::classic(),new bt::time_input_facet("%H:%M:%S"))};
const size_t formats_n = sizeof(formats)/sizeof(formats[0]);

time_t date_as_time_t(const bt::ptime& pt) {
    bt::ptime timet_start(boost::gregorian::date(1970,1,1));
    bt::time_duration diff = pt - timet_start;
    return diff.ticks()/bt::time_duration::rep_type::ticks_per_second;
}

time_t time_as_time_t(bt::ptime & pt) {
	time_t now = time(NULL);
	tm * pt_tm = std::localtime(&now);
	tm old_tm = to_tm(pt);

	pt_tm->tm_hour = old_tm.tm_hour;
	pt_tm->tm_min  = old_tm.tm_min;
	pt_tm->tm_sec  = old_tm.tm_min;

	bt::ptime new_pt = bt::ptime_from_tm(*pt_tm);
	std::cout << new_pt << std::endl;
	return date_as_time_t(new_pt);
}

bool test_format(std::string const & s, std::locale const & format, bt::ptime & pt ) {
	std::istringstream is(s);
    is.imbue(format);
    is >> pt;
    if(pt != bt::ptime()) return true;
    return false;
}

time_t parse_date(std::string const & s ) {

	bt::ptime pt;
    for(size_t i=0; i<formats_n; ++i) {
        if (test_format(s, formats[i], pt)) {
        	return date_as_time_t(pt);
        }
    }
    // if (test_format(s, formats[formats_n - 1], pt)) {
    // 	return time_as_time_t(pt);
    // }

	throw std::invalid_argument("Invalid date : " + s);

}