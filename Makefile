CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -O2 -Iinclude

all: bin/nanokv bin/test_nanokv

bin/nanokv: src/nanokv.c src/main.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

bin/test_nanokv: src/nanokv.c tests/test_nanokv.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^

test: bin/test_nanokv
	./bin/test_nanokv

clean:
	rm -rf bin *.db
