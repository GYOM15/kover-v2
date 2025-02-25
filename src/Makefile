.PHONY: build clean

CFLAGS = -Wall -Wextra
exec = kover
modules = scene validation
modules_c_files = $(patsubst %,%.c,$(modules))
modules_h_files = $(patsubst %,%.h,$(modules))
modules_o_files = $(patsubst %,%.o,$(modules))

build: $(exec)

$(exec): $(exec).o $(modules_o_files)
	gcc $^ -o $@

$(exec).o: $(exec).c
	gcc $(CFLAGS) -c $<

$(modules_o_files): %.o: %.c %.h
	gcc $(CFLAGS) -c $<

clean:
	rm -f $(modules_o_files)
	rm -f $(exec)
