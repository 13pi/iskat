#include "Walker.h"

bool Walker::get_next(std::string & s) {
	skip_deep();
	if (cur == end) return false;
	s = cur->path().string();
	++cur;
	return true;
}

Walker::Walker(std::string const & start, int md, bool fl) : follow_syml(fl), max_depth(md) {
	fs::path s_dir(start);
	cur = fs::recursive_directory_iterator(s_dir);
}

void Walker::skip_deep() {
	if (max_depth == -1) return;
	while(cur != end && cur.level() > max_depth) ++cur;
}
