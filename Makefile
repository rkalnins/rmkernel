all: fullclean clean build format
.PHONY: fullclean clean build format

fullclean :
	rm -rf build/ debug/

clean :
	$(MAKE) clean -C build

build :
	cmake -DOS_PORT=arm-cortex-m4 -DCMAKE_C_COMPILER=/usr/local/bin/arm-none-eabi-gcc -DCMAKE_BUILD_TYPE=Debug -Bbuild && $(MAKE) -C build

format :
	./tools/format.sh
