CC=g++
CFLAGS=-c -std=c++0x -Wall -Werror -Wno-unused-result -DBOOST_ALL_DYN_LINK
LDFLAGS=-lboost_program_options -lboost_filesystem -lboost_system -lboost_regex

EXEC=iskat
SOURCES=main.cpp File.cpp Filter.cpp Walker.cpp DateParser.cpp Action.cpp PermParser.cpp
HEADERS=File.h Filter.h Walker.h iskat.h DateParser.h Action.h PermParser.h
OBJECTS=$(SOURCES:.cpp=.o)

DBGDIR=build/debug
DBGOBJS=$(addprefix $(DBGDIR)/, $(OBJECTS))
DBGCFLAGS=-g3 -O0 -DDEBUG

RELDIR=build/release
RELOBJS=$(addprefix $(RELDIR)/, $(OBJECTS))
RELCFLAGS=-O3 -DNDEBUG

TESTSRC=$(wildcard test/*.cc)
TESTOBJ=$(TESTSRC:.cc=.o)
TESTCFLAG=-O0 -Wall -Werror
TESTLDFLAG=
TESTEXEC=test/test
RELMAINOBJ=$(addprefix $(RELDIR)/,main.o)
TESTRELOBJECTS=$(filter-out $(RELMAINOBJ),$(RELOBJS))

MAN=iskat.1.gz

.PHONY: all clean debug prep release rebuild install tgz uninstall test

default: prep release

release: $(RELOBJS)
	$(CC) $(RELOBJS) $(LDFLAGS) -o $(EXEC) 

debug: $(DBGOBJS)
	$(CC) $(DBGOBJS) $(LDFLAGS) -o $(EXEC) 

$(DBGDIR)/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $@ $<

$(RELDIR)/%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $@ $<

$(HEADERS):

iskat.1:
	

rebuild: clean default

clean:
	rm -vf $(EXEC) $(RELOBJS) $(DBGOBJS)

$(MAN): iskat.1
	gzip iskat.1.gz

install: $(EXEC) $(MAN)
	cp $(EXEC) /usr/bin/
	cp iskat.1.gz /usr/local/share/man/man1/

uninstall:
	rm -f /usr/bin/$(EXEC)

tgz:
	tar -cvzf ../bin/iskat.tgz $(SOURCES) $(HEADERS) makefile

prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

test/%.o: %.cc
	$(CC) $(CFLAGS) $(TESTCFLAG) -o $@ $<

$(TESTEXEC): $(TESTRELOBJECTS) $(TESTOBJ)
	$(CC) $(TESTOBJ) $(TESTRELOBJECTS) $(TESTLDFLAG) -o $(TESTEXEC)

test: $(TESTEXEC)
	@test/test