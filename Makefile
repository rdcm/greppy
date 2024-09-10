.PHONY: build clean run tests

build:
	mkdir -p build
	cd build && cmake .. && make

clean:
	rm -rf build

run:
	./build/greppy

tests:
	./build/tests