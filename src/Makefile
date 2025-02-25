CFLAGS = -Wall -Wextra

.PHONY: build clean

build: kover

kover: kover.o scene.o validation.o
	gcc kover.o scene.o validation.o -o kover

kover.o: kover.c
	gcc $(CFLAGS) -c kover.c

scene.o: scene.h scene.c
	gcc $(CFLAGS) -c scene.c

validation.o: validation.h validation.c
	gcc $(CFLAGS) -c validation.c

clean:
	rm -f *.o
	rm -f kover
