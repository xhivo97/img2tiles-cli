ifeq ($(OS), Windows_NT)
	NAME=img-slice.exe
	NULL=nul
else
	NAME=img-slice
	NULL=/dev/null
endif

CC=gcc
CFLAGS=-O2 -DNDEBUG -Wall -Wextra
LDFLAGS=-lpng

SRCDIR=src
SRCS=$(wildcard $(SRCDIR)/*.c)
RELOBJS=$(patsubst $(SRCDIR)/%.c, release/obj/%.o, $(SRCS))
DBGOBJS=$(patsubst $(SRCDIR)/%.c, debug/obj/%.o, $(SRCS))

RELEASEBIN=release/$(NAME)
DEBUGBIN=debug/$(NAME)

all: $(RELEASEBIN)

debug: CFLAGS=-g -O0 -DDEBUG -Wall -Wextra
debug: $(DEBUGBIN)

$(RELEASEBIN): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELOBJS) $(LDFLAGS) -o $@

$(DEBUGBIN): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGOBJS) $(LDFLAGS) -o $@

release/obj/%.o: $(SRCDIR)/%.c
	@ mkdir -p release
	@ mkdir -p release/obj
	$(CC) $(CFLAGS) -c $< -o $@

debug/obj/%.o: $(SRCDIR)/%.c
	@ mkdir -p debug
	@ mkdir -p debug/obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(RELOBJS) $(DBGOBJS) $(DEBUGBIN) $(RELEASEBIN)
	@ mkdir release release/obj 2> $(NULL) || true
	rmdir release/obj release
	@ mkdir debug debug/obj 2> $(NULL) || true
	rmdir debug/obj debug