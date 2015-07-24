#include "PermParser.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/tokenizer.hpp>

typedef std::string str;
typedef str const & str_cref;
typedef str::const_iterator str_it;
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
typedef boost::regex re;
typedef boost::smatch sm;
typedef str::const_iterator str_it;
typedef boost::regex_iterator<str_it> re_it;


bool match(str_cref t, sm & s, re const & r) {
	return regex_match(t,s,r);
}

str group(sm & s, int i) {
	return str(s[i].first, s[i].second);
}

bool has(str_cref s, char c) {
	return s.find(c) != str::npos;
}

struct perm_parser {
	mode_t mask ;

	str mode;
	sm s;

	bool match(str_cref t, re const & r) {
		return regex_match(t,s,r);
	}

	str group(int i) {
		return str(s[i].first, s[i].second);
	}

	bool feed(str_cref text) {
		mask = 0;
		re e1("^([-+=\\\\])?([0-7]{1,4})$");
		re e2("^([-+=\\\\])?(([ugoa]*([-+=]([ugo]|[rwxst]*))+,)*([ugoa]*([-+=]([ugo]|[rwxst]*))+))$");

		// 644, 777, etc...
		if (match(text, e1)) {
			mode = group(1);
			parse_num(group(2));
			return true;
		}

		// symbolical
		if (match(text, e2)) {
			mode = group(1);
			parse_sym(group(2));
			return true;
		}
		return false;
	}

	void parse_num(str_cref t) {
		mask = std::stoi(t, 0, 8);
	}

	void parse_sym(str_cref t) {
		boost::char_separator<char> sep(",");
		tokenizer tokens(t, sep);
		auto end = tokens.end();
		for (auto it = tokens.begin();it != end; ++it) {
			parse_tok(*it);
		}
	}

	void parse_tok(str_cref t) {
		re e("([ugoa]*)(([-+=]([ugo]|[rwxst]*))+)");
		match(t, e);

		str selectors = group(1);
		str actions = group(2);

		mode_t selector_mask = 0;
		if (selectors == "") selectors = "a";
		if (has(selectors, 'u')) selector_mask |= 04700;
		if (has(selectors, 'g')) selector_mask |= 02070;
		if (has(selectors, 'o')) selector_mask |= 01007;
		if (has(selectors, 'a')) selector_mask = 07777;

		re e2("([-+=])(([ugo])|([rwxst]*))");
		re_it it(actions.begin(), actions.end(), e2);
		re_it end;

		for (; it != end; ++it) {
			mode_t bits = 0;
			sm s1 = *it;
			str act = ::group(s1, 1);
			str modifier = ::group(s1, 2);

			// +u +g +o
			if (s1[3].matched) {
				switch (modifier[0]) {
					case 'u': 
						bits = (mask & 0700);
						bits |= (bits >> 3) | (bits >> 6) | mask & 04000;
						break;
					case 'g': 
						bits = mask & 070;
						bits |= (bits >> 3) | (bits << 3) | mask & 02000;
						break;
					case 'o': 
						bits = mask & 07;
						bits |= (bits << 6) | (bits << 3) | mask & 01000;
						break;
					case 'a': bits = mask;
				}
			}

			// +r +w +x
			else {
				if (has(modifier, 'r')) bits |= 0444;
				if (has(modifier, 'w')) bits |= 0222;
				if (has(modifier, 'x')) bits |= 0111;
				if (has(modifier, 's')) bits |= 01000;
				if (has(modifier, 't')) bits |= 06000;
			}

			switch(act[0]) {
				case '+': mask |= bits & selector_mask; break;
				case '-': mask ^= bits & selector_mask; break;
				case '=': mask = (mask & ~selector_mask) | (bits & selector_mask);
			}		
		}
	}
};

parser_result_t parse_permissions(str_cref expr) {
	perm_parser parser;
	char action;

	if (parser.feed(expr)) {
		if (parser.mode.empty()) action = '=';
		else action = parser.mode[0];

		return {true, parser.mask, action};
	}
	else {
		return {false, 0, '\0'};
	}
}
/*#include <iostream>

int main(int argc, char const *argv[]) {

	using std::cout;

	str s;

	cout << std::oct;

	while(getline(std::cin, s)) {
		auto p = parse_permissions(s);

		auto ok = p.get<0>();
		auto mask = p.get<1>();
		auto act = p.get<2>();


		cout << (ok ? "OK: " : "FAIL");
		if (ok) cout << act << mask;
		cout << std::endl; 
	}
	
	return 0;
}
*/