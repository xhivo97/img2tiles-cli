CC=gcc
CFLAGS=-O2 -DNDEBUG -Wall -Wextra
LDFLAGS=-lpng
BASENAME=img-slice

ifeq ($(OS), Windows_NT)
	SHELL=cmd.exe
	NAME=$(BASENAME).exe
	LDFLAGS+=-municode
else
	NAME=$(BASENAME)
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

$(RELEASEBIN): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELOBJS) $(LDFLAGS) -o $@

$(DEBUGBIN): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGOBJS) $(LDFLAGS) -o $@


ifeq ($(OS), Windows_NT)
release/obj/%.o: $(SRCDIR)/%.c
	@ mkdir release\obj 2> nul || VER>NUL
	$(CC) $(CFLAGS) -c $< -o $@

debug/obj/%.o: $(SRCDIR)/%.c
	@ mkdir debug\obj 2> nul || VER>NUL
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@ mkdir release\obj 2> nul || VER>NUL
	@ mkdir debug\obj 2> nul || VER>NUL
	del /f /q release\obj\* release\* && rmdir release\obj release
	del /f /q debug\obj\* debug\* && rmdir debug\obj debug
else
release/obj/%.o: $(SRCDIR)/%.c
	@ mkdir -p release/obj || true
	$(CC) $(CFLAGS) -c $< -o $@

debug/obj/%.o: $(SRCDIR)/%.c
	@ mkdir -p debug/obj || true
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@ mkdir -p release/obj || true
	@ mkdir -p debug/obj || true
	rm -f release/obj/*.o && rmdir release/obj && rm -f release/* && rmdir release
	rm -f debug/obj/*.o && rmdir debug/obj && rm -f debug/* && rmdir debug
endif