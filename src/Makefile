CC = gcc
INCLUDE = -L../lib -I../lib
BLIBS = -lmpfr -lgmp -lm
FLIBS = -lraylib

.DEFAULT_TARGET: all
.PHONY: test all main temp_folders deps profiling clean clone_install_raylib clone_install_raygui clean_all backend xnemecv00.zip

all: deps main run

pack: xnemecv00.zip
	mkdir -p ../pack
	cp ../hodnoceni.txt ../pack/.
	cp -R frontend ../pack
	cp -R backend ../pack
	cp ../profiling/profiling.c ../pack/.
	cp ../UzivatelskaDokumentace.pdf ../pack/.
	cp ../odevzdani.txt ../pack/.
	zip -r xnemecv00.zip ../pack/
	rm -f -r ../pack

test:
	$(CC) $(INCLUDE) backend/operation_tests.c backend/operation.c -g3 -o test $(FLIBS) $(BLIBS)
	./test

deps: tmp_folders clone_install_raylib clone_install_raygui


tmp_folders:
	mkdir -p ../tmp
	mkdir -p ../lib
	mkdir -p ../build

doc:
	rm -f -r doc.pdf
	doxygen Doxyfile && \
	cd latex && \
	pdflatex refman.tex && \
	mv refman.pdf ../../doc.pdf;
	rm -f -r latex
	rm -f -r html

run:
	../build/calc

main: frontend/main.c
	$(CC) $(INCLUDE) frontend/main.c backend/operation.c -o ../build/calc $(FLIBS) $(BLIBS)

prof: ../profiling/profiling.c backend/operation.c
	$(CC) $(INCLUDE) ../profiling/profiling.c backend/operation.c -o ../profiling/profiling -pg $(BLIBS) && \
	../profiling/profiling < ../profiling/testp.txt && \
	gprof ../profiling/profiling gmon.out > ../profiling/vystup.txt
	rm -f gmon.out

clean:
	rm -r -f ../lib
	rm -r -f ../tmp
	rm -r -f ../build
	rm -r -f ../profiling/profiling
	rm -r -f ../pack
	rm -r -f backend/operation_tests

clone_install_raylib:
	@if [ ! -f "../lib/libraylib.a" ] || [ ! -f "../lib/raylib.h" ]; then \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raylib.git && \
		cd raylib/src && \
		make PLATFORM=PLATFORM_DESKTOP && \
		cp libraylib.a ../../../lib/ && \
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




