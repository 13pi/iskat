CC=g++
CFLAGS=-c -std=c++0x -Wall -Werror
LDFLAGS=-lboost_program_options -lboost_filesystem-mt

EXEC=iskat
SOURCES=main.cpp
HEADERS=
OBJECTS=$(SOURCES:.cpp=.o)

DBGDIR = build/debug
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJECTS))
DBGCFLAGS = -g3 -O0 -DDEBUG

RELDIR = build/release
RELOBJS = $(addprefix $(RELDIR)/, $(OBJECTS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release rebuild install tgz uninstall

default: prep release

release: $(RELOBJS)
	$(CC) $(RELOBJS) $(LDFLAGS) -o $(EXEC) 

debug: $(DBGOBJS)
	$(CC) $(DBGOBJS) $(LDFLAGS) -o $(EXEC) 

$(DBGDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $@ $<

$(RELDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $@ $<

rebuild: clean default

clean:
	rm -vf $(EXEC) $(RELOBJS) $(DBGOBJS)

install: $(EXEC)
	cp $(EXEC) /usr/bin/

uninstall:
	rm -f /usr/bin/$(EXEC)

tgz:
	tar -cvzf ../bin/iskat.tgz $(SOURCES) $(HEADERS) makefile

prep:
	@mkdir -p $(DBGDIR) $(RELDIR)