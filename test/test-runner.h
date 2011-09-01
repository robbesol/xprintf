/*
 * Copyright(c) 2009 Technologies88 B.V.
 * All rights reserved.
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

int isOnGcc(void);
int isTestingHost(void);

void test_setpos(const char curFile[], int curLine,
		struct test_printf_info * tpi);

#define TEST(...)			test(__FILE__, __LINE__, tpi, __VA_ARGS__)
#define TESTHOST(...)		testHost(__FILE__, __LINE__, tpi, __VA_ARGS__)
#define TEST_SAME(fmt, ...)	TEST(fmt, fmt, __VA_ARGS__);
#define TEST_SETPOSNEXT()	test_setpos(__FILE__, __LINE__ + 1, tpi)

#endif /* TESTRUNNER_H_ */
