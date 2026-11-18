CC     = gcc
CFLAGS = -Wall -Wextra -std=gnu11

# make SANITIZE=1 test  -> build with AddressSanitizer + UndefinedBehaviorSanitizer
ifeq ($(SANITIZE),1)
CFLAGS += -g -fsanitize=address,undefined -fno-omit-frame-pointer
endif

# Pinned GUI dependency versions (git tags)
RAYLIB_VERSION = 6.2
RAYGUI_VERSION = 5.0

# Build GLFW for X11 only. With Wayland enabled, GNOME (which has no server-side
# decorations) shows the window without a title bar or close/minimize buttons
# unless libdecor is installed. Via XWayland, GNOME draws the normal title bar.
RAYLIB_FLAGS = GLFW_LINUX_ENABLE_WAYLAND=FALSE GLFW_LINUX_ENABLE_X11=TRUE

SRC_DIR   = src
TEST_DIR  = tests
BUILD_DIR = build
LIB_DIR   = lib
TMP_DIR   = tmp
PROF_DIR  = profiling
DOCS_DIR  = docs

INCLUDE = -L$(LIB_DIR) -I$(LIB_DIR)
BLIBS   = -lmpfr -lgmp -lm
FLIBS   = -lraylib -lGL -lpthread -ldl -lrt -lX11

BACKEND_SRC    = $(SRC_DIR)/backend/operation.c
CALCULATOR_SRC = $(SRC_DIR)/calculator/calculator.c
FRONTEND_SRC   = $(SRC_DIR)/frontend/main.c
PROFILING_SRC  = $(SRC_DIR)/profiling/profiling.c

FORMAT_FILES = $(wildcard $(SRC_DIR)/*/*.c $(SRC_DIR)/*/*.h $(TEST_DIR)/*.c $(TEST_DIR)/*.h)
CLANG_FORMAT ?= clang-format-18

RAYLIB_STAMP = $(LIB_DIR)/.raylib-$(RAYLIB_VERSION)
RAYGUI_STAMP = $(LIB_DIR)/.raygui-$(RAYGUI_VERSION)

.DEFAULT_GOAL := all
.PHONY: all main run test deps profiling prof doc doc-html package pack format format-check clean

all: deps main run

## Dependencies (raylib + raygui), downloaded once per pinned version ##

deps: $(RAYLIB_STAMP) $(RAYGUI_STAMP)

$(RAYLIB_STAMP):
	rm -rf $(TMP_DIR)/raylib $(LIB_DIR)/.raylib-* && mkdir -p $(TMP_DIR) $(LIB_DIR)
	git clone --depth 1 --branch $(RAYLIB_VERSION) https://github.com/raysan5/raylib.git $(TMP_DIR)/raylib
	$(MAKE) -C $(TMP_DIR)/raylib/src PLATFORM=PLATFORM_DESKTOP $(RAYLIB_FLAGS)
	cp $(TMP_DIR)/raylib/src/libraylib.a $(TMP_DIR)/raylib/src/raylib.h $(LIB_DIR)/
	touch $@

$(RAYGUI_STAMP):
	rm -rf $(TMP_DIR)/raygui $(LIB_DIR)/.raygui-* && mkdir -p $(TMP_DIR) $(LIB_DIR)
	git clone --depth 1 --branch $(RAYGUI_VERSION) https://github.com/raysan5/raygui.git $(TMP_DIR)/raygui
	cp $(TMP_DIR)/raygui/src/raygui.h $(TMP_DIR)/raygui/styles/jungle/style_jungle.h $(LIB_DIR)/
	touch $@

## Build ##

$(BUILD_DIR):
	mkdir -p $@

main: deps | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) $(FRONTEND_SRC) $(CALCULATOR_SRC) $(BACKEND_SRC) -o $(BUILD_DIR)/calc $(FLIBS) $(BLIBS)

run:
	./$(BUILD_DIR)/calc

test: | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_DIR)/operation_tests.c $(BACKEND_SRC) -o $(BUILD_DIR)/operation_tests $(BLIBS)
	$(CC) $(CFLAGS) $(TEST_DIR)/calculator_tests.c $(CALCULATOR_SRC) $(BACKEND_SRC) -o $(BUILD_DIR)/calculator_tests $(BLIBS)
	./$(BUILD_DIR)/operation_tests
	./$(BUILD_DIR)/calculator_tests

profiling: | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(PROFILING_SRC) $(BACKEND_SRC) -o $(BUILD_DIR)/profiling -pg $(BLIBS)

prof: profiling
	./$(BUILD_DIR)/profiling < $(PROF_DIR)/testp.txt && \
	gprof $(BUILD_DIR)/profiling gmon.out > $(PROF_DIR)/vystup.txt
	rm -f gmon.out

## Documentation (Doxygen) ##

doc-html: | $(BUILD_DIR)
	doxygen Doxyfile

# Requires a LaTeX installation; result: build/doc.pdf
doc: doc-html
	$(MAKE) -C $(BUILD_DIR)/doc/latex
	cp $(BUILD_DIR)/doc/latex/refman.pdf $(BUILD_DIR)/doc.pdf

## Packaging ##

# Debian package via CPack; result: build/package/*.deb
package: main profiling
	cmake -S . -B $(BUILD_DIR)/cmake
	cd $(BUILD_DIR)/cmake && cpack -B ../package

# Archive for the IVS course submission
pack: xnemecv00.zip

xnemecv00.zip:
	rm -rf pack && mkdir -p pack
	cp $(DOCS_DIR)/project/hodnoceni.txt $(DOCS_DIR)/project/odevzdani.txt pack/
	cp -R $(SRC_DIR)/* pack/
	cp $(TEST_DIR)/* pack/
	cp $(DOCS_DIR)/UzivatelskaDokumentace.pdf pack/
	zip -r $@ pack/
	rm -rf pack

## Code style ##

format:
	$(CLANG_FORMAT) -i $(FORMAT_FILES)

format-check:
	$(CLANG_FORMAT) --dry-run --Werror $(FORMAT_FILES)

clean:
	rm -rf $(LIB_DIR) $(TMP_DIR) $(BUILD_DIR) pack xnemecv00.zip gmon.out
