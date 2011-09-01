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
#include <string.h> // for memset()
/*
 * From the ISO C99 document:
 *-------------------------------------

 double frexp(double value, int *exp);
 float frexpf(float value, int *exp);
 long double frexpl(long double value, int *exp);

 The frexp functions break a floating-point number into a normalized fraction and an
 integral power of 2. They store the integer in the int object pointed to by exp.
 The frexp functions return the value x, such that x has a magnitude
 in the interval [1/2,1) or zero, and value equals x*2<sup>*exp</sup>.
 If value is zero, both parts of the result are zero.

 If value is not a floating-point number, the results are unspecified. Otherwise, the
 frexp functions return the value x, such that x has a magnitude in the interval [1/2,1) or
 zero, and value equals x*2<sup>exp</sup>. If valueis zero, both parts of the result are zero.

 *-------------------------------------

 int ilogb(double x);
 int ilogbf(float x);
 int ilogbl(long double x);

 The ilogb functions extract the exponent of x as a signed int value. If x is zero they
 compute the value FP_ILOGB0; if x is infinite they compute the valueINT_MAX; if x is
 aNaN they compute the value FP_ILOGBNAN; otherwise, they are equivalent to calling
 the corresponding logb function and casting the returned value to type int. A domain
 error or range error may occur if x is zero, infinite, or NaN. If the correct value is outside
 the range of the return type, the numeric result is unspecified.

 The ilogb functions return the exponent of x as a signed int value.

 *-------------------------------------

 double ldexp(double x, int exp);
 float ldexpf(float x, int exp);
 long double ldexpl(long double x, int exp);

 The ldexp functions multiply a floating-point number by an integral power of 2.
 A range error may occur.

 The ldexp functions return x*2<sup>exp</sup>.

 *-------------------------------------
 */

#include  "impl/xprintf-intl.h"

static void DEBUG_FORMAT(const struct xprintf__conversionPparams *params,
		const char *msgFmt, ...) __attribute__ ((format (printf, 2, 3)));

#ifdef TESTDEBUG
static void DEBUG_FORMAT(const struct xprintf__conversionPparams *params,
		const char *msgFmt, ...) {
	char specFlag = '-';
	if (params != 0) {
		switch (params->convSpec) {
			case convDoubleExponent:
			specFlag = 'e';
			break;
			case convDoubleFixed:
			specFlag = 'f';
			break;
			case convDoubleGeneral:
			specFlag = 'g';
			break;
			case convDoubleUnsupported:
			specFlag = '?';
			break;
			default:
			specFlag = '!';
			break;
		}
	}
	va_list ap;
	va_start(ap, msgFmt);
	xprintf__debugFormatV("xprintf__printDouble", params, specFlag, msgFmt, ap);
	va_end(ap);
}
#else /* TESTDEBUG */
// make sure the compiler can check the calls and then optimize all debugging code away
static inline void DEBUG_FORMAT(
		const struct xprintf__conversionPparams *params, const char *msgFmt,
		...) {
	// empty -- keep compiler happy:
	(void) params;
	(void) msgFmt;
}
#endif /* TESTDEBUG */

static inline int getExponent10(double doubleArg) {

	switch (fpclassify(doubleArg)) {
	case FP_ZERO:
		// note: log10(0) fails
		return 0;

	case FP_NAN:
	case FP_INFINITE:
		// log10(nan) and log10(inf) are likely to fail as well :-)
		return 0;
	}

	// note: log10(<0) fails
	return floor(log10(fabs(doubleArg)));
}

/*
 * ISO-C:

 * e,E,f,F,g,G:
 A double argument representing an infinity or NaN is converted in the style
 of an f or F conversion specifier.

 */
