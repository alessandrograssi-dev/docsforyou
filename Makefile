IMAGE_NAME ?= docsforyou:latest
CONTAINER_NAME ?= docsforyou
PORT ?= 8080
DOCKER ?= docker

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

docker-check:
	$(DOCKER) info > /dev/null

docker-build: docker-check
	$(DOCKER) build -t $(IMAGE_NAME) .

docker-run: docker-check
	$(DOCKER) run --rm -d --name $(CONTAINER_NAME) -p $(PORT):8080 $(IMAGE_NAME)

docker-stop:
	$(DOCKER) stop $(CONTAINER_NAME)

docker-logs:
	$(DOCKER) logs -f $(CONTAINER_NAME)

.PHONY: all build clean comparison docker-check docker-build docker-run docker-stop docker-logs