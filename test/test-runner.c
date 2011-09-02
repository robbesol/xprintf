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

#include "test-runner.h"
#include "test-rig.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void log_ID(const char *pre, const struct test_printf_info * tpip,
		const char *postFmt, ...) __attribute__ ((format (printf, 3, 4)));

static void log_ID(const char *pre, const struct test_printf_info *tpip,
		const char *postFmt, ...) {
	if (pre != 0) {
		fprintf(stderr, "%s", pre);
	}

	if (tpip == 0)
		fprintf(stderr, "[NULLname/NULLtest]");
	else if (tpip->test_name == 0)
		fprintf(stderr, "%s", tpip->test_subjectName);
	else
		fprintf(stderr, "%s/%s", tpip->test_subjectName, tpip->test_name);

	if (postFmt != 0) {
		va_list ap;
		va_start(ap, postFmt);
		vfprintf(stderr, postFmt, ap);
		va_end(ap);
	}
}

int handleProgError(const struct test_printf_info * tpi, const char *msg, ...) {
	fflush(stdout);

	fprintf(stderr, "\n##### PROGRAM error");
	if (tpi != 0) {
		fprintf(stderr, " with %s", tpi->test_subjectName);
		if (tpi->test_name != 0)
			fprintf(stderr, "/%s", tpi->test_name);
	}

	va_list ap;
	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);

	fprintf(stderr, "\n");
	fflush(stderr);
	exit(1);
	return -1;
}

#define REQUIRE_NON_NULL_ARGPOINTER(varname) while (varname == 0) \
		handleProgError(tpi, " %s() called with %s NULL\n", __func__, #varname);

static void initTPI(struct test_printf_info *tpi, const char name[],
		test_printf_p fn) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)
	REQUIRE_NON_NULL_ARGPOINTER(name)
	REQUIRE_NON_NULL_ARGPOINTER(fn)

	tpi->test_subjectName = name;
	tpi->test_name = 0;
	tpi->test_printf_p = fn;
	tpi->tests_total = -1;
	tpi->tests_failed = 0;
	tpi->tests_grandtotal = 0;
	tpi->tests_grandfailed = 0;
}

void startAllTests(struct test_printf_info **tpip, const char name[],
		test_printf_p fn) {
	if (tpip == 0) {
		handleProgError(0, " %s() called with tpip NULL\n", __func__);
	}

	struct test_printf_info *tpi = malloc(sizeof *tpi);
	if (tpi == 0) {
		fflush(stdout);
		fprintf(stderr,
				"##### error: malloc(sizeof (struct test_printf_info)) failed\n");
		fflush(stderr);
		exit(1);
	}

	initTPI(tpi, name, fn);
	if (tpi->tests_grandfailed < 0) {
		handleProgError(tpi, ": initTPI weird grand: %d/%d\n",
				tpi->tests_grandfailed, tpi->tests_grandtotal);
	}
	//	log_ID("\n----- ", tpi, ": start of tests\n");
	*tpip = tpi;
}

int endAllTests(struct test_printf_info *tpi) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)

	if (tpi->tests_grandtotal == 0) {
		log_ID("----- ", tpi, ": no tests done\n");
	} else if (tpi->tests_grandfailed > 0) {
		log_ID("----- ", tpi, " had errors: %d/%d\n", tpi->tests_grandfailed,
				tpi->tests_grandtotal);
	} else if (tpi->tests_grandfailed < 0) {
		log_ID("----- ", tpi, " weird: %d/%d\n", tpi->tests_grandfailed,
				tpi->tests_grandtotal);
	} else {
		log_ID("----- ", tpi, ": no errors\n");
	}

	if (tpi->tests_total >= 0) {
		handleProgError(tpi, ": last tests not endTests()ed");
	}

	return tpi->tests_grandfailed;
}

void startTests(struct test_printf_info * tpi, const char test_name[]) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)
	REQUIRE_NON_NULL_ARGPOINTER(test_name);
	if (tpi->tests_grandfailed < 0) {
		handleProgError(tpi, ": startTests(\"%s\") weird grand: %d/%d\n",
				test_name, tpi->tests_grandfailed, tpi->tests_grandtotal);
	}
	if (tpi->tests_total >= 0) {
		handleProgError(tpi, ": last tests not endTests()ed");
	}

	tpi->tests_total = 0;
	tpi->tests_failed = 0;
	tpi->test_name = test_name;

	const char name_prefix[] = "test_";
	const int name_prefix_len = strlen(name_prefix);
	if (0 == strncmp(test_name, name_prefix, name_prefix_len)) {
		tpi->test_name = test_name + name_prefix_len;
	}

	//	log_ID("\nStart of ", tpi, "\n");
}

int endTests(struct test_printf_info * tpi) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)
	if (tpi->tests_grandfailed < 0) {
		handleProgError(tpi, ": endTests(\"%s\") weird grand: %d/%d\n",
				tpi->test_name, tpi->tests_grandfailed, tpi->tests_grandtotal);
	}

	if (tpi->tests_failed > 0) {
		fprintf(stderr, "==========\n");
	}

	//	if (tpi->tests_failed > 0) {
	//		log_ID("\nErrors found in ", tpi, ": %d/%d\n", tpi->tests_failed,
	//				tpi->tests_total);
	//	} else {
	//		log_ID("No errors found in ", tpi, ": 0/%d\n", tpi->tests_total);
	//	}

	tpi->test_name = 0;
	tpi->tests_total = -1;
	tpi->tests_failed = 0;

	return tpi->tests_failed;
}

