#ifndef _FILTER_H
#define _FILTER_H

#include "iskat.h"

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/utility.hpp>
#include <string>
#include <vector>
#include <ctime>

#include "DateParser.h"

typedef std::vector<std::string> size_list_t;

#include "File.h"

struct BaseFilter {
	virtual ~BaseFilter() {};
	virtual bool operator()(File const &) = 0;
	virtual BaseFilter * clone() const {return (BaseFilter*)NULL;}
};

struct TrueFilter : BaseFilter, clone_mixin {
	virtual bool operator()(File const &) { return true; }
};

struct TypeFilter : BaseFilter, clone_mixin {
	TypeFilter( std::string const & );
	TypeFilter( FileType t ) : type(t) {}
	virtual bool operator()(File const &f) { return type == f.type(); }

	FileType type;
};

struct NameFilter : BaseFilter, clone_mixin {
	NameFilter( std::string const & n ) : pattern(n) {}
	virtual bool operator()(File const &f);

	std::string pattern;
};

struct SizeFilter : BaseFilter, clone_mixin {
	SizeFilter ( off_t sz, bool greater_than ) : size(sz), gt(greater_than) {} 
	virtual bool operator()(File const &f);

	off_t size;
	bool gt;
};

struct UidFilter : BaseFilter, clone_mixin {
	UidFilter ( uid_t u ) : uid(u) {}
	UidFilter (std::string const &);
	virtual bool operator()(File const & f) { return uid == f.uid(); }

	uid_t uid;
};

struct GidFilter : BaseFilter, clone_mixin {
	GidFilter ( gid_t u ) : gid(u) {}
	GidFilter ( std::string const &);
	virtual bool operator()(File const & f ) { return gid == f.gid(); }

	gid_t gid;
};
struct TimeFilter : BaseFilter, clone_mixin {
	TimeFilter ( std::string const &, bool);
	virtual bool operator()(File const & f );

	time_t timepoint;
	bool older;
};

typedef boost::ptr_list<BaseFilter> FilterList;
typedef FilterList::iterator filter_it;

void make_size_filters(size_list_t &, FilterList &);
FilterList make_filters(boost::program_options::variables_map &, size_list_t &);
inline BaseFilter* new_clone( BaseFilter const & o) { return o.clone(); }

#endif // _FILTER_H