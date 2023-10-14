CC := gcc
BUILD_DIR := build


all: build
.PHONY: all

config:
	mkdir -p $(BUILD_DIR)
	cmake -B $(BUILD_DIR)
.PHONY: config

build: config
	cmake --build $(BUILD_DIR)
.PHONY: build

# verbose build
buildv: config
	VERBOSE=1 cmake --build $(BUILD_DIR)
.PHONY: buildc

lint:
	clang-format --style=google -n $(shell find src \
	                                           -not -path "src/lib/extern/*" \
	                                           -type f \
																						 \( -name '*.h' -or -name '*.c' \))
.PHONY: lint

clean:
	$(RM) -r build
.PHONY: clean
