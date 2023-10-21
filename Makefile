all: debug

GENERATOR ?=
NAME := app
BUILD_DIR := build
DEBUG_DIR := $(BUILD_DIR)/debug
TARGET := build/debug/$(NAME)

ifeq ($(GEN),ninja)
	GENERATOR=-G "Ninja"
endif

run: all
	$(TARGET)

release:
	@mkdir -p build/release && \
	cd build/release && \
	cmake $(GENERATOR) -DCMAKE_BUILD_TYPE=Release ../.. && \
	cmake --build . --config Release

debug:
	@mkdir -p build/debug && \
	cd build/debug && \
	cmake $(GENERATOR) -DCMAKE_BUILD_TYPE=Debug ../.. && \
	cmake --build . --config Debug

test: debug
	$(DEBUG_DIR)/test/test

clean:
	rm -rf build

format:
	find src/ -iname *.cpp | xargs clang-format --sort-includes=0 -style=file -i
	find lib/ -iname *.cpp | xargs clang-format --sort-includes=0 -style=file -i
	find includes/shell -iname *.hpp | xargs clang-format --sort-includes=0 -style=file -i
	cmake-format -i CMakeLists.txt
