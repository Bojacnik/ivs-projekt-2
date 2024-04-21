CC = gcc
#CFLAGS = -Iinclude/raygui/src/

.PHONY: test all main temp deps clean clone_install_raylib clone_install_raygui clean_all backend clone_install_gmp clone_install_mpfr

all: deps main

deps: tmp_folders clone_install_gmp clone_install_mpfr clone_install_raylib clone_install_raygui

test:
	gcc backend/operation_tests.c backend/operation.c -g3 -o tests -L../lib -lraylib -lmpfr -lgmp -lm
	./tests

tmp_folders:
	mkdir -p ../tmp
	mkdir -p ../lib


main: frontend/main.c
	$(CC) $(CFLAGS) frontend/main.c backend/operation.c -o main -L../lib -lraylib -lmpfr -lgmp -lm
	cp backend/operation.h frontend/.
	./main

clone_install_gmp:
	@if [ ! -f "../lib/libgmp.a" ] || [ ! -f "backend/gmp.h" ]; then \
		cd ../tmp && \
		git clone --depth 1 https://github.com/alisw/GMP.git && \
		cd GMP && ./configure --disable-shared && \
		make  &&\
		mv gmp.h ../../src/backend/gmp.h && \
		mv .libs/libgmp.a ../../lib/libgmp.a; \
	else \
		echo "GMP already installed. Skipping..."; \
	fi

clone_install_mpfr:
	@if [ ! -f "../lib/libmpfr.a" ] || [ ! -f "backend/mpfr.h" ]; then \
      	cd ../tmp && \
      	git clone --depth 1 https://github.com/alisw/MPFR && \
		cd MPFR && ./configure --enable-static && \
		autoreconf -f -i && \
		make && \
		mv src/mpfr.h ../../src/backend/mpfr.h && \
		mv src/.libs/libmpfr.a ../../lib/libmpfr.a;\
   	else \
    		echo "MPFR already installed. Skipping..."; \
  	fi

clone_install_raylib:
	@if [ ! -f "../lib/libraylib.a" ] || [ ! -f "frontend/raylib.h" ]; then \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raylib.git && \
		cd raylib/src && \
		make PLATFORM=PLATFORM_DESKTOP && \
		mv libraylib.a ../../../lib/ && \
		cp raylib.h ../../../src/frontend/.; \
	else \
		echo "Raylib has already been cloned from GitHUB. Skipping installation!"; \
	fi

clone_install_raygui:
	@if [ ! -f "frontend/raygui.h" ] || [ ! -f "frontend/style_jungle.h" ]; then \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raygui.git && \
		cp ../tmp/raygui/src/raygui.h ../src/frontend/. && \
		cp ../tmp/raygui/styles/jungle/style_jungle.h ../src/frontend/style_jungle.h; \
	else \
		echo "Raygui has already been cloned from GitHUB. Skipping installation!"; \
	fi

clean:
	rm -f frontend/main
	rm -f frontend/operation.h
	rm -f frontend/style_jungle.h
	rm -f raylib.h
	rm -f raygui.h
	rm -f backend/gmp.h
	rm -f backend/mpfr.h
	rm -r -f ../lib
	rm -r -f ../tmp