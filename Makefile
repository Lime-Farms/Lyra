CFLAGS ?= -Wall -Wextra -Wpedantic -g -O1
LDFLAGS ?= -g -O1
LYRA_OBJS := lyra hm-node hm-ll hm ring em timer

.PHONY: all lyra init docs test clean

all: lyra

lyra: bin/lyra

init: bin

bin:
	mkdir -p -- bin

bin/lyra: $(addprefix bin/,$(addsuffix .o,$(LYRA_OBJS)))
	cc $(LDFLAGS) -o bin/lyra $^

bin/%.o: src/%.c | init
	cc $(CFLAGS) -std=c99 -I include -c -o $@ $<

docs:
	@printf "not implemented yet.\n" >&2

test:
	@printf "not implemented yet.\n" >&2

clean:
	rm -r -- bin/*
