.PHONY: purge clean build format docs view_docs

build:
	cmake -DOS_PORT=arm-cortex-m4 -DCMAKE_C_COMPILER=/usr/local/bin/arm-none-eabi-gcc -DCMAKE_BUILD_TYPE=Debug -Bbuild && $(MAKE) -C build

purge:
	rm -rf build/

clean:
	$(MAKE) clean -C build

format:
	./tools/format.sh

docs:
	doxygen

view_docs:
	open docs/html/index.html
