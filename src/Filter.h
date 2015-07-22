#ifndef _FILTER_H
#define _FILTER_H

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/utility.hpp>
#include <string>
#include <vector>

typedef std::vector<std::string> size_list_t;

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

struct NameFilter : BaseFilter {
	NameFilter( std::string const & n ) : pattern(n) {}
	virtual bool operator()(File const &f);
	NameFilter* clone() const { return new NameFilter(*this); }

	std::string pattern;
};

struct SizeFilter : BaseFilter {
	SizeFilter ( off_t sz, bool greater_than ) : size(sz), gt(greater_than) {} 
	virtual bool operator()(File const &f);
	SizeFilter* clone() const { return new SizeFilter(*this); }

	off_t size;
	bool gt;
};

struct UidFilter : BaseFilter {
	UidFilter ( uid_t u ) : uid(u) {}
	UidFilter (std::string const &);
	virtual bool operator()(File const & f) { return uid == f.uid(); }
	UidFilter* clone() const { return new UidFilter(*this); }

	uid_t uid;
};

struct GidFilter : BaseFilter {
	GidFilter ( gid_t u ) : gid(u) {}
	GidFilter ( std::string const &);
	virtual bool operator()(File const & f ) { return gid == f.gid(); }
	GidFilter* clone() const { return new GidFilter(*this); }

	gid_t gid;
};

typedef boost::ptr_list<BaseFilter> FilterList;
typedef FilterList::iterator filter_it;

void make_size_filters(size_list_t &, FilterList &);
FilterList make_filters(boost::program_options::variables_map &, size_list_t &);
inline BaseFilter* new_clone( BaseFilter const & o) { return o.clone(); }

#endif // _FILTER_H