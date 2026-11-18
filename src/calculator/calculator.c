/**
 * @file calculator.c
 * @brief Calculator state machine.
 */
#include "calculator.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_binary(CalcAction a) { return a >= CALC_ADD && a <= CALC_ROOT; }

static bool is_unary(CalcAction a) { return a >= CALC_SIN && a <= CALC_FACTORIAL; }

/** Symbol of a binary operation as shown on the expression line. */
static const char *symbol(CalcAction op) {
	switch (op) {
		case CALC_ADD: return "+";
		case CALC_SUB: return "-";
		case CALC_MUL: return "*";
		case CALC_DIV: return "/";
		case CALC_POW: return "^";
		case CALC_ROOT: return "root";
		default: return "";
	}
}

static bool has_number(const Calculator *calc) {
	return !calc->error && calc->display[0] != '\0' && strcmp(calc->display, "-") != 0;
}

static void set_display(Calculator *calc, const char *text) {
	snprintf(calc->display, sizeof calc->display, "%s", text);
}

/** Expression line for a pending operation: "2 +" or "" if none. */
static void set_pending_expression(Calculator *calc) {
	if (calc->pending == CALC_NONE) {
		calc->expression[0] = '\0';
	} else {
		snprintf(calc->expression, sizeof calc->expression, "%s %s", calc->previous, symbol(calc->pending));
	}
}

/** Applies an operation and returns a new string with the full-precision result. */
static char *compute(const Calculator *calc, CalcAction op, const char *a, const char *b) {
	switch (op) {
		case CALC_ADD: return op_add(a, b);
		case CALC_SUB: return op_sub(a, b);
		case CALC_MUL: return op_mul(a, b);
		case CALC_DIV: return op_div(a, b);
		case CALC_POW: return op_pow(a, b);
		case CALC_ROOT: return op_root(a, b);
		case CALC_SIN: return op_sin(a, calc->angle_unit);
		case CALC_COS: return op_cos(a, calc->angle_unit);
		case CALC_TAN: return op_tan(a, calc->angle_unit);
		case CALC_FACTORIAL: return op_factorial(a);
		default: return NULL;
	}
}

/** Computes @p op and shows its result rounded for the display. */
static void show_result(Calculator *calc, CalcAction op, const char *a, const char *b) {
	char *raw = compute(calc, op, a, b);
	char *display = op_format(raw, CALC_DISPLAY_DIGITS);
	set_display(calc, display ? display : OP_ERROR);
	free(raw);
	free(display);

	calc->error = op_is_error(calc->display);
	calc->showing_result = true;
}

/** Evaluates the pending binary operation, if any. */
static void evaluate(Calculator *calc) {
	if (calc->pending == CALC_NONE || !has_number(calc)) {
		return;
	}
	snprintf(calc->expression, sizeof calc->expression, "%s %s %s =", calc->previous, symbol(calc->pending),
	         calc->display);
	show_result(calc, calc->pending, calc->previous, calc->display);
	calc->previous[0] = '\0';
	calc->pending = CALC_NONE;
}

/** Starts a fresh number when the display holds a result or an error. */
static void start_typing(Calculator *calc) {
	if (calc->error || calc->showing_result) {
		calc->display[0] = '\0';
		calc->error = false;
		calc->showing_result = false;
		set_pending_expression(calc);
	}
}

static void append(Calculator *calc, const char *text) {
	if (strlen(calc->display) + strlen(text) <= CALC_DISPLAY_MAX_LEN) {
		strcat(calc->display, text);
	}
}

static void input_digit(Calculator *calc, char digit) {
	start_typing(calc);

	// Replace a lone leading zero instead of producing "05" or "-05".
	char *number = calc->display[0] == '-' ? calc->display + 1 : calc->display;
	if (strcmp(number, "0") == 0) {
		number[0] = digit;
		return;
	}
	append(calc, (char[]){digit, '\0'});
}

static void input_decimal(Calculator *calc) {
	start_typing(calc);
	if (strchr(calc->display, '.') != NULL) {
		return;
	}
	if (calc->display[0] == '\0' || strcmp(calc->display, "-") == 0) {
		append(calc, "0");
	}
	append(calc, ".");
}

static void input_negate(Calculator *calc) {
	if (calc->error) {
		return;
	}
	size_t len = strlen(calc->display);
	if (calc->display[0] == '-') {
		memmove(calc->display, calc->display + 1, len); // includes '\0'
	} else if (len < CALC_DISPLAY_MAX_LEN && strcmp(calc->display, "0") != 0) {
		memmove(calc->display + 1, calc->display, len + 1);
		calc->display[0] = '-';
	}
}

