/**
 * @file calculator.h
 * @brief Calculator state machine, independent of any GUI library.
 *
 * The UI translates button clicks and key presses into CalcAction values,
 * passes them to calc_handle() and draws Calculator::display and
 * Calculator::expression.
 */
#pragma once

#include <stdbool.h>

#include "../backend/operation.h"

/** Maximum number of characters in the main display. */
#define CALC_DISPLAY_MAX_LEN 17
/** Significant digits shown for results. */
#define CALC_DISPLAY_DIGITS 10
/** Maximum length of the expression line ("2 + 3 ="). */
#define CALC_EXPRESSION_MAX_LEN 63

/** Everything the user can do. */
typedef enum {
	CALC_NONE,
	CALC_DIGIT_0,
	CALC_DIGIT_1,
	CALC_DIGIT_2,
	CALC_DIGIT_3,
	CALC_DIGIT_4,
	CALC_DIGIT_5,
	CALC_DIGIT_6,
	CALC_DIGIT_7,
	CALC_DIGIT_8,
	CALC_DIGIT_9,
	CALC_DECIMAL,
	CALC_NEGATE,      ///< toggle the sign of the current number
	CALC_DELETE,      ///< remove the last character
	CALC_CLEAR_ENTRY, ///< CE: clear the current number
	CALC_CLEAR,       ///< C: clear everything
	// binary operations
	CALC_ADD,
	CALC_SUB,
	CALC_MUL,
	CALC_DIV,
	CALC_POW,
	CALC_ROOT,
	// unary operations
	CALC_SIN,
	CALC_COS,
	CALC_TAN,
	CALC_FACTORIAL,
	CALC_TOGGLE_ANGLE_UNIT, ///< switch between radians and degrees
	CALC_EQUALS,
} CalcAction;

typedef struct {
	char display[CALC_DISPLAY_MAX_LEN + 1];       ///< number being typed or last result
	char expression[CALC_EXPRESSION_MAX_LEN + 1]; ///< what is being computed, e.g. "2 +"
	char previous[CALC_DISPLAY_MAX_LEN + 1];      ///< left operand of the pending operation
	CalcAction pending;                           ///< pending binary operation or CALC_NONE
	OpAngleUnit angle_unit;                       ///< unit used by sin, cos and tan
	bool error;                                   ///< display shows OP_ERROR
	bool showing_result;                          ///< next digit starts a new number
} Calculator;

/** Resets the calculator to its initial state (radians, empty display). */
void calc_init(Calculator *calc);

/** Applies one user action. */
void calc_handle(Calculator *calc, CalcAction action);

/**
 * @brief Maps a typed character to an action.
 *
 * Digits, `. , + - * / ^ ! =` map to the matching keys; letters are
 * shortcuts: s = sin, c = cos, t = tan, r = root, n = negate,
 * d = degrees/radians.
 * @return CALC_NONE for characters without a meaning.
 */
CalcAction calc_action_for_char(int c);

/**
 * @brief Puts a pasted number on the display.
 * @return false (and leaves the state unchanged) if @p text is not a number.
 */
bool calc_paste(Calculator *calc, const char *text);
