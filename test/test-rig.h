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
