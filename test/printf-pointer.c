/*
 * Copyright(c) 2009 Technologies88 B.V.
 * All rights reserved.
 */

#include <stdio.h>

#define SHOW(fmt, ...)		do {fprintf(stderr, fmt, __VA_ARGS__); fprintf(stderr, " -- %s\n",  fmt);} while (0)

static int za = 10;
static int *zap = &za;

static void doPointerOutput(void) {
	zap++; // prevent variable zap to be optimized away

	SHOW("%p", zap);
	SHOW("%#p", zap);
	SHOW("%8p", zap);
	SHOW("%#8p", zap);
	SHOW("%08p", zap);
	SHOW("%#08p", zap);
	SHOW("%.8p", zap);
	SHOW("%#.8p", zap);
	SHOW("%0.8p", zap);
	SHOW("%#0.8p", zap);
	SHOW("%08.8p", zap);
	SHOW("%#08.8p", zap);
}

int main(void) {
	doPointerOutput();
	return 0;
}
