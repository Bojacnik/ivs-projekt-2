CC = gcc
#CFLAGS = -Iinclude/raygui/src/

#this spits out raylib.a
#mv libraylib.a ../../lib/.

#now clone raygui
#now move raygui.h

#i was playing around, not doing anything currently
#SHELL ?= shell
#OpS ?= Windows_NT;
#ifeq ($(OS), Windows_NT)
#	OpS = $(OS)
#	SHELL = cmd
#else
#	OpS = LINUX
#	SHELL = shell
#endif



.PHONY: all main clean clone_install_raylib clone_install_raygui

all: clone_install_raylib clone_install_raygui main test

windows: clone_install_raylib_windows clone_install_raygui_windows main

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

clone_install_raylib_windows:
	IF NOT EXIST %cd%\..\lib\libralib.a ( \
		MKDIR ..\tmp &&\
		MKDIR ..\lib && \
		cd %cd%\..\tmp && git clone --depth 1 https://github.com/raysan5/raylib.git && cd raylib\src && mingw32-make.exe PLATFORM=PLATFORM_DESKTOP && \
		move %cd%\..\tmp\raylib\src\libraylib.a %cd%\..\lib && \
		copy %cd%\..\tmp\raylib\src\raylib.h %cd%\raylib.h &&\
		echo RAYLIB SUCCESSFULLY INSTALLED)

clone_install_raygui:
	@if [ ! -d "../tmp/raygui" ]; then \
		mkdir -p ../tmp && \
		cd ../tmp && \
		git clone --depth 1 https://github.com/raysan5/raygui.git && \
		cp ../tmp/raygui/src/raygui.h ../src/.; \
	else \
		echo "Raygui has already been cloned from GitHUB. Skipping installation!"; \
	fi

clone_install_raygui_windows:
	IF NOT EXIST %cd%/raygui.h ( \
		cd %cd%\..\tmp && git clone https://github.com/raysan5/raygui.git && \
		copy %cd%\..\tmp\raygui\src\raygui.h %cd%\raygui.h && \
		echo RAYGUI INSTALLED)


cleanup_windows:
	IF EXIST %cd%\..\tmp (\
	RMDIR /S /Q ..\tmp &&\
	echo FILES CLEANED UP\
	)