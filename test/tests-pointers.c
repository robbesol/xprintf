/*
 * Copyright(c) 2009 Technologies88 B.V.
 * All rights reserved.
 */

#include "test-runner.h"
#include "test-rig.h"

static int za = 10;
static int *zap = &za;

void test_pointers(struct test_printf_info *tpi) {
	startTests(tpi, __func__);

	zap++; // prevent variable zap to be optimized away

	TEST("0", "%p", 0); // MacOS: "0x0" [3]
	TEST("0", "%#p", 0); // MacOS: "0x0" [3]
	TEST("00000000", "%08p", 0); // MacOS: "0x000000" [8]
	TEST("00000000", "%#08p", 0); // MacOS: "0x000000" [8]
	TEST("00000000", "%08.8p", 0); // MacOS: "0x00000000" [10]
	TEST("00000000", "%#08.8p", 0); // MacOS: "0x00000000" [10]

	TESTHOST("%p", (void*) zap);
	TESTHOST("%#p", (void*) zap);
	TESTHOST("%8p", (void*) zap);
	TESTHOST("%#8p", (void*) zap);
	TESTHOST("%08p", (void*) zap);
	TESTHOST("%#08p", (void*) zap);
	TESTHOST("%.8p", (void*) zap);
	TESTHOST("%#.8p", (void*) zap);
	TESTHOST("%0.8p", (void*) zap);
	TESTHOST("%#0.8p", (void*) zap);
	TESTHOST("%08.8p", (void*) zap);
	TESTHOST("%#08.8p", (void*) zap);

	endTests(tpi);
}
