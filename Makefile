exec = bin/kover
main = src/kover.c

.PHONY: bindir clean test

$(exec): bindir $(main)
	gcc $(main) -o $(exec)

bindir:
	mkdir -p bin

clean:
	rm -rf bin

test: $(exec)
	$(MAKE) -C bats
