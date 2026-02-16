all: build

build:
	cmake -S . -B build
	$(MAKE) -C build

clean:
	rm -rf build

.PHONY: all build clean