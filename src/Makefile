CC = gcc
#CFLAGS = -Iinclude/raygui/src/

#this spits out raylib.a
#mv libraylib.a ../../lib/.

.PHONY: all main clean clone_install_raylib clone_install_raygui

all: clone_install_raylib clone_install_raygui main

main: main.c
	$(CC) $(CFLAGS) main.c -o main -L../lib -lraylib -lm
	rm -f -r ../tmp

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
	rm -f main
	rm -f raylib.h
	rm -f raygui.h
	rm -r -f ../lib
	rm -r -f ../tmp