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

#include <limits.h>
#include "xdebug.h"

/*
 * From the ISO-C document:
 *
 **********

 The strtol function converts the initial portion of the string pointed to by src to long int representation.
 First, it decomposes the input string into three parts:
 an initial, possibly empty, sequence of white-space characters (as specified by the isspace function),
 a subject sequence resembling an integer represented in some radix,
 and a final string of one or more unrecognized characters, including the terminating null character of the input string.
 Then, it attempts to convert the subject sequence to an integer, and return the result.

 The expected form of the subject sequence is that of an integer constant as described in 6.4.4.1,
 optionally preceded by a plus or minus sign, but not including an integer suffix.
 The letters from a (or A) through z (or Z) are ascribed the values 10 through 35;
 only letters and digits whose ascribed values are less than that of base are permitted.

 The subject sequence is defined as the longest initial subsequence of the input string,
 starting with the first non-white-space character, that is of the expected form.
 The subject sequence contains no characters if the input string is empty or consists entirely of white space,
 or if the first non-white-space character is other than a sign or a permissible letter or digit.

 If the subject sequence has the expected form, the sequence of characters starting with
 the first digit is interpreted as an integer constant according to the rules of 6.4.4.1.
 If the subject sequence begins with a minus sign, the value resulting from the conversion is negated (in the return type).
 A pointer to the final string is stored in the object pointed to by afterptr, provided that afterptr is not a null pointer.

 If the subject sequence is empty or does not have the expected form, no conversion is performed;
 the value of src is stored in the object pointed to by afterptr, provided that afterptr is not a null pointer.

 The strtol function returns the converted value, if any.
 If no conversion could be performed, zero is returned.
 If the correct value is outside the range of representable values, LONG_MIN or LONG_MAX is returned
 (according to sign of the value, if any), and the value of the macro ERANGE is stored in errno.

 **********
 */

static int char2value(int base, char c) {
	int val;
	if ('0' <= c && c <= '9') {
		val = c - '0';
	} else if ('a' <= c && c <= 'f') {
		val = 10 + c - 'a';
	} else if ('A' <= c && c <= 'F') {
		val = 10 + c - 'A';
	} else {
		return -1;
	}
	if (val >= base) {
		// not a valid char for this base
		return -1;
	}
	return val;
}

#define XSTRTOL_FAIL(msg)	{ /* debugnl("%s(%s) fails: %s", __func__, src, msg); */ if (afterptr != 0) *afterptr = src; return 0; }

static long int xstrtol_based(const char *src, _constchar *afterptr, int base) {
	if (src == 0) {
		debugnl("%s() null src", __func__);
		return 0;
	}

	const char *p = src;

	// skip spaces
	while (*p == ' ' || *p == '\t') {
		p++;
	}

	int negative;
	switch (*p) {
	case '\0':
		XSTRTOL_FAIL("only spaces")

	case '+':
		p += 1;
		negative = 0;
		break;

	case '-':
		p += 1;
		negative = 1;
		break;

	default:
		negative = 0;
		break;
	}

	switch (base) {
	case 8:
	case 10:
	case 16:
		break;

	case 0:
		base = 10;
		if (*p == '0') {
			// could be zero, octal or hex
			switch (p[1]) {
			case 'x':
			case 'X':
				// is hex
				p += 2;
				base = 16;
				break;

			default:
				// is octal or zero (also octal)
				base = 8;
				break;
			}
		}
		break;

	default:
		XSTRTOL_FAIL("base should be 8, 10, 16")
	}

	// convert first (leftmost) digit
	int digit = char2value(base, *p);
	if (digit < 0) {
		XSTRTOL_FAIL("no digits")
	}
	p += 1;

	long int value = digit;
	int overflow = 0;

	for (;; p++) {
		digit = char2value(base, *p);
		if (digit < 0) {
			break;
		}

		const long int oldVal = value;
		value *= base;
		value += digit;
		if (!overflow) {
			overflow = value / base != oldVal;
			if (overflow) {
				debugnl("%s() overflow: old %ld dig %c new %ld", __func__,
						oldVal, *p, value);
			}
		}
	}

	// valid conversion
	if (afterptr != 0) {
		*afterptr = p;
	}

	if (overflow) {
		if (negative) {
			return LONG_MIN;
		} else {
			return LONG_MAX;
		}
	}

	if (negative) {
		return -value;
	} else {
		return value;
	}
}

long int xstrtol(const char *src, _constchar *afterptr) {
	return xstrtol_based(src, afterptr, 0);
}

long int xstrtol10(const char *src, _constchar *afterptr) {
	return xstrtol_based(src, afterptr, 10);
}
