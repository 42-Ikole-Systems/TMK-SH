all: debug

GENERATOR ?=

ifeq ($(GEN),ninja)
	GENERATOR=-G "Ninja"
endif

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

clean:
	rm -rf build

format:
	find src/ -iname *.cpp | xargs clang-format --sort-includes=0 -style=file -i
	find lib/ -iname *.cpp | xargs clang-format --sort-includes=0 -style=file -i
	find includes/ -iname *.hpp | xargs clang-format --sort-includes=0 -style=file -i
	cmake-format -i CMakeLists.txt
