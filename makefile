CFLAGS+= -Wall -pedantic -ggdb -O3 -Iinclude
OBJECTS= main.o game.o cam.o calc.o gui.o tile.o level.o
LIBS= -lncurses

crawl: ${OBJECTS}
	gcc ${CFLAGS} -o $@ ${OBJECTS} ${LIBS}

%.o: src/%.c include/*.h
	gcc ${CFLAGS} -c -o $@ $<

clean:
	-rm -f $(OBJECTS) crawl

install: crawl
	sudo cp -v crawl /usr/bin/

.PHONY: clean