static inline int xprintf__handleSpecial(
		struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params, double doubleArg) {

	// handle special cases

	const char *specStr = 0;

	switch (fpclassify(doubleArg)) {
	case FP_NAN:
		specStr = "nan";
		if (params->flags.upper) {
			specStr = "NAN";
		}
		break;

	case FP_INFINITE:
		specStr = "inf";
		if (params->flags.upper) {
			specStr = "INF";
		}
		break;

	case FP_ZERO:
		break;
	case FP_SUBNORMAL:
		break;
	case FP_NORMAL:
		break;

	default:
		DEBUG_FORMAT(params, "unknown fpclassify(%f): %d", doubleArg,
				fpclassify(doubleArg));
		return ERRNO_INVALID_RESULT;
	}

	if (specStr == 0) {
		// not a special case
		return 0;
	}

	struct xprintf__conversionPparams stringParams = *params;
	stringParams.flags.alternative = 0;
	stringParams.precision = -1;
	return xvprintf__printString(xchpr, &stringParams, specStr);
}

static int xprintf__f(struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params, double doubleArg) {

	int isSigned = signbit(doubleArg);

	// If the precision is missing, it is taken as 6
	int floatPrecision = params->precision;
	if (floatPrecision < 0) {
		floatPrecision = 6;
	}

	double precisionFactor = 1;
	for (int i = 1; i <= floatPrecision; i++) {
		precisionFactor *= 10;
	}

	// unsign and round arg
	double roundedArg = doubleArg;
	if (isSigned) {
		roundedArg = -roundedArg;
	}
	roundedArg += 0.5 / precisionFactor;

	// split into integral and fractional part
	double intPart;
	double unsignedfrac = modf(roundedArg, &intPart);
	double fracPart;
	(void) modf(unsignedfrac * precisionFactor, &fracPart);

	if (params->convSpec == convDoubleGeneral && !params->flags.alternative) {
		while (floatPrecision > 0 && fmod(fracPart, 10) == 0) {
			floatPrecision -= 1;
			fracPart /= 10;
		}
	}

	// if the precision is explicitly zero, no decimal-point character appears.
	// Unless the alternative flag (#) is specified, which ensures there is one.
	int dotFracWidth; // width of '.' oplus fraction
	if (floatPrecision == 0 && !params->flags.alternative) {
		dotFracWidth = 0;
	} else {
		dotFracWidth = 1 + floatPrecision;
	}

	// build parameters for integer part
	struct xprintf__conversionPparams intParams = *params;
	if (isSigned) {
		intParams.sign = '-';
	}
	intParams.precision = -1; // print the integral part with no precision.
	int intMinWidth = params->minWidth - dotFracWidth;
	if (intMinWidth <= 0) {
		// If a decimal point appears, at least one digit appears before it.
		if (dotFracWidth > 0) {
			intMinWidth = 1;
		} else {
			intMinWidth = 0;
		}
	}
	intParams.minWidth = intMinWidth;

	//	unsigned long long int intArg;
	//	intArg = (unsigned long long int) intPart;
	int outCount = xprintf__printNumber(xchpr, &intParams, intPart);
	if (outCount < 0) {
		return outCount;
	}

	if (dotFracWidth > 0) {
		struct xprintf__conversionPparams fracParams = *params;
		memset(&fracParams, 0, sizeof fracParams);
		fracParams.digitBase = params->digitBase;
		fracParams.minWidth = dotFracWidth;
		fracParams.sign = '.';
		fracParams.precision = dotFracWidth - 1;

		int cnt = xprintf__printNumber(xchpr, &fracParams, fracPart);
		if (cnt < 0) {
			return cnt;
		}
		outCount += cnt;
	}

	return outCount;
}

