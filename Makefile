COMPILER=gcc
CFLAGS=-O2 -DNDEBUG -Wall -Wextra
LDFLAGS=-lpng -ljpeg
NAME=img-slice

ifeq ($(OS), Windows_NT)
	SHELL=powershell.exe
	.SHELLFLAGS=-NoProfile -Command
	CC=$(shell where.exe -q $(COMPILER); if (!$$LASTEXITCODE) \
		{ where.exe $(COMPILER) | ForEach-Object { if ($$_.Contains('msys')) \
		{ $$_ } } } else { "ERROR" })
	ERROR=$(shell "ERROR")
	MKDIR=mkdir -p
	RM=rm -Force
	SUPPRESS=-Force | Out-Null
	LDFLAGS+=-municode
	RUNTESTS=./test/bin/test_image.exe
else
	CC=$(COMPILER)
	MKDIR=mkdir -p
	SUPPRESS=2>/dev/null || true
	RM=rm -f
	RUNTESTS=for test in $(TESTBINS) ; do ./$$test ; done
endif

SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
RELOBJS=$(patsubst $(SRCDIR)/%.c, release/obj/%.o, $(SRCS))
RELOBJS:=$(filter-out release/obj/main.o, $(RELOBJS))
DBGOBJS=$(patsubst $(SRCDIR)/%.c, debug/obj/%.o, $(SRCS))
DBGOBJS:=$(filter-out debug/obj/main.o, $(DBGOBJS))

RELEASEBIN=release/$(NAME)
DEBUGBIN=debug/$(NAME)

TEST=test
TESTS=$(wildcard $(TEST)/*.c)
TESTBINS=$(patsubst $(TEST)/%.c, $(TEST)/bin/%, $(TESTS))
SILENT=

all: $(RELEASEBIN)
debug: CFLAGS=-g -O0 -DDEBUG -Wall -Wextra
debug: $(DEBUGBIN)

remake: clean $(RELEASEBIN)


$(RELEASEBIN): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELOBJS) $(SRCDIR)/main.c $(LDFLAGS) -o $@

$(DEBUGBIN): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGOBJS) $(SRCDIR)/main.c $(LDFLAGS) -o $@

release/obj/%.o: $(SRCDIR)/%.c
ifeq ($(CC), $(ERROR))
	@ echo "$(COMPILER) not found."
	@ exit 1
endif
	@ $(MKDIR) release/obj $(SUPPRESS)
	$(SILENT) $(CC) $(CFLAGS) -c $< -o $@

debug/obj/%.o: $(SRCDIR)/%.c
ifeq ($(CC), $(ERROR))
	@ echo "Cannot find compiler."
	@ exit 1
endif
	@ $(MKDIR) debug/obj $(SUPPRESS)
	$(SILENT) $(CC) $(CFLAGS) -c $< -o $@

$(TEST)/bin/%: $(TEST)/%.c
	$(SILENT) $(CC) $(CFLAGS) $< $(DBGOBJS) $(LDFLAGS) -o $@

$(TEST)/bin:
	mkdir $@

test: CFLAGS=-g -O0 -DDEBUG -Wall -Wextra
test: SILENT=
test: clean $(DBGOBJS) $(TEST)/bin $(TESTBINS)
	@ $(RUNTESTS)

clean:
	@ $(MKDIR) release/obj $(SUPPRESS)
	@ $(MKDIR) debug/obj $(SUPPRESS)
	@ $(MKDIR) test/bin $(SUPPRESS)
	$(SILENT) $(RM) release/obj/*.o
	$(SILENT) rmdir release/obj
	$(SILENT) $(RM) release/*
	$(SILENT) rmdir release
	$(SILENT) $(RM) debug/obj/*.o
	$(SILENT) rmdir debug/obj
	$(SILENT) $(RM) debug/*
	$(SILENT) rmdir debug
	$(SILENT) $(RM) test/bin/*