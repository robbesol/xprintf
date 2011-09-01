#ifndef TESTRIG_H_
#define TESTRIG_H_

#include <stdarg.h>

struct test_printf_info;

typedef int test_printf_fn(struct test_printf_info *tpi, const char *expected,
		int expectedLen, const char *format, va_list ap);

typedef test_printf_fn *test_printf_p;

struct test_printf_info {
	const char * test_subjectName;
	const char *test_name;
	test_printf_p test_printf_p;
	int tests_total;
	int tests_failed;
	int tests_grandtotal;
	int tests_grandfailed;
	char *curtest_poshead;
};

void startAllTests(struct test_printf_info **tpi, const char name[],
		test_printf_p fn);
int endAllTests(struct test_printf_info *tpi);

#define DEFINE_test_printf_info(name, fn) struct test_printf_info *tpi; startAllTests(&tpi, name, fn);

void test_iso_ints(struct test_printf_info *);
void test_iso_strings(struct test_printf_info *);
void test_iso_doubles(struct test_printf_info *);
void test_nodoubles(struct test_printf_info *);
void test_BrunoHaible(struct test_printf_info *);

void test_all_iso(struct test_printf_info *);

void test_pointers(struct test_printf_info *);

int compareResult(struct test_printf_info *, const char *expected,
		int expectedLen, const char *format, const char *result, int resultLen);

void setTestingHost(int value);

#endif /* TESTRIG_H_ */
