#ifndef _ACTION_H
#define _ACTION_H

#include "iskat.h"

#include <ostream>
#include <sstream>

#include <boost/ptr_container/ptr_list.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/utility.hpp>

#include "File.h"

struct BaseAction {
	virtual ~BaseAction() {};
	virtual void operator()(File const &) = 0;
	virtual BaseAction * clone() const {return (BaseAction*)NULL;}
};

struct PrintAction : BaseAction, clone_mixin {
	virtual void operator()(File const &);
};

struct ExecAction : BaseAction, clone_mixin {
	ExecAction(std::string const & s) : pattern(s) {}
	virtual void operator()(File const &);

	std::string pattern;
};

struct ExecAllAction : BaseAction, clone_mixin {
	ExecAllAction(std::string const & s) : pattern(s) {}
	~ExecAllAction();
	virtual void operator()(File const &);

	std::string pattern;
	std::stringstream ss;
};

typedef boost::ptr_list<BaseAction> ActionList;

void apply_actions(File const & f, ActionList &);
ActionList make_actions(boost::program_options::variables_map &);
inline BaseAction* new_clone( BaseAction const & o) { return o.clone(); }

#endif // _FILTER_H