static void input_binary(Calculator *calc, CalcAction op) {
	// "-" with nothing typed starts a negative number (also for the 2nd operand).
	if (op == CALC_SUB && (calc->error || calc->display[0] == '\0')) {
		if (calc->error) {
			calc_handle(calc, CALC_CLEAR_ENTRY);
		}
		set_display(calc, "-");
		return;
	}
	if (calc->error) {
		return;
	}
	if (!has_number(calc)) {
		// Operator pressed right after another one: switch the operator.
		if (calc->pending != CALC_NONE) {
			calc->display[0] = '\0';
			calc->pending = op;
			set_pending_expression(calc);
		}
		return;
	}

	evaluate(calc); // allow chaining: 2 + 3 * 4 = 20
	if (calc->error) {
		return;
	}
	strcpy(calc->previous, calc->display);
	calc->display[0] = '\0';
	calc->showing_result = false;
	calc->pending = op;
	set_pending_expression(calc);
}

static void input_unary(Calculator *calc, CalcAction op) {
	if (!has_number(calc)) {
		return;
	}
	char operand[sizeof calc->display];
	strcpy(operand, calc->display);

	char call[CALC_DISPLAY_MAX_LEN + 8];
	if (op == CALC_FACTORIAL) {
		snprintf(call, sizeof call, "%s!", operand);
	} else {
		const char *name = op == CALC_SIN ? "sin" : op == CALC_COS ? "cos" : "tan";
		snprintf(call, sizeof call, "%s(%s)", name, operand);
	}
	if (calc->pending != CALC_NONE) {
		snprintf(calc->expression, sizeof calc->expression, "%s %s %s", calc->previous, symbol(calc->pending), call);
	} else {
		snprintf(calc->expression, sizeof calc->expression, "%s =", call);
	}

	show_result(calc, op, operand, NULL);
}

static void input_delete(Calculator *calc) {
	if (calc->error || calc->showing_result) {
		start_typing(calc);
	} else if (calc->display[0] != '\0') {
		calc->display[strlen(calc->display) - 1] = '\0';
	}
}

void calc_init(Calculator *calc) {
	*calc = (Calculator){0};
	calc->pending = CALC_NONE;
	calc->angle_unit = OP_RADIANS;
}

void calc_handle(Calculator *calc, CalcAction action) {
	if (action >= CALC_DIGIT_0 && action <= CALC_DIGIT_9) {
		input_digit(calc, (char)('0' + (action - CALC_DIGIT_0)));
	} else if (is_binary(action)) {
		input_binary(calc, action);
	} else if (is_unary(action)) {
		input_unary(calc, action);
	} else {
		switch (action) {
			case CALC_DECIMAL: input_decimal(calc); break;
			case CALC_NEGATE: input_negate(calc); break;
			case CALC_DELETE: input_delete(calc); break;
			case CALC_CLEAR_ENTRY:
				calc->display[0] = '\0';
				calc->error = false;
				calc->showing_result = false;
				set_pending_expression(calc);
				break;
			case CALC_CLEAR: {
				OpAngleUnit unit = calc->angle_unit;
				calc_init(calc);
				calc->angle_unit = unit;
				break;
			}
			case CALC_TOGGLE_ANGLE_UNIT:
				calc->angle_unit = calc->angle_unit == OP_RADIANS ? OP_DEGREES : OP_RADIANS;
				break;
			case CALC_EQUALS: evaluate(calc); break;
			default: break;
		}
	}
}

CalcAction calc_action_for_char(int c) {
	if (c >= '0' && c <= '9') {
		return (CalcAction)(CALC_DIGIT_0 + (c - '0'));
	}
	switch (tolower(c)) {
		case '.':
		case ',': return CALC_DECIMAL;
		case '+': return CALC_ADD;
		case '-': return CALC_SUB;
		case '*': return CALC_MUL;
		case '/': return CALC_DIV;
		case '^': return CALC_POW;
		case '!': return CALC_FACTORIAL;
		case '=': return CALC_EQUALS;
		case 's': return CALC_SIN;
		case 'c': return CALC_COS;
		case 't': return CALC_TAN;
		case 'r': return CALC_ROOT;
		case 'n': return CALC_NEGATE;
		case 'd': return CALC_TOGGLE_ANGLE_UNIT;
		default: return CALC_NONE;
	}
}

bool calc_paste(Calculator *calc, const char *text) {
	if (text == NULL) {
		return false;
	}
	char buffer[128];
	while (isspace((unsigned char)*text)) {
		text++;
	}
	if (snprintf(buffer, sizeof buffer, "%s", text) >= (int)sizeof buffer) {
		return false; // too long; truncating would change the number
	}
	for (size_t len = strlen(buffer); len > 0 && isspace((unsigned char)buffer[len - 1]); len--) {
		buffer[len - 1] = '\0';
	}

	char *number = op_format(buffer, CALC_DISPLAY_DIGITS);
	bool ok = !op_is_error(number) && strlen(number) <= CALC_DISPLAY_MAX_LEN;
	if (ok) {
		calc->error = false;
		set_display(calc, number);
		calc->showing_result = true; // typing a digit replaces the pasted number
		set_pending_expression(calc);
	}
	free(number);
	return ok;
}
