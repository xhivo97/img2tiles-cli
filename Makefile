NAME=img-slice
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