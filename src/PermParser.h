#ifndef _PERM_PARSER_H
#define _PERM_PARSER_H 

#include <sys/types.h>
#include <string>
#include <boost/tuple/tuple.hpp>

typedef boost::tuple<bool, mode_t, char> parser_result_t;

parser_result_t parse_permissions(std::string const &);

#endif