all: clean build
.PHONY: clean build

clean :
	$(MAKE) clean -C build

build:
	cmake -DCMAKE_C_COMPILER=/usr/local/bin/arm-none-eabi-gcc -DCMAKE_BUILD_TYPE=Debug -Bbuild && $(MAKE) -C build
