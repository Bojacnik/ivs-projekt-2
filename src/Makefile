CC = gcc
#CFLAGS = -Iinclude/raygui/src/


all: clone_install_gmp clone_install_mpfr clone_install_raylib clone_install_raygui backend main

.PHONY: all main clean clone_install_raylib clone_install_raygui clean_all backend clone_install_gmp clone_install_mpfr

backend: backend/operation.c
	$(CC) backend.c -lm -lgmp

main: frontend/main.c
	$(CC) $(CFLAGS) frontend/main.c -o main -L../lib -lraylib -lm
	rm -f -r ../tmp
	./main

clone_install_gmp:
	@if [ ! -f "../lib/libgmp.a" ] || [ ! -f "backend/gmp.h" ]; then \
		mkdir -p ../tmp && \
		cd ../tmp && \
		git clone --depth 1 https://github.com/alisw/GMP.git && \
		cd GMP && ./configure --disable-shared && \
		make && \
		mv ../tmp/GMP/gmp.h ../src/backend/gmp.h && \
		mv .libs/libgmp.a ../../lib/libgmp.a; \
	else \
		echo "MPFR already installed. Skipping..."; \
	fi

clone_install_mpfr:
	@if [ ! -f "../lib/libmpfr.a" ] || [ ! -f "backend/mpfr.h" ]; then \
      	mkdir -p ../tmp && \
      	cd ../tmp && \
      	git clone --depth 1 https://github.com/alisw/MPFR && \
		cd MPFR && ./configure --enable-static && \
		autoreconf -f -i && \
		make && \
		mv mpfr.h ../../src/backend/mpfr.h && \
		mv .libs/libmpfr.a ../../lib/libmpfr.a;\
   	else \
    		echo "MPFR already installed. Skipping..."; \
  	fi

clone_install_raylib:
	@if [ ! -f "../lib/libraylib.a" ] || [ ! -f "raylib.h" ]; then \
		mkdir -p ../tmp && \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raylib.git && \
		cd raylib/src && \
		make PLATFORM=PLATFORM_DESKTOP && \
		mkdir ../../../lib && \
		mv libraylib.a ../../../lib/ && \
		cp raylib.h ../../../src/.; \
	else \
		echo "Raylib has already been cloned from GitHUB. Skipping installation!"; \
	fi

clone_install_raygui:
	@if [ ! -f "raygui.h" ] || [ ! -f "../lib/style_jungle.h" ]; then \
		mkdir -p ../tmp && \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raygui.git && \
		cp ../tmp/raygui/src/raygui.h ../src/. && \
		mv ../tmp/raygui/styles/jungle/style_jungle.h ../lib/style_jungle.h; \
	else \
		echo "Raygui has already been cloned from GitHUB. Skipping installation!"; \
	fi

clean:
	rm -f main

clean_all:
	rm -f frontend/main
	rm -f raylib.h
	rm -f raygui.h
	rm -f backend/gmp.h
	rm -f backend/mpfr.h
	rm -r -f ../lib/*
	rm -r -f ../tmp