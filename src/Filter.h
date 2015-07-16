#ifndef _FILTER_H
#define _FILTER_H

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/utility.hpp>
#include <string>

#include "File.h"

struct BaseFilter {
	virtual bool operator()(File const &) = 0;
	virtual BaseFilter * clone() const = 0;
};

struct TrueFilter : BaseFilter {
	virtual bool operator()(File const &) { return true; }
	TrueFilter* clone() const { return new TrueFilter(*this); }
};

struct TypeFilter : BaseFilter {
	TypeFilter( std::string const & );
	TypeFilter( FileType t ) : type(t) {}
	virtual bool operator()(File const &f) { return type == f.type(); }
	TypeFilter* clone() const { return new TypeFilter(*this); }

	FileType type;
};

typedef boost::ptr_list<BaseFilter> FilterList;

FilterList make_filters(boost::program_options::variables_map &);
BaseFilter* new_clone( BaseFilter const & o) { return o.clone(); }

#endif // _FILTER_H