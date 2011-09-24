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

#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_

#include <stdarg.h>

struct test_printf_info;

void logSuccess(struct test_printf_info *, const char *expected,
		int expectedLen, const char *format);

void logErrorRemark(struct test_printf_info *, const char *format,
		const char *msgFmt, ...);

void logErrorResult(struct test_printf_info *, const char *expected,
		int expectedLen, const char *format, const char *result, int resultLen);

int handleProgError(const struct test_printf_info * tpi, const char *msg, ...) __attribute__ ((format (printf, 2, 3)));

/**
 * initializes counters for the next test run
 */
void startTests(struct test_printf_info * tpi, const char test_name[]);

/**
 * return the number of errors
 */
int endTests(struct test_printf_info *);

/**
 * test a format and args against the result xanswer
 */
int test(const char curFile[], int curLine, struct test_printf_info *,
		char *xanswer, const char * restrict format, ...);

/**
 * test a format and args against the result returned by the host
 */
int testHost(char curFile[], int curLine, struct test_printf_info *,
		const char * restrict format, ...);

/**
 * test a format and va_list against the result xanswer
 */
int vtest(struct test_printf_info *, char *xanswer,
		const char * restrict format, va_list ap);

/**
 * test a format and va_list against the result returned by the host
 */
int vtestHost(struct test_printf_info *, const char * restrict format,
		va_list ap);

int isTestingHost(void);

void test_setpos(const char curFile[], int curLine,
		struct test_printf_info * tpi);

#define TEST(...)			test(__FILE__, __LINE__, tpi, __VA_ARGS__)
#define TESTHOST(...)		testHost(__FILE__, __LINE__, tpi, __VA_ARGS__)
#define TEST_SAME(fmt, ...)	TEST(fmt, fmt, __VA_ARGS__);
#define TEST_SETPOSNEXT()	test_setpos(__FILE__, __LINE__ + 1, tpi)

#endif /* TESTRUNNER_H_ */
