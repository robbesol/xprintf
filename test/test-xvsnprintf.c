/*
 * testing xvsnprintf()
 */

#include "test-rig.h"
#include "xsprintf.h"

static test_printf_fn test_xprintf;

static int test_xprintf(struct test_printf_info *tpi,
		const char *expected, int expectedLen, const char *format, va_list ap) {
	char result[200] = "wef";
	int resultLen = xvsnprintf(result, sizeof result, format, ap);
	return compareResult(tpi, expected, expectedLen, format,
			result, resultLen);
}

int main(void) {
	DEFINE_test_printf_info("xvsnprintf()", test_xprintf);

	test_all_iso(tpi);

	if (endAllTests(tpi) != 0) {
		return 1;
	}

	test_pointers(tpi);
	return 0;
}
