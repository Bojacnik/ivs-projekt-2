CC = gcc
#CFLAGS = -Iinclude/raygui/src/

#this spits out raylib.a
#mv libraylib.a ../../lib/.

#now clone raygui
#now move raygui.h

.PHONY: all main clean clone_install_raylib clone_install_raygui

all: clone_install_raylib clone_install_raygui main test

main: main.c
	$(CC) $(CFLAGS) main.c -o main -L../lib -lraylib -lm
	./main

clean:
	rm -f main

clone_install_raylib:
	@if [ ! -d "../tmp/raylib" ]; then \
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
	@if [ ! -d "../tmp/raygui" ]; then \
		mkdir -p ../tmp && \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raygui.git && \
		cp ../tmp/raygui/src/raygui.h ../src/.; \
	else \
		echo "Raygui has already been cloned from GitHUB. Skipping installation!"; \
	fi

test:
	echo $(OS);
