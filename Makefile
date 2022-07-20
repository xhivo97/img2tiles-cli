CC=gcc
CFLAGS=-O2 -DNDEBUG -Wall -Wextra
LDFLAGS=-lpng
BASENAME=img-slice

ifeq ($(OS), Windows_NT)
	SHELL=powershell.exe
	.SHELLFLAGS=-NoProfile -Command
	MKDIR=mkdir -p
	RM=rm -Force
	SUPPRESS=-Force | Out-Null
	NAME=$(BASENAME).exe
	LDFLAGS+=-municode
else
	NAME=$(BASENAME)
	MKDIR=mkdir -p
	SUPPRESS=2>/dev/null || true
	RM=rm -f
endif

SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
RELOBJS=$(patsubst $(SRCDIR)/%.c, release/obj/%.o, $(SRCS))
DBGOBJS=$(patsubst $(SRCDIR)/%.c, debug/obj/%.o, $(SRCS))

RELEASEBIN=release/$(NAME)
DEBUGBIN=debug/$(NAME)

all: $(RELEASEBIN)
debug: CFLAGS=-g -O0 -DDEBUG -Wall -Wextra
debug: $(DEBUGBIN)

remake: clean $(RELEASEBIN)

$(RELEASEBIN): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELOBJS) $(LDFLAGS) -o $@

$(DEBUGBIN): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGOBJS) $(LDFLAGS) -o $@

release/obj/%.o: $(SRCDIR)/%.c
ifeq ($(OS), Windows_NT)
	@ $$env:PATH = ($$env:PATH -split ';' | ?{ $$_ -notmatch '\\Strawberry\\c\\bin$$' }) -join ';'
endif
	@ $(MKDIR) release/obj $(SUPPRESS)
	$(CC) $(CFLAGS) -c $< -o $@

debug/obj/%.o: $(SRCDIR)/%.c
ifeq ($(OS), Windows_NT)
	@ $$env:PATH = ($$env:PATH -split ';' | ?{ $$_ -notmatch '\\Strawberry\\c\\bin$$' }) -join ';'
endif
	@ $(MKDIR) debug/obj $(SUPPRESS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@ $(MKDIR) release/obj $(SUPPRESS)
	@ $(MKDIR) debug/obj $(SUPPRESS)
	$(RM) release/obj/*.o
	rmdir release/obj
	$(RM) release/*
	rmdir release
	$(RM) debug/obj/*.o
	rmdir debug/obj
	$(RM) debug/*
	rmdir debug