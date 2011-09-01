/*
 * Copyright (c) 2009 Technologies88 B.V.
 * All rights reserved.
 */

#include "test-rig.h"

#include <stdio.h>
#include <stdlib.h>

static test_printf_fn test_xprintf;

static int test_xprintf(struct test_printf_info *tpi, const char *expected,
		int expectedLen, const char *format, va_list ap) {
	char *resultString;
	int resultLen = vasprintf(&resultString, format, ap);
	int result = compareResult(tpi, expected, expectedLen, format, resultString, resultLen);
	free(resultString);
	return result;
}

int main(void) {
	setTestingHost(1);
	DEFINE_test_printf_info("vasprintf(HOST)", test_xprintf);

	test_all_iso(tpi);

	return endAllTests(tpi) != 0; // return 1 in case of errors
}
