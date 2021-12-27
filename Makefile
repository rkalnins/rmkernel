all : fullclean clean build format docs view_docs
.PHONY: fullclean clean build format docs view_docs

fullclean :
	rm -rf build/

clean :
	$(MAKE) clean -C build

build :
	cmake -DOS_PORT=arm-cortex-m4 -DCMAKE_C_COMPILER=/usr/local/bin/arm-none-eabi-gcc -DCMAKE_BUILD_TYPE=Debug -Bbuild && $(MAKE) -C build

format :
	./tools/format.sh

docs :
	doxygen

view_docs :
	open docs/html/index.html
