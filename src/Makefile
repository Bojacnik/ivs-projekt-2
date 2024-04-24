CC = gcc
#CFLAGS = -Iinclude/raygui/src/

.PHONY: test all main temp_folders deps profiling clean clone_install_raylib clone_install_raygui clean_all backend clone_install_gmp clone_install_mpfr

all: deps main run

deps: tmp_folders clone_install_gmp clone_install_mpfr clone_install_raylib clone_install_raygui

test:
	gcc backend/operation_tests.c backend/operation.c -g3 -o test -L../lib -lraylib -lmpfr -lgmp -lm
	./test

tmp_folders:
	mkdir -p ../tmp
	mkdir -p ../lib
	mkdir -p ../build

doc:
	rm -f -r doc.pdf
	doxygen Doxyfile && \
	cd latex && \
	pdflatex refman.tex && \
	mv refman.pdf ../doc.pdf;
	rm -f -r latex
	rm -f -r html

run:
	./calc

main: frontend/main.c
	$(CC) $(CFLAGS) frontend/main.c backend/operation.c -o ../build/calc -L../lib -lraylib -lmpfr -lgmp -lm

prof: ../profiling/profiling.c backend/operation.c
	$(CC) ../profiling/profiling.c backend/operation.c -o ../profiling/profiling -pg -lmpfr -lgmp -lm
	../profiling/profiling < ../profiling/testp.txt
	gprof ../profiling/profiling gmon.out > analysis.txt

clone_install_gmp:
	@if [ ! -f "../lib/libgmp.a" ] || [ ! -f "../lib/gmp.h" ]; then \
		cd ../tmp && \
		git clone --depth 1 https://github.com/alisw/GMP.git && \
		cd GMP && ./configure --disable-shared && \
		make  &&\
		cp gmp.h ../../lib/gmp.h && \
		cp .libs/libgmp.a ../../lib/libgmp.a; \
	else \
		echo "GMP already installed. Skipping..."; \
	fi

clone_install_mpfr:
	@if [ ! -f "../lib/libmpfr.a" ] || [ ! -f "../lib/mpfr.h" ]; then \
      	cd ../tmp && \
      	git clone --depth 1 https://github.com/alisw/MPFR && \
		cd MPFR && ./configure --enable-static && \
		autoreconf -f -i && \
		make && \
		cp src/mpfr.h ../../lib/mpfr.h && \
		cp src/.libs/libmpfr.a ../../lib/libmpfr.a;\
   	else \
    		echo "MPFR already installed. Skipping..."; \
  	fi

clone_install_raylib:
	@if [ ! -f "../lib/libraylib.a" ] || [ ! -f "../lib/raylib.h" ]; then \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raylib.git && \
		cd raylib/src && \
		make PLATFORM=PLATFORM_DESKTOP && \
		mv libraylib.a ../../../lib/ && \
		cp raylib.h ../../../lib/.; \
	else \
		echo "Raylib has already been cloned from GitHUB. Skipping installation!"; \
	fi

clone_install_raygui:
	@if [ ! -f "../lib/raygui.h" ] || [ ! -f "../lib/style_jungle.h" ]; then \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raygui.git && \
		cp ../tmp/raygui/src/raygui.h ../lib/. && \
		cp ../tmp/raygui/styles/jungle/style_jungle.h ../lib/style_jungle.h; \
	else \
		echo "Raygui has already been cloned from GitHUB. Skipping installation!"; \
	fi

pack:


clean:
	rm -f test
	rm -f calc
	rm -f frontend/main
	rm -f frontend/style_jungle.h
	rm -f raylib.h
	rm -f raygui.h
	rm -f backend/gmp.h
	rm -f backend/mpfr.h
	rm -r -f ../lib
	rm -r -f ../tmp
	rm -r -f ../profiling/profiling
	rm -r -f ../build

