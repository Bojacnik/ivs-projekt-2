/**
 * @file main.c
 * @brief Calculator GUI built with raylib + raygui.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "style_jungle.h"

#include "../backend/operation.h"

#define SCREEN_WIDTH 376
#define SCREEN_HEIGHT 456

/** Max characters on the display (keeps text inside the text box). */
#define DISPLAY_MAX_LEN 17
/** Significant digits shown for results. */
#define DISPLAY_DIGITS 10

/** Everything the user can do, via a button or the keyboard. */
typedef enum {
	ACT_NONE,
	ACT_DIGIT_0, ACT_DIGIT_1, ACT_DIGIT_2, ACT_DIGIT_3, ACT_DIGIT_4,
	ACT_DIGIT_5, ACT_DIGIT_6, ACT_DIGIT_7, ACT_DIGIT_8, ACT_DIGIT_9,
	ACT_DECIMAL,
	ACT_DELETE,      // remove last character
	ACT_CLEAR_ENTRY, // CE: clear current number
	ACT_CLEAR,       // C: clear everything
	ACT_ADD, ACT_SUB, ACT_MUL, ACT_DIV, ACT_POW, ACT_ROOT, // binary operations
	ACT_SIN, ACT_FACTORIAL,                                // unary operations
	ACT_EQUALS,
} Action;

typedef struct {
	Rectangle bounds;
	const char *label;
	Action action;
} Button;

static const Button BUTTONS[] = {
	{{32, 112, 120, 56}, "Del", ACT_DELETE},
	{{160, 112, 56, 56}, "CE", ACT_CLEAR_ENTRY},
	{{224, 112, 56, 56}, "C", ACT_CLEAR},
	{{288, 112, 56, 56}, "+", ACT_ADD},

	{{32, 176, 56, 56}, "Sin", ACT_SIN},
	{{96, 176, 56, 56}, "7", ACT_DIGIT_7},
	{{160, 176, 56, 56}, "8", ACT_DIGIT_8},
	{{224, 176, 56, 56}, "9", ACT_DIGIT_9},
	{{288, 176, 56, 56}, "-", ACT_SUB},

	{{32, 240, 56, 56}, "Root", ACT_ROOT},
	{{96, 240, 56, 56}, "4", ACT_DIGIT_4},
	{{160, 240, 56, 56}, "5", ACT_DIGIT_5},
	{{224, 240, 56, 56}, "6", ACT_DIGIT_6},
	{{288, 240, 56, 56}, "*", ACT_MUL},

	{{32, 304, 56, 56}, "x^y", ACT_POW},
	{{96, 304, 56, 56}, "1", ACT_DIGIT_1},
	{{160, 304, 56, 56}, "2", ACT_DIGIT_2},
	{{224, 304, 56, 56}, "3", ACT_DIGIT_3},
	{{288, 304, 56, 56}, "/", ACT_DIV},

	{{32, 368, 56, 56}, "n!", ACT_FACTORIAL},
	{{96, 368, 56, 56}, ".", ACT_DECIMAL},
	{{160, 368, 56, 56}, "0", ACT_DIGIT_0},
	{{224, 368, 120, 56}, "=", ACT_EQUALS},
};

typedef struct {
	char current[DISPLAY_MAX_LEN + 1];  // number being typed (or last result)
	char previous[DISPLAY_MAX_LEN + 1]; // left operand of the pending operation
	Action pending;                     // pending binary operation or ACT_NONE
	bool error;                         // display shows OP_ERROR
	bool showing_result;                // next digit starts a new number
} Calculator;

static bool is_binary(Action a) {
	return a >= ACT_ADD && a <= ACT_ROOT;
}

static bool has_number(const Calculator *calc) {
	return !calc->error && calc->current[0] != '\0' && strcmp(calc->current, "-") != 0;
}

static void set_current(Calculator *calc, const char *text) {
	strncpy(calc->current, text, DISPLAY_MAX_LEN);
	calc->current[DISPLAY_MAX_LEN] = '\0';
}

/** Applies an operation and returns a new string with the result. */
static char *compute(Action op, const char *a, const char *b) {
	switch (op) {
		case ACT_ADD: return op_add(a, b);
		case ACT_SUB: return op_sub(a, b);
		case ACT_MUL: return op_mul(a, b);
		case ACT_DIV: return op_div(a, b);
		case ACT_POW: return op_pow(a, b);
		case ACT_ROOT: return op_root(a, b);
		case ACT_SIN: return op_sin(a);
		case ACT_FACTORIAL: return op_factorial(a);
		default: return NULL;
	}
}

/** Computes @p op and shows its result (formatted for the display). */
static void show_result(Calculator *calc, Action op, const char *a, const char *b) {
	char *raw = compute(op, a, b);
	char *display = op_format(raw, DISPLAY_DIGITS);
	set_current(calc, display ? display : OP_ERROR);
	free(raw);
	free(display);

	calc->error = op_is_error(calc->current);
	calc->showing_result = true;
}

/** Evaluates the pending binary operation, if any. */
static void evaluate(Calculator *calc) {
	if (calc->pending == ACT_NONE || !has_number(calc)) {
		return;
	}
	show_result(calc, calc->pending, calc->previous, calc->current);
	calc->previous[0] = '\0';
	calc->pending = ACT_NONE;
}

