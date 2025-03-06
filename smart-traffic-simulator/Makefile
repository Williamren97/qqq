TARGET := bin/enviro

all: 
	mkdir -p lib
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean

start:
	esm start

stop:
	esm stop

run: all start
	enviro

install: all
	esm start
	enviro