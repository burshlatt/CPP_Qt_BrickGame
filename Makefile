.PHONY: all install open uninstall

all: install open

install: uninstall
	mkdir build
	cd build && cmake ..
	cd build && cmake --build .

open:
	@echo --------------------- OPEN ----------------------
	build/BrickGame
	@echo --------------------- OPEN ----------------------

uninstall:
	@echo ------------------- UNINSTALL -------------------
	rm -rf build
	@echo ------------------- UNINSTALL -------------------
