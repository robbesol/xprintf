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

#include "xdebug.h"
#include "impl/xvdebug.h"

int debugF3(int64_t value, int divider) {
	if (divider <= 0) {
		return debugNonl("%lld/%d", value, divider);
	}

	if (value == 0) {
		return debugNonl("0");
	}

	char *sign = "+";
	if (value < 0) {
		sign = "-";
		value = -value;
	}

	int vi = value / divider;

	uint64_t vf1000 = value % divider;
	vf1000 *= 1000;
	vf1000 /= divider;

	return debugNonl("%s%d.%03d", sign, vi, (int) vf1000);
}

int debugStringF3(const char *s, int64_t value, int divider) {
	int scount = 0;
	if (s != 0) {
		scount = debugNonl("%s", s);
	}
	return scount + debugF3(value, divider);
}

int debugF3nl(int64_t value, int divider, const char *format, ...) {
	int scount = 0;
	if (format != 0) {
		va_list ap;
		va_start(ap, format);
		scount = vdebugNonl(format, ap);
		va_end(ap);
		scount += debugNonl(": ");
	}
	scount += debugF3(value, divider);
	return scount + debugNL();
}
