.PHONY: lyra docs test clean

lyra: bin/lyra

bin/lyra: src/lyra.o
	mkdir -p -- ./bin
	cc -std=c99 -Wall -Wextra -pedantic -g -O1 -o bin/lyra $^

bin/%.o: src/%.c
	cc -std=c99 -Wall -Wextra -pedantic -g -O1 -I include -c -o $@ $^

docs:
	printf "not implemented yet.\n" >&2

test:
	printf "not implemented yet.\n" >&2

clean:
	rm -rf -- bin/*
