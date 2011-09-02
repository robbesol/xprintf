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

#include "test-runner.h"
#include "test-rig.h"

void test_nodoubles(struct test_printf_info *tpi) {
	startTests(tpi, __func__);

	TEST_SAME("%.8f", M_PI);
	TEST_SAME("%.0f", M_PI);
	TEST_SAME("%1.f", M_PI);
	TEST_SAME("%3.f", M_PI);
	TEST_SAME("%0.0f", 0.32);
	TEST_SAME("%.6f", 5.55555);
	TEST_SAME("%.5f", 5.55555);
	TEST_SAME("%.4f", 5.55555);
	TEST_SAME("%.0f", 5.55555);

	// %f, negative
	TEST_SAME("%f", -M_PI);
	TEST_SAME("%.8f", -M_PI);
	TEST_SAME("%.0f", -M_PI);
	TEST_SAME("%1.f", -M_PI);
	TEST_SAME("%3.f", -M_PI);
	TEST_SAME("%0.0f", 0.32);
	TEST_SAME("%.6f", -5.55555);
	TEST_SAME("%.5f", -5.55555);
	TEST_SAME("%.4f", -5.55555);
	TEST_SAME("%.0f", -5.55555);
	/* %f, flags */
	TEST_SAME("%+f", -M_PI);
	TEST_SAME("% f", -M_PI);
	TEST_SAME("% 10f", -M_PI);
	TEST_SAME("%#.0f", -M_PI);
	TEST_SAME("%05.2f", -M_PI);
	TEST_SAME("%06.2f", -M_PI);

	// leading zero in frac
	TEST_SAME("%06.3f", 3.02134);
	TEST_SAME("%06.3f", -3.02134);
	TEST_SAME("%07.3f", -3.02134);

	TEST_SAME("vrot %07.3f blerk", -3.02134);
	TEST("vrot %07.3f blerk 11", "vrot %07.3f blerk %d", -3.02134, 11);
	TEST("10 vrot %07.3f blerk", "%d vrot %07.3f blerk", 10, -3.02134);

	TEST_SAME("vrot %07.3f blerk %f aap", -3.02134, -5.555);
	TEST("x 10   vrot %07.3f blerk %f aap 011",
			"x %-4d vrot %07.3f blerk %f aap %03d", 10, -3.02134, -5.555, 11);

	//    /* %e, basic formatting */
	//    /* for %e we can't expect to reproduce exact strings and lengths, since SUS
	//     * only guarantees that the exponent shall always contain at least two
	//     * digits. On Windows, it seems to be at least three digits long.
	//     * Therefore, we compare the test_results of parsing the expected test_result and the
	//     * actual test_result.
	//     */
	//    TEST (buf, g_snprintf (buf, 128, "%e", M_PI) >= 12 && same_value (buf, "3.141593e+00"));
	//    TEST (buf, g_snprintf (buf, 128, "%.8e", M_PI) >= 14 && same_value (buf, "3.14159265e+00"));
	//    TEST (buf, g_snprintf (buf, 128, "%.0e", M_PI) >= 5 && same_value (buf, "3e+00"));
	//    TEST (buf, g_snprintf (buf, 128, "%.1e", 0.0) >= 7 && same_value (buf, "0.0e+00"));
	//    TEST (buf, g_snprintf (buf, 128, "%.1e", 0.00001) >= 7 && same_value (buf, "1.0e-05"));
	//    TEST (buf, g_snprintf (buf, 128, "%.1e", 10000.0) >= 7 && same_value (buf, "1.0e+04"));
	//    /* %e, flags */
	//    TEST (buf, g_snprintf (buf, 128, "%+e", M_PI) >= 13 && same_value (buf, "+3.141593e+00"));
	//    TEST (buf, g_snprintf (buf, 128, "% e", M_PI) >= 13 && same_value (buf, " 3.141593e+00"));
	//    TEST (buf, g_snprintf (buf, 128, "%#.0e", M_PI) >= 6 && same_value (buf, "3.e+00"));
	//    TEST (buf, g_snprintf (buf, 128, "%09.2e", M_PI) >= 9 && same_value (buf, "03.14e+00"));

	endTests(tpi);
}

