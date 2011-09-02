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

#include <math.h>
#include <stdio.h>

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "test-runner.h"
#include "test-rig.h"

#include <math.h>

void test_iso_doubles(struct test_printf_info *tpi) {
	startTests(tpi, __func__);

	//    /* %f, basic formattting */

	TEST("0.000000", "%f", (double) 0);
	TEST("-0.000000", "%f", - (double) 0);

	TEST("3.141593", "%f", M_PI);
	TEST("3.14159265", "%.8f", M_PI);
	TEST("3", "%.0f", M_PI);
	TEST("3", "%1.f", M_PI);
	TEST("  3", "%3.f", M_PI);
	TEST("0", "%0.0f", 0.32);
	TEST("5.555550", "%.6f", 5.55555);
	TEST("5.55555", "%.5f", 5.55555);
	TEST("5.5556", "%.4f", 5.55555);
	TEST("6", "%.0f", 5.55555);
	/* %f, flags */
	TEST("+3.141593", "%+f", M_PI);
	TEST(" 3.141593", "% f", M_PI);
	TEST("3.", "%#.0f", M_PI);
	TEST("03.14", "%05.2f", M_PI);

	// %f, negative
	TEST("-3.141593", "%f", -M_PI);
	TEST("-3.14159265", "%.8f", -M_PI);
	TEST("-3", "%.0f", -M_PI);
	TEST("-3", "%1.f", -M_PI);
	TEST(" -3", "%3.f", -M_PI);
	TEST("0", "%0.0f", 0.32);
	TEST("-5.555550", "%.6f", -5.55555);
	TEST("-5.55555", "%.5f", -5.55555);
	TEST("-5.5556", "%.4f", -5.55555);
	TEST("-6", "%.0f", -5.55555);
	/* %f, flags */
	TEST("-3.141593", "%+f", -M_PI);
	TEST("-3.141593", "% f", -M_PI);
	TEST(" -3.141593", "% 10f", -M_PI);
	TEST("-3.", "%#.0f", -M_PI);
	TEST("-3.14", "%05.2f", -M_PI);
	TEST("-03.14", "%06.2f", -M_PI);

	// leading zero in frac
	TEST("03.021", "%06.3f", 3.02134);
	TEST("-3.021", "%06.3f", -3.02134);
	TEST("-03.021", "%07.3f", -3.02134);

	TEST("nan", "%f", NAN);
	TEST("nan", "%.f", NAN);
	TEST("nan", "%.0f", NAN);
	TEST("nan", "%3.0f", NAN);
	TEST("    nan", "%7f", NAN);
	TEST("    nan", "%7.f", NAN);
	TEST("    nan", "%7.0f", NAN);
	TEST("    nan", "%7.2f", NAN);
	TEST("    nan", "%7.8f", NAN);
	TEST("    nan", "% 7f", NAN);
	TEST("    nan", "% 7.f", NAN);
	TEST("    nan", "% 7.0f", NAN);
	TEST("    nan", "% 7.2f", NAN);
	TEST("    nan", "% 7.8f", NAN);
	TEST("inf", "%f", INFINITY);
	TEST("inf", "%.f", INFINITY);
	TEST("inf", "%.0f", INFINITY);

	TEST("nan", "%0f", NAN);
	TEST("nan", "%0.f", NAN);
	TEST("nan", "%0.0f", NAN);
	TEST("inf", "%0f", INFINITY);
	TEST("inf", "%0.f", INFINITY);
	TEST("inf", "%0.0f", INFINITY);

	// these tests with INF and NAN and "%0f" fail on Mac OSX !
	if (isTestingHost()) {
		fprintf(
				stderr,
				"----- not running NAN and INF tests for \"%%0f\" because GCC's libc is broken (v %d)\n",
				isOnGcc());
	} else {
		TEST("    nan", "%07f", NAN);
		TEST("    inf", "%07f", INFINITY);
		TEST("    nan", "%07.f", NAN);
		TEST("    inf", "%07.f", INFINITY);
		TEST("    nan", "%07.0f", NAN);
		TEST("    inf", "%07.0f", INFINITY);
		TEST("    nan", "%07.2f", NAN);
		TEST("    inf", "%07.2f", INFINITY);
		TEST("    nan", "%07.8f", NAN);
		TEST("    inf", "%07.8f", INFINITY);
	}

	TEST("nan", "%e", NAN);
	TEST("NAN", "%E", NAN);
	TEST("nan", "%.e", NAN);
	TEST("nan", "%.0e", NAN);
	TEST("nan", "%3.0e", NAN);
	TEST("    nan", "%7e", NAN);
	TEST("    nan", "%7.e", NAN);
	TEST("    nan", "%7.0e", NAN);
	TEST("    nan", "%7.2e", NAN);
	TEST("    nan", "%7.8e", NAN);
	TEST("    nan", "% 7e", NAN);
	TEST("    nan", "% 7.e", NAN);
	TEST("    nan", "% 7.0e", NAN);
	TEST("    nan", "% 7.2e", NAN);
	TEST("    nan", "% 7.8e", NAN);
	TEST("INF", "%E", INFINITY);
	TEST("inf", "%.e", INFINITY);
	TEST("inf", "%.0e", INFINITY);

	TEST("0.000000e+00", "%e", (double) 0);
	TEST("-0.000000e+00", "%e", -(double) 0);
	TEST("3.141593e+00", "%e", M_PI);

	TEST("0.000000E+00", "%E", (double) 0);
	TEST("-0.000000E+00", "%E", -(double) 0);
	TEST("3.141593E+00", "%E", M_PI);

	TEST("3.14159265e+00", "%.8e", M_PI);
	TEST("3e+00", "%.0e", M_PI);
	TEST("0.0e+00", "%.1e", 0.0);
	TEST("1.0e-05", "%.1e", 0.00001);
	TEST("-1.0e-05", "%.1e", -0.00001);
	TEST("1.0e+04", "%.1e", 10000.0);
	TEST("-1.0e+04", "%.1e", -10000.0);
	/* %e, flags */
	TEST("+3.141593e+00", "%+e", M_PI);
	TEST(" 3.141593e+00", "% e", M_PI);
	TEST("3.e+00", "%#.0e", M_PI);
	TEST("03.14e+00", "%09.2e", M_PI);

	// g
	TEST("0", "%g", (double) 0);
	TEST("-0", "%g", -(double) 0);
	TEST("3.14159", "%g", M_PI);

	TEST("0", "%G", (double) 0);
	TEST("-0", "%G", -(double) 0);
	TEST("3.14159", "%G", M_PI);

	TEST("1.00000e+06", "%.5e", (double) 1000001);
	TEST("1.000001e+06", "%.6e", (double) 1000001);

	/*
	 * test exponent for small delta
	 */

	const double base = 0.0000001;
	const char format[] = "%e";
	const double val[] = { base, 1.01 * base, 0.99 * base };
	const char
			*expected[] = { "1.000000e-07", "1.010000e-07", "9.900000e-08" };
	char *hanswer;

	// check host answers to conform with ISO
	int hostOK = 1;
	for (unsigned int i = 0; i < sizeof val / sizeof val[0]; ++i) {
		int hansLen = asprintf(&hanswer, format, val[i]);
		if (hanswer == 0 || hansLen < 0) {
			logErrorRemark(tpi, format,
					"HOST problem:  asprintf() failed: %d\n", hansLen);
		} else {
			TEST_SETPOSNEXT();
			hostOK &= compareResult(tpi, expected[i], strlen(expected[i]),
					format, hanswer, hansLen);
			free(hanswer);
		}
	}

	if (isTestingHost()) {
		// do not test host against itself
	} else if (!hostOK) {
		TEST_SETPOSNEXT();
		logErrorRemark(tpi, "%g",
				"Skipping tests for exponent with small delta because host not ok");
	} else {
		for (int i = 0; i < 10; ++i) {
			double delta = i * 0.1 * base;
			if (!TESTHOST("%g", base + delta)) {
				fprintf(stderr, "Used %.8f + %.8f = %.8f\n", base, delta, base
						+ delta);
			}
			if (!TESTHOST("%g", base - delta)) {
				fprintf(stderr, "Used %.8f - %.8f = %.8f\n", base, delta, base
						- delta);
			}
		}
	}

	/*
	 * end of exponent with small delta
	 */

	TEST("1e+06", "%g", (double) 1000000);
	TEST("1e+06", "%g", (double) 1000001);
	TEST("1.00001e+06", "%g", (double) 1000010);
	TEST("100001", "%g", (double) 100001);
	TEST("1.00001e+06", "%g", (double) 1000008);
	TEST("1e-07", "%g", (double) 0.0000001);
	TEST("1e-07", "%g", (double) 0.00000010000001); //
	TEST("1e-07", "%g", (double) 0.0000001000001);
	TEST("9.99e-08", "%g", (double) 0.0000000999);
	TEST("1.00001e-07", "%g", (double) 0.000000100001);
	TEST("0.001", "%g", (double) 0.001000000);
	TEST("0.001", "%g", (double) 0.0010000001);
	TEST("0.001", "%g", (double) 0.001000001);
	TEST("0.00100001", "%g", (double) 0.00100001);
	TEST("     1e+05", "%10.3g", (double) 100000);
	TEST("     1e+05", "%10.3g", (double) 100001);
	TEST("     1e+05", "%10.3g", (double) 100008);
	TEST("     1e-06", "%10.3g", (double) 0.000001);
	TEST("     1e+05", "%10.5g", (double) 100000);
	TEST("     1e+05", "%10.5g", (double) 100001);
	TEST("1.0001e+05", "%10.5g", (double) 100008);
	TEST("     1e-06", "%10.5g", (double) 0.000001);

	TEST("3.1415927", "%.8g", M_PI);
	TEST("3", "%.0g", M_PI);
	TEST("0", "%.1g", 0.0);
	TEST("1e-05", "%.1g", 0.00001);
	TEST("-1e-05", "%.1g", -0.00001);
	TEST("1e+04", "%.1g", 10000.0);
	TEST("-1e+04", "%.1g", -10000.0);
	/* %g, flags */
	TEST("+3.14159", "%+g", M_PI);
	TEST(" 3.14159", "% g", M_PI);
	TEST("3.", "%#.0g", M_PI);
	TEST("0000003.1", "%09.2g", M_PI);

	endTests(tpi);
}
