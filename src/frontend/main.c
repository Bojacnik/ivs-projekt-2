/**
 * @file main.c
 * @brief Calculator GUI: draws the calculator and forwards input to it.
 *
 * All calculator behaviour lives in src/calculator; this file only maps
 * buttons and keys to CalcAction values and renders the state.
 */
#include <stddef.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_jungle.h"

#include "../calculator/calculator.h"

#define SCREEN_WIDTH  376
#define SCREEN_HEIGHT 528

#define MARGIN   32
#define CELL     64  // grid pitch
#define BUTTON   56  // button size
#define GRID_TOP 136 // y of the first button row

#define TEXT_SIZE_BUTTON     20
#define TEXT_SIZE_DISPLAY    28
#define TEXT_SIZE_EXPRESSION 20

typedef struct {
	int col, row, width; // grid position, width in cells
	const char *label;
	CalcAction action;
} Button;

static const Button BUTTONS[] = {
        {0, 0, 2, "Deg/Rad", CALC_TOGGLE_ANGLE_UNIT},
        {2, 0, 1, "Cos", CALC_COS},
        {3, 0, 1, "Tan", CALC_TAN},
        {4, 0, 1, "+/-", CALC_NEGATE},

        {0, 1, 2, "Del", CALC_DELETE},
        {2, 1, 1, "CE", CALC_CLEAR_ENTRY},
        {3, 1, 1, "C", CALC_CLEAR},
        {4, 1, 1, "+", CALC_ADD},

        {0, 2, 1, "Sin", CALC_SIN},
        {1, 2, 1, "7", CALC_DIGIT_7},
        {2, 2, 1, "8", CALC_DIGIT_8},
        {3, 2, 1, "9", CALC_DIGIT_9},
        {4, 2, 1, "-", CALC_SUB},

        {0, 3, 1, "Root", CALC_ROOT},
        {1, 3, 1, "4", CALC_DIGIT_4},
        {2, 3, 1, "5", CALC_DIGIT_5},
        {3, 3, 1, "6", CALC_DIGIT_6},
        {4, 3, 1, "*", CALC_MUL},

        {0, 4, 1, "x^y", CALC_POW},
        {1, 4, 1, "1", CALC_DIGIT_1},
        {2, 4, 1, "2", CALC_DIGIT_2},
        {3, 4, 1, "3", CALC_DIGIT_3},
        {4, 4, 1, "/", CALC_DIV},

        {0, 5, 1, "n!", CALC_FACTORIAL},
        {1, 5, 1, ".", CALC_DECIMAL},
        {2, 5, 1, "0", CALC_DIGIT_0},
        {3, 5, 2, "=", CALC_EQUALS},
};

static Rectangle button_bounds(const Button *b) {
	return (Rectangle){
	        MARGIN + b->col * CELL,
	        GRID_TOP + b->row * CELL,
	        b->width * CELL - (CELL - BUTTON),
	        BUTTON,
	};
}

static bool ctrl_down(void) { return IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL); }

static void handle_keyboard(Calculator *calc) {
	if (ctrl_down()) {
		if (IsKeyPressed(KEY_C) && !calc->error && calc->display[0] != '\0') {
			SetClipboardText(calc->display);
		}
		if (IsKeyPressed(KEY_V)) {
			calc_paste(calc, GetClipboardText());
		}
		while (GetCharPressed() != 0) {
			// drop characters typed together with Ctrl
		}
		return;
	}

	for (int c = GetCharPressed(); c != 0; c = GetCharPressed()) {
		calc_handle(calc, calc_action_for_char(c));
	}
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) calc_handle(calc, CALC_EQUALS);
	if (IsKeyPressed(KEY_BACKSPACE)) calc_handle(calc, CALC_DELETE);
	if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_ESCAPE)) calc_handle(calc, CALC_CLEAR);
}

/** Draws the text right-aligned, cutting it from the left if it is too wide. */
static void draw_expression(const char *text, Rectangle bounds) {
	GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_SIZE_EXPRESSION);
	const float spacing = (float)GuiGetStyle(DEFAULT, TEXT_SPACING);
	while (*text != '\0' && MeasureTextEx(GuiGetFont(), text, TEXT_SIZE_EXPRESSION, spacing).x > bounds.width) {
		text++;
	}
	GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);
	GuiLabel(bounds, text);
	GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
}

/** Draws one frame and applies a clicked button. */
static void draw_frame(Calculator *calc) {
	BeginDrawing();
	ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

	// Display: angle unit + expression line above the current number.
	const float width = SCREEN_WIDTH - 2 * MARGIN;
	GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_SIZE_EXPRESSION);
	GuiLabel((Rectangle){MARGIN, 16, 40, 24}, calc->angle_unit == OP_DEGREES ? "DEG" : "RAD");
	draw_expression(calc->expression, (Rectangle){MARGIN + 40, 16, width - 40, 24});

	GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_SIZE_DISPLAY);
	GuiTextBox((Rectangle){MARGIN, 44, width, 72}, calc->display, sizeof calc->display, false);

	GuiSetStyle(DEFAULT, TEXT_SIZE, TEXT_SIZE_BUTTON);
	CalcAction clicked = CALC_NONE;
	for (size_t i = 0; i < sizeof BUTTONS / sizeof BUTTONS[0]; i++) {
		const Button *b = &BUTTONS[i];
		if (GuiButton(button_bounds(b), b->label)) {
			clicked = b->action;
		}
	}
	calc_handle(calc, clicked);

	EndDrawing();
}

int main(void) {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Calculator");
	SetTargetFPS(60);
	SetExitKey(KEY_NULL); // Esc clears instead of closing the window

	GuiLoadStyleJungle();
	GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);

	Calculator calc;
	calc_init(&calc);
	while (!WindowShouldClose()) {
		handle_keyboard(&calc);
		draw_frame(&calc);
	}

	CloseWindow();
	return 0;
}
