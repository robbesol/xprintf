/*
 * Copyright(c) 2009 Technologies88 B.V.
 * All rights reserved.
 */

#include "test-rig.h"

extern test_printf_fn vtest_xprintf_core;

#include "impl/xprintf-core.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

struct testChannel {
	char *buf;
	int len;
};

static int test__write_to_buf(struct xprintf_protectedChannel *xchpr,
		const char *pData, unsigned int length) {
	struct testChannel *xchp = xchpr->channel;

	if (xchp->buf == 0)
		return -1; // return value <0 indicates error

	// add chars to buf
	int newBufLen = xchp->len + length + 1; // includes a terminating '\0'!
	xchp->buf = realloc(xchp->buf, newBufLen);
	if (xchp->buf == 0)
		return -1; // return value <0 indicates error

	// append new data
	memcpy(xchp->buf + xchp->len, pData, length);
	xchp->len += length;
	xchp->buf[xchp->len] = '\0';

	return length;
}

/* *
 * Calls xvprintf_protected(), then calls compareResult() to judge the result to the expected.
 *
 * @param tpi passed to compareResult()
 * @param expected what the result should be
 * @param expectedLen length of \c expected
 * @param format passed to xvprintf_protected()
 * @param ap passed to xvprintf_protected()
 * @return returnalv ue of compareResult()
 */
int vtest_xprintf_core(struct test_printf_info *tpi,
		const char *expected, int expectedLen, const char *format, va_list ap) {
	// set up protected channel with an rempty buffer
	struct testChannel xch = { .buf = malloc(1), .len = 0 };
	struct xprintf_protectedChannel xchpr = { .channel = &xch,
			.writeProtected = test__write_to_buf };

	// run xvprintf_protected()
	int count = xvprintf_protected(&xchpr, format, ap);

	// judge result
	int result = compareResult(tpi, expected, expectedLen, format,
			xch.buf, count);

	// clean up
	if (xch.buf != 0)
		free(xch.buf);

	return result;
}
