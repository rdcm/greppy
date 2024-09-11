.PHONY: build clean run tests format

build:
	mkdir -p build
	cd build && cmake .. && make

clean:
	rm -rf build

run:
	./build/greppy foo bar

tests:
	./build/tests

format:
	find ./src ./tests -name '*.cpp' -o -name '*.h' | xargs clang-format -i
