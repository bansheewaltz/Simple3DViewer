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

LINTFLAGS := --style=google
lint:     LINTFLAGS += -n
lint_fix: LINTFLAGS += -i
lint lint_fix:
	clang-format ${LINTFLAGS} $(shell find src \
	                                      -not -path "src/lib/extern/*" \
	                                      -type f \
													              \( -name '*.h'   -or -name '*.c' -or \
													                 -name '*.hpp' -or -name '*.cpp' \))
.PHONY: lint lint_fix

clean:
	$(RM) -r build
.PHONY: clean
