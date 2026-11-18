/**
 * @file calculator_tests.c
 * @brief Tests of the calculator state machine (src/calculator), no GUI needed.
 */
#include "../src/calculator/calculator.h"
#include "test_util.h"

/**
 * @brief Feeds key presses to a fresh calculator.
 *
 * Characters go through calc_action_for_char(), except these test-only
 * codes: D = Del, E = CE, C = C.
 */
static Calculator press(const char *keys) {
	Calculator calc;
	calc_init(&calc);
	for (const char *k = keys; *k != '\0'; k++) {
		switch (*k) {
			case 'D': calc_handle(&calc, CALC_DELETE); break;
			case 'E': calc_handle(&calc, CALC_CLEAR_ENTRY); break;
			case 'C': calc_handle(&calc, CALC_CLEAR); break;
			default: calc_handle(&calc, calc_action_for_char(*k)); break;
		}
	}
	return calc;
}

static void expect(const char *keys, const char *display, const char *expression) {
	Calculator calc = press(keys);
	char what[128];
	snprintf(what, sizeof what, "display after \"%s\"", keys);
	check_str(what, calc.display, display);
	snprintf(what, sizeof what, "expression after \"%s\"", keys);
	check_str(what, calc.expression, expression);
}

static void test_typing(void) {
	expect("123", "123", "");
	expect("0005", "5", "");
	expect("-0005", "-5", "");
	expect("..5.", "0.5", "");
	expect("-.5", "-0.5", "");
	expect("12D", "1", "");
	expect("DDD5", "5", "");
	expect("123E", "", "");
	expect("123456789012345678901", "12345678901234567", ""); // length limit
	expect("5n", "-5", "");
	expect("5nn", "5", "");
	expect("n", "-", "");
	expect("0n", "0", "");
}

static void test_binary_operations(void) {
	expect("0.1+0.2=", "0.3", "0.1 + 0.2 =");
	expect("7-10=", "-3", "7 - 10 =");
	expect("6*7=", "42", "6 * 7 =");
	expect("514/3=", "171.3333333", "514 / 3 =");
	expect("2^10=", "1024", "2 ^ 10 =");
	expect("9r2=", "3", "9 root 2 =");
	expect("-8r3=", "-2", "-8 root 3 =");
	expect("2+", "", "2 +");
	expect("2+3", "3", "2 +");
	expect("2*-3=", "-6", "2 * -3 =");
}

static void test_chaining(void) {
	expect("2+3*4=", "20", "5 * 4 =");
	expect("2+3*", "", "5 *");
	expect("2+*3=", "6", "2 * 3 ="); // second operator replaces the first
	expect("2^10=+1=", "1025", "1024 + 1 =");
	expect("5=", "5", ""); // nothing pending
}

static void test_unary_operations(void) {
	expect("3!", "6", "3! =");
	expect("3!!", "720", "6! =");
	expect("2+3!=", "8", "2 + 6 =");
	expect("2+3!", "6", "2 + 3!");
	expect("0s", "0", "sin(0) =");
	expect("d30s", "0.5", "sin(30) =");
	expect("d60c", "0.5", "cos(60) =");
	expect("d45t", "1", "tan(45) =");
	expect("dd30s", "-0.9880316241", "sin(30) ="); // back to radians
	expect("s", "", "");                           // nothing to apply it to
}

static void test_results_and_errors(void) {
	expect("1/0=", OP_ERROR, "1 / 0 =");
	expect("2.5!", OP_ERROR, "2.5! =");
	expect("d90t", OP_ERROR, "tan(90) =");
	expect("1/0=5", "5", "");             // digit after an error starts over
	expect("1/0=+", OP_ERROR, "1 / 0 ="); // operators ignored on error
	expect("1/0=-5", "-5", "");           // minus starts a negative number
	expect("1/0=n", OP_ERROR, "1 / 0 =");
	expect("1/0=D", "", "");
	expect("5+5=7", "7", ""); // digit after a result starts over
	expect("5+5=n", "-10", "5 + 5 =");
	expect("5+5=D", "", "");
	expect("2+3=C", "", "");
	expect("2+E", "", "2 +"); // CE keeps the pending operation
	expect("2+3E4=", "6", "2 + 4 =");
}

static void test_clear_keeps_angle_unit(void) {
	Calculator calc = press("d5C");
	check_true("C keeps degrees", calc.angle_unit == OP_DEGREES);
	calc = press("d");
	check_true("d switches to degrees", calc.angle_unit == OP_DEGREES);
	calc = press("");
	check_true("radians by default", calc.angle_unit == OP_RADIANS);
}

static void test_paste(void) {
	Calculator calc = press("2+");
	check_true("paste number", calc_paste(&calc, "  12.50\n"));
	check_str("pasted display", calc.display, "12.5");
	calc_handle(&calc, CALC_EQUALS);
	check_str("pasted operand used", calc.display, "14.5");

	calc = press("7");
	check_true("reject text", !calc_paste(&calc, "hello"));
	check_true("reject NULL", !calc_paste(&calc, NULL));
	check_str("rejected paste keeps display", calc.display, "7");

	char long_number[200];
	memset(long_number, '1', sizeof long_number - 1);
	long_number[sizeof long_number - 1] = '\0';
	check_true("reject over-long text", !calc_paste(&calc, long_number));

	calc = press("1/0=");
	check_true("paste clears error", calc_paste(&calc, "3"));
	check_true("no error after paste", !calc.error);
}

static void test_key_mapping(void) {
	check_true("'7' is digit 7", calc_action_for_char('7') == CALC_DIGIT_7);
	check_true("',' is decimal", calc_action_for_char(',') == CALC_DECIMAL);
	check_true("'S' is sin", calc_action_for_char('S') == CALC_SIN);
	check_true("'x' does nothing", calc_action_for_char('x') == CALC_NONE);
}

int main(void) {
	test_typing();
	test_binary_operations();
	test_chaining();
	test_unary_operations();
	test_results_and_errors();
	test_clear_keeps_angle_unit();
	test_paste();
	test_key_mapping();
	return test_report("calculator");
}
