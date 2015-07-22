#ifndef _IKSAT_H
#define _IKSAT_H

extern char const * PROGRAM_NAME;
extern char const * VERSION;

struct clone_mixin {
	virtual clone_mixin* clone() const { return new clone_mixin(*this); }
};

#endif