/** Starts a fresh number when the display holds a result or an error. */
static void start_typing(Calculator *calc) {
	if (calc->error || calc->showing_result) {
		calc->current[0] = '\0';
		calc->error = false;
		calc->showing_result = false;
	}
}

static void append(Calculator *calc, const char *text) {
	if (strlen(calc->current) + strlen(text) <= DISPLAY_MAX_LEN) {
		strcat(calc->current, text);
	}
}

static void input_digit(Calculator *calc, char digit) {
	start_typing(calc);

	// Replace a lone leading zero instead of producing "05" or "-05".
	size_t len = strlen(calc->current);
	if (len > 0 && calc->current[len - 1] == '0' && (len == 1 || (len == 2 && calc->current[0] == '-'))) {
		calc->current[len - 1] = digit;
		return;
	}
	append(calc, (char[]) {digit, '\0'});
}

static void input_decimal(Calculator *calc) {
	start_typing(calc);
	if (strchr(calc->current, '.') != NULL) {
		return;
	}
	if (calc->current[0] == '\0' || strcmp(calc->current, "-") == 0) {
		append(calc, "0");
	}
	append(calc, ".");
}

static void input_binary(Calculator *calc, Action op) {
	// "-" with nothing typed starts a negative number.
	if (op == ACT_SUB && (calc->error || calc->current[0] == '\0') ) {
		calc->error = false;
		calc->showing_result = false;
		set_current(calc, "-");
		return;
	}
	if (calc->error) {
		return;
	}
	if (!has_number(calc)) {
		// Operator pressed twice in a row: just switch the operator.
		if (calc->pending != ACT_NONE) {
			calc->pending = op;
		}
		return;
	}

	evaluate(calc); // allow chaining: 2 + 3 * 4 = 20
	if (calc->error) {
		return;
	}
	strcpy(calc->previous, calc->current);
	calc->current[0] = '\0';
	calc->showing_result = false;
	calc->pending = op;
}

static void input_unary(Calculator *calc, Action op) {
	if (has_number(calc)) {
		char operand[sizeof calc->current];
		strcpy(operand, calc->current);
		show_result(calc, op, operand, NULL);
	}
}

static void handle(Calculator *calc, Action action) {
	if (action >= ACT_DIGIT_0 && action <= ACT_DIGIT_9) {
		input_digit(calc, (char) ('0' + (action - ACT_DIGIT_0)));
	} else if (is_binary(action)) {
		input_binary(calc, action);
	} else {
		switch (action) {
			case ACT_DECIMAL:
				input_decimal(calc);
				break;
			case ACT_DELETE:
				start_typing(calc);
				if (calc->current[0] != '\0') {
					calc->current[strlen(calc->current) - 1] = '\0';
				}
				break;
			case ACT_CLEAR_ENTRY:
				calc->current[0] = '\0';
				calc->error = false;
				calc->showing_result = false;
				break;
			case ACT_CLEAR:
				*calc = (Calculator) {0};
				break;
			case ACT_SIN:
			case ACT_FACTORIAL:
				input_unary(calc, action);
				break;
			case ACT_EQUALS:
				evaluate(calc);
				break;
			default:
				break;
		}
	}
}

/** Maps a typed character to an action. */
static Action action_for_char(int c) {
	if (c >= '0' && c <= '9') {
		return (Action) (ACT_DIGIT_0 + (c - '0'));
	}
	switch (c) {
		case '.': case ',': return ACT_DECIMAL;
		case '+': return ACT_ADD;
		case '-': return ACT_SUB;
		case '*': return ACT_MUL;
		case '/': return ACT_DIV;
		case '^': return ACT_POW;
		case '!': return ACT_FACTORIAL;
		case '=': return ACT_EQUALS;
		default: return ACT_NONE;
	}
}

static void handle_keyboard(Calculator *calc) {
	for (int c = GetCharPressed(); c != 0; c = GetCharPressed()) {
		handle(calc, action_for_char(c));
	}
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) handle(calc, ACT_EQUALS);
	if (IsKeyPressed(KEY_BACKSPACE)) handle(calc, ACT_DELETE);
	if (IsKeyPressed(KEY_DELETE)) handle(calc, ACT_CLEAR);
}

/**
 * @brief Draws one frame and handles a clicked button.
 */
static void draw_frame(Calculator *calc) {
	BeginDrawing();
	ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

	GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
	Action clicked = ACT_NONE;
	for (size_t i = 0; i < sizeof BUTTONS / sizeof BUTTONS[0]; i++) {
		if (GuiButton(BUTTONS[i].bounds, BUTTONS[i].label)) {
			clicked = BUTTONS[i].action;
		}
	}
	handle(calc, clicked);

	GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
	GuiTextBox((Rectangle) {32, 24, 312, 72}, calc->current, sizeof calc->current, false);

	EndDrawing();
}

int main(void) {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Calculator");
	SetTargetFPS(60);

	GuiLoadStyleJungle();
	GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_RIGHT);

	Calculator calc = {0};
	while (!WindowShouldClose()) {
		handle_keyboard(&calc);
		draw_frame(&calc);
	}

	CloseWindow();
	return 0;
}