static int xprintf__e(struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params, double doubleArg) {

	// find exponent and scale unsigned arg

	const int e10 = getExponent10(doubleArg);

	char expSign = '+';
	unsigned int e10unsigned = 0;
	if (e10 < 0) {
		e10unsigned = -e10;
		expSign = '-';
		// scale arg up
		for (unsigned int i = 0; i < e10unsigned; i++) {
			doubleArg *= 10;
		}
	} else if (e10 > 0) {
		e10unsigned = e10;
		// scale arg down
		for (unsigned int i = 0; i < e10unsigned; i++) {
			doubleArg /= 10;
		}
	}

	int expWidth = 2;
	if (e10unsigned > 0) {
		// note: log10(0) fails and log10(<0) fails
		expWidth += (int) log10((double) e10unsigned);
		if (expWidth < 2) {
			expWidth = 2;
		}
	}

	int outCount = 0;
	int cnt;

	// print the scaled arg
	struct xprintf__conversionPparams fParams = *params;
	fParams.minWidth -= 2 + expWidth; // subtract the the "e±ddd" from field width
	cnt = xprintf__f(xchpr, &fParams, doubleArg);
	if (cnt < 0) {
		return cnt;
	}
	outCount += cnt;

	// print the "e"
	char *eStr = "e";
	if (params->flags.upper) {
		eStr = "E";
	}
	cnt = xchpr->writeProtected(xchpr, eStr, 1);
	if (cnt < 0) {
		return cnt;
	}
	outCount += cnt;

	// print the exponent
	struct xprintf__conversionPparams iParams;
	memset(&iParams, 0, sizeof iParams);
	iParams.digitBase = params->digitBase;
	iParams.precision = expWidth;
	iParams.flags.signplus = 1;
	iParams.sign = expSign;
	cnt = xprintf__printNumber(xchpr, &iParams,
			(unsigned long long int) e10unsigned);
	if (cnt < 0) {
		return cnt;
	}
	outCount += cnt;

	return outCount;
}

static int xprintf__doubleSwitch(struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params, double doubleArg) {
	if (xchpr == 0) {
		return ERRNO_NULL_ARG;
	}
	if (params == 0) {
		return ERRNO_NULL_ARG;
	}
	if (xchpr->writeProtected == 0) {
		return ERRNO_BAD_CHANNEL;
	}

	// handle INF and NAN
	int outCount = xprintf__handleSpecial(xchpr, params, doubleArg);
	// outCount<0 is error, outCount>0 means output handled by xprintf__handleSpecial().
	if (outCount != 0) {
		return outCount;
	}

	switch (params->convSpec) {
	case convDoubleExponent:
		return xprintf__e(xchpr, params, doubleArg);

	case convDoubleFixed:
		return xprintf__f(xchpr, params, doubleArg);

	case convDoubleGeneral:
		// choose between %e" or "%f": find out later
		break;

	case convDoubleUnsupported:
		DEBUG_FORMAT(params, "bad convSpec %d", params->convSpec);
		return ERRNO_INVALID_RESULT;

	default:
		DEBUG_FORMAT(params, "bad convSpec %d", params->convSpec);
		return ERRNO_INVALID_RESULT;
	}

	/*
	 * ISO-C:

	 * g,G:
	 A double argument representing a floating-point number is converted in
	 style f or e (or in style F or E in the case of a G conversion specifier),
	 depending on the value converted and the precision.
	 Let P equal the precision if nonzero, 6 if the precision is omitted,
	 or 1 if the precision is zero.
	 Then, if a conversion with style E would have an exponent of X:
	 - if P>X≥-4, the conversion is with style f (or F) and precision P-(X+1).
	 - otherwise, the conversion is with style e (or E) and precision P-1.

	 Finally, unless the # flag is used, any trailing zeros are removed from the
	 fractional portion of the result and the decimal-point character is removed if
	 there is no fractional portion remaining.

	 */

	int P = params->precision;
	if (P < 0) {
		P = 6;
	} else if (P == 0) {
		P = 1;
	}

	const int X = getExponent10(doubleArg);

	//DEBUG_FORMAT(params, "IN: P=%d X=%d arg=%g", P, X, doubleArg);

	if (P > X && X >= -4) {
		// the conversion is with style f (or F) and precision P-(X+1).
		struct xprintf__conversionPparams p = *params;
		p.precision = P - (X + 1);
		//DEBUG_FORMAT(&p, "OUT: %%f");
		return xprintf__f(xchpr, &p, doubleArg);
	} else {
		// the conversion is with style e (or E) and precision P-1.
		struct xprintf__conversionPparams p = *params;
		p.precision = P - 1;
		//DEBUG_FORMAT(&p, "OUT: %%e");
		return xprintf__e(xchpr, &p, doubleArg);
	}
}

int (* const xprintf__printDouble)(struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params, double doubleArg) =
		xprintf__doubleSwitch;
