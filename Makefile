all: build

build:
	cmake -S . -B build
	$(MAKE) -C build

clean:
	rm -rf build

comparison:
	rm -rf tmp.txt
	for i in $$(find ./src -type f); do \
		echo "// =====================================" >> tmp.txt; \
		echo "// $$i" >> tmp.txt; \
		cat $$i >> tmp.txt; \
		echo >> tmp.txt; \
		echo >> tmp.txt; \
	done

.PHONY: all build clean