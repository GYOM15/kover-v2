.PHONY: build clean test

exec = kover

build:
	$(MAKE) build -C src
	$(MAKE) bin
	cp src/$(exec) bin/$(exec)

bin:
	mkdir -p bin

clean:
	$(MAKE) clean -C src
	rm -rf bin

test: build
	$(MAKE) -C bats