int compareResult(struct test_printf_info * tpi, const char *expected,
		int expectedLen, const char *format, const char *result, int resultLen) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)

	int testPassed;
	if (expectedLen < 0 || expected == 0) {
		// expecting an error
		testPassed = resultLen < 0;
	} else if (result == 0) {
		// unexpected null buffer
		testPassed = 0;
	} else {
		// we can compare expected with result
		testPassed = resultLen == expectedLen //
				&& 0 == memcmp(result, expected, expectedLen);
	}

	if (testPassed) {
		logSuccess(tpi, expected, expectedLen, format);
	} else {
		logErrorResult(tpi, expected, expectedLen, format, result, resultLen);
	}
	return testPassed;
}

void logSuccess(struct test_printf_info * tpi, const char *expected,
		int expectedLen, const char *format) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)

	(void) expected;
	(void) expectedLen;
	(void) format;

	tpi->tests_total++;
	tpi->tests_grandtotal++;
}

static void writeErrorHeader(struct test_printf_info * tpi, const char *format) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)

	tpi->tests_total++;
	tpi->tests_grandtotal++;
	tpi->tests_failed++;
	tpi->tests_grandfailed++;

	const char *posid = tpi->curtest_poshead;
	if (posid == 0) {
		posid = "[unknown file.pos]: ";
	}

	fprintf(stderr, "==========\n");
	log_ID(posid, tpi, " failed");
	if (format != 0) {
		fprintf(stderr, " for \"%s\"", format);
	}
}

void logErrorRemark(struct test_printf_info * tpi, const char *format,
		const char *msgFmt, ...) {
	writeErrorHeader(tpi, format);
	fprintf(stderr, " ");

	va_list va;
	va_start(va, msgFmt);
	vfprintf(stderr, msgFmt, va);
	va_end(va);
	fprintf(stderr, "\n");
}

void logErrorResult(struct test_printf_info * tpi, const char *expected,
		int expectedLen, const char *format, const char *result, int resultLen) {
	writeErrorHeader(tpi, format);

	char *head = tpi->curtest_poshead;
	if (head == 0) {
		head = "";
	}

	fprintf(stderr, "\n%s expected: %3d ", head, expectedLen);
	if (expected == 0) {
		fprintf(stderr, "null");
	} else {
		fprintf(stderr, "\"%.*s\"", expectedLen, expected);
	}
	fprintf(stderr, "\n%s      got: %3d ", head, resultLen);
	if (result == 0) {
		fprintf(stderr, "null");
	} else {
		fprintf(stderr, "\"%.*s\"", resultLen, result);
	}
	if (resultLen < 0) {
		fprintf(stderr, " -- returned ERRORCODE %d", resultLen);
	}
	fprintf(stderr, "\n");
}

int vtest(struct test_printf_info * tpi, char *xanswer,
		const char * restrict format, va_list ap) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)
	REQUIRE_NON_NULL_ARGPOINTER(xanswer)
	REQUIRE_NON_NULL_ARGPOINTER(tpi->test_printf_p)

	fflush(stdout);
	return tpi->test_printf_p(tpi, xanswer, strlen(xanswer), format, ap);
}

void test_setpos(const char curFile[], int curLine,
		struct test_printf_info * tpi) {
	REQUIRE_NON_NULL_ARGPOINTER(tpi)

	if (tpi->curtest_poshead != 0) {
		free(tpi->curtest_poshead);
	}
	tpi->curtest_poshead = 0;

	if (curFile == 0) {
		return;
	}

	if (asprintf(&tpi->curtest_poshead, "%s:%d: ", curFile, curLine) < 0) {
		tpi->curtest_poshead = 0;
	}
}

int test(const char curFile[], int curLine, struct test_printf_info * tpi,
		char *xanswer, const char * restrict format, ...) {
	test_setpos(curFile, curLine, tpi);
	va_list ap;
	va_start(ap, format);
	int result = vtest(tpi, xanswer, format, ap);
	va_end(ap);
	return result;
}

int vtestHost(struct test_printf_info * tpi, const char * restrict format,
		va_list ap) {
	REQUIRE_NON_NULL_ARGPOINTER(format)

	// get host answer
	char *xanswer;
	va_list xap;
	va_copy(xap, ap);
	int xansLen = vasprintf(&xanswer, format, xap);
	va_end(xap);
	if (xanswer == 0 || xansLen < 0) {
		logErrorRemark(tpi, format, "HOST problem:  vasprintf() failed: %d\n",
				xansLen);
		return 0;
	}

	int result = vtest(tpi, xanswer, format, ap);

	// just checking...
	if ((int) strlen(xanswer) != xansLen) {
		logErrorRemark(tpi, format,
				"HOST problem: vasprintf() = [%d] but result is [%d] \"%s\"\n",
				xansLen, (int) strlen(xanswer), xanswer);
	}

	free(xanswer);
	return result;
}

int testHost(char curFile[], int curLine, struct test_printf_info * tpi,
		const char * restrict format, ...) {
	test_setpos(curFile, curLine, tpi);
	va_list ap;
	va_start(ap, format);
	int result = vtestHost(tpi, format, ap);
	va_end(ap);
	return result;
}

#ifdef __GXX_ABI_VERSION
static const int onGcc = __GXX_ABI_VERSION;
#else
static const int onGcc = 0;
#endif

int isOnGcc(void) {
	return onGcc;
}

static int testingHost = 0;

void setTestingHost(int value) {
	testingHost = value;
}

int isTestingHost(void) {
	return testingHost;
}
