#include "Action.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <cstdlib>

ActionList make_actions(boost::program_options::variables_map & po) {
	using std::string;
	ActionList ret;

	if (po.count("exec")) {
		string t = po["exec"].as<string>();
		boost::regex e("^(.*)(\\;|\\+)$");
		boost::smatch s;

		if (regex_match(t, s, e)) {
			
			string text = string(s[1].first, s[1].second);
			string action = string(s[2].first, s[2].second);

			if (action == ";") ret.push_back(new ExecAction(text));
			else ret.push_back(new ExecAllAction(text));
		} 
		else throw std::invalid_argument("Invalid exec argument: " + t);
		
		}
	if (po.count("print") || (ret.size() == 0)) {
		ret.push_back(new PrintAction);
	}

	return ret;
}

void apply_actions(File const & f, ActionList & actions) {
	auto it = actions.begin();
	auto end = actions.end();

	for (;it != end; ++it) {
		(*it)(f);
	}
}

void PrintAction::operator()(File const & f) {
	std::cout << f.path() << std::endl;
}

void ExecAction::operator() (File const & f) {
	std::string const sub = "{}";
	std::string cmd = boost::replace_all_copy(pattern, sub, f.path());
	system(cmd.c_str());
}
void ExecAllAction::operator() (File const & f) {
	ss << f.path() << " ";
}

ExecAllAction::~ExecAllAction() {
	std::string const sub = "{}";
	std::string cmd = boost::replace_all_copy(pattern, sub, ss.str());
	system(cmd.c_str());
}
