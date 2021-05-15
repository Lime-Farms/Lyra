.PHONY: lyra init docs test clean

lyra: init bin/lyra

init:
	mkdir -p -- bin

bin/lyra: bin/lyra.o bin/hm-node.o bin/hm-ll.o bin/hm.o bin/ring.o
	cc -std=c99 -Wall -Wextra -pedantic -g -O1 -o bin/lyra $^

bin/%.o: src/%.c
	cc -std=c99 -Wall -Wextra -pedantic -g -O1 -I include -c -o $@ $^

docs:
	@printf "not implemented yet.\n" >&2

test:
	@printf "not implemented yet.\n" >&2

clean:
	rm -rf -- bin/*
