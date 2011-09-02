/*
 * Copyright (c) 2007-2011 Markus van Dijk
 * All rights reserved.
 *
 * This file is part of the xprintf project.
 * The xprintf project is open source software and distributed
 * under the terms of the Simplified BSD License:
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *	*	Redistributions of source code must retain the above copyright notice,
 * 		this list of conditions and the following disclaimer.
 *	*	Redistributions in binary form must reproduce the above copyright notice,
 * 		this list of conditions and the following disclaimer in the documentation
 * 		and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
