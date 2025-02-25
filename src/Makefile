.PHONY: build clean

build: kover

kover: kover.o validation.o
	gcc kover.o validation.o -o kover

validation.o: validation.h validation.c
	gcc -c validation.c

kover.o: kover.c
	gcc -c kover.c

clean:
	rm -f *.o
	rm -f kover
