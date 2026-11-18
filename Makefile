CC = gcc

SRC_DIR   = src
TEST_DIR  = tests
BUILD_DIR = build
LIB_DIR   = lib
TMP_DIR   = tmp
PROF_DIR  = profiling
DOCS_DIR  = docs

INCLUDE = -L$(LIB_DIR) -I$(LIB_DIR)
BLIBS   = -lmpfr -lgmp -lm
FLIBS   = -lraylib

BACKEND_SRC   = $(SRC_DIR)/backend/operation.c
FRONTEND_SRC  = $(SRC_DIR)/frontend/main.c
PROFILING_SRC = $(SRC_DIR)/profiling/profiling.c
TEST_SRC      = $(TEST_DIR)/operation_tests.c

.DEFAULT_GOAL := all
.PHONY: all main run test deps dirs profiling prof doc pack clean clone_install_raylib clone_install_raygui

all: deps main run

dirs:
	mkdir -p $(TMP_DIR) $(LIB_DIR) $(BUILD_DIR)

deps: dirs clone_install_raylib clone_install_raygui

main: $(FRONTEND_SRC) $(BACKEND_SRC) | dirs
	$(CC) $(INCLUDE) $(FRONTEND_SRC) $(BACKEND_SRC) -o $(BUILD_DIR)/calc $(FLIBS) $(BLIBS)

run:
	./$(BUILD_DIR)/calc

test: | dirs
	$(CC) $(INCLUDE) $(TEST_SRC) $(BACKEND_SRC) -g3 -o $(BUILD_DIR)/test $(BLIBS)
	./$(BUILD_DIR)/test

profiling: $(PROFILING_SRC) $(BACKEND_SRC) | dirs
	$(CC) $(INCLUDE) $(PROFILING_SRC) $(BACKEND_SRC) -o $(BUILD_DIR)/profiling -pg $(BLIBS)

prof: profiling
	./$(BUILD_DIR)/profiling < $(PROF_DIR)/testp.txt && \
	gprof $(BUILD_DIR)/profiling gmon.out > $(PROF_DIR)/vystup.txt
	rm -f gmon.out

doc:
	doxygen Doxyfile && \
	$(MAKE) -C $(BUILD_DIR)/doc/latex && \
	cp $(BUILD_DIR)/doc/latex/refman.pdf $(DOCS_DIR)/doc.pdf

pack: xnemecv00.zip

xnemecv00.zip:
	rm -rf pack && mkdir -p pack
	cp $(DOCS_DIR)/project/hodnoceni.txt $(DOCS_DIR)/project/odevzdani.txt pack/
	cp -R $(SRC_DIR)/frontend $(SRC_DIR)/backend pack/
	cp $(TEST_SRC) $(PROFILING_SRC) pack/
	cp $(DOCS_DIR)/UzivatelskaDokumentace.pdf pack/
	zip -r $@ pack/
	rm -rf pack

clean:
	rm -rf $(LIB_DIR) $(TMP_DIR) $(BUILD_DIR) pack xnemecv00.zip gmon.out

clone_install_raylib:
	@if [ ! -f "$(LIB_DIR)/libraylib.a" ] || [ ! -f "$(LIB_DIR)/raylib.h" ]; then \
		cd $(TMP_DIR) && \
		git clone --depth 1 https://github.com/raysan5/raylib.git && \
		$(MAKE) -C raylib/src PLATFORM=PLATFORM_DESKTOP && \
		cp raylib/src/libraylib.a raylib/src/raylib.h ../$(LIB_DIR)/; \
	else \
		echo "Raylib has already been cloned from GitHub. Skipping installation!"; \
	fi

clone_install_raygui:
	@if [ ! -f "$(LIB_DIR)/raygui.h" ] || [ ! -f "$(LIB_DIR)/style_jungle.h" ]; then \
		cd $(TMP_DIR) && \
		git clone --depth 1 https://github.com/raysan5/raygui.git && \
		cp raygui/src/raygui.h ../$(LIB_DIR)/ && \
		cp raygui/styles/jungle/style_jungle.h ../$(LIB_DIR)/style_jungle.h; \
	else \
		echo "Raygui has already been cloned from GitHub. Skipping installation!"; \
	fi
