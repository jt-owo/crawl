CFLAGS+= -Wall -pedantic -ggdb -O3 -Iinclude
OBJECTS= main.o gui.o
LIBS= -lncurses

crawl: ${OBJECTS}
	gcc ${CFLAGS} -o $@ ${OBJECTS} ${LIBS}

%.o: src/%.c include/*.h
	gcc ${CFLAGS} -c -o $@ $<

clean:
	-rm -f $(OBJECTS) crawl

install: crawl
	cp -v crawl /usr/bin/

.PHONY: clean
