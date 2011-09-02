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

#include <ctype.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#include  "impl/xprintf-intl.h"

static inline void DEBUG_FORMAT(const char * const nptr,
		const struct xprintf__conversionPparams *params, const char *msgFmt,
		...) __attribute__ ((format (printf, 3, 4)));

#ifdef TESTDEBUG
static void DEBUG_FORMAT(const char * const format,
		const struct xprintf__conversionPparams *params, const char *msgFmt, ...) {
	// convert enum xprintf__conversionSpec to a char
	static const char specChars[] = "?dufcs%gp";
	char consSpecChar;
	if (params->convSpec < (sizeof specChars) - 1) {
		consSpecChar = specChars[params->convSpec];
	} else {
		consSpecChar = '!';
	}
	va_list ap;
	va_start(ap, msgFmt);
	xprintf__debugFormatV(format, params, consSpecChar, msgFmt, ap);
	va_end(ap);
}
#else /* TESTDEBUG */
// make sure the compiler can check the calls and then optimize all debugging code away
static inline void DEBUG_FORMAT(const char * const nptr,
		const struct xprintf__conversionPparams *params, const char *msgFmt,
		...) {
	// empty -- keep compiler happy:
	(void) nptr;
	(void) params;
	(void) msgFmt;
}
#endif /* TESTDEBUG */

// static, but constant data:
static const char *hexDigitsLower = "0123456789abcdef";
static const char *hexDigitsUpper = "0123456789ABCDEF";

// use this shortcut with caution -- warning: side effects
#define WRITE_STRING_OR_EXIT(str, strLen) {int _cnt = xchpr->writeProtected(xchpr, str, strLen); if (_cnt < 0) return _cnt; outCount += _cnt;}
#define WRITE_SPACES_OR_EXIT(rpt) {int _cnt = xprintf__writeSpaces(xchpr, rpt); if (_cnt < 0) return _cnt; outCount += _cnt;}

static inline int xprintf__writeSpaces(struct xprintf_protectedChannel *xchpr,
		int repeat) {
	static const char spaces[] = "          ";
	static const int spacesCount = sizeof spaces - 1; // the string has a terminating '\0'

	int todo = repeat;
	while (todo > 0) {
		int cnt;
		if (todo >= spacesCount) {
			cnt = xchpr->writeProtected(xchpr, spaces, spacesCount);
			todo -= spacesCount;
		} else {
			cnt = xchpr->writeProtected(xchpr, spaces, todo);
			todo = 0;
		}
		if (cnt < 0) {
			return cnt;
		}
	}
	return repeat;
}

static inline int xprintf__writeZeroes(struct xprintf_protectedChannel *xchpr,
		int repeat) {
	static const char zeroes[] = "0000000000";
	static const int zeroesCount = (sizeof zeroes) - 1; // the string has a terminating '\0'

	int todo = repeat;
	while (todo > 0) {
		int cnt;
		if (todo >= zeroesCount) {
			cnt = xchpr->writeProtected(xchpr, zeroes, zeroesCount);
			todo -= zeroesCount;
		} else {
			cnt = xchpr->writeProtected(xchpr, zeroes, todo);
			todo = 0;
		}
		if (cnt < 0) {
			return cnt;
		}
	}
	return repeat;
}

static inline int doprintRepeatedChar(struct xprintf_protectedChannel *xchpr,
		char c, int repeat) {
	for (int i = 0; i < repeat; i++) {
		int cnt = xchpr->writeProtected(xchpr, &c, 1);
		if (cnt < 0) {
			return cnt;
		}
	}
	return repeat;
}

int xprintf__printNumber(struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params,
		unsigned long long int intArg) {
	if (xchpr == 0 || xchpr->writeProtected == 0) {
		return ERRNO_NULL_ARG;
	}
	if (params == 0) {
		return ERRNO_NULL_ARG;
	}

	unsigned int dgtBase = params->digitBase;
	if (dgtBase > 16) {
		dgtBase = 16; // >16 not supported and will overflow hexDigits[]
	} else if (dgtBase < 2) {
		return ERRNO_INVALID_ARG;
	}

	int precision = params->precision;

	int digitCount;
	unsigned long long int leftmostDigitPower;
	if (intArg == 0 && precision == 0) {
		//  "The result of converting a zero value with a precision of zero is no characters."
		// (but see below!)
		digitCount = 0;
		leftmostDigitPower = 0;
	} else {
		// determine digit count and pwr and dgt for the leftmost digit
		// start by assuming one digit only
		digitCount = 1;
		leftmostDigitPower = 1;
		// see if we are right
		for (unsigned long long int v = intArg; v >= dgtBase; v /= dgtBase) {
			// needs more digits to cover this value
			digitCount += 1;
			leftmostDigitPower *= dgtBase;
		}
	}

	int outLen = digitCount;

	int altLen = 0;
	if (params->flags.alternative) {
		if (dgtBase == 16) {
			// "a nonzero result has 0x (or 0X) prefixed to it"
			if (intArg != 0) {
				altLen = 2;
				outLen += altLen;
			}
		} else if (dgtBase == 8) {
			// "it increases the precision, if and only if necessary,
			// to force the first digit of the result to be a zero
			// (if the value and precision are both 0, a single 0 is printed)."
			if (digitCount > 0 && intArg == 0) {
				// we have a 0
			} else if (precision <= digitCount) {
				precision = digitCount + 1;
			}
		}
	}

	int zeroPadLen = 0; // '0' and precision padding
	if (digitCount < precision) {
		zeroPadLen = precision - digitCount;
		outLen += zeroPadLen;
	}

	if (params->sign != '\0') {
		outLen++;
	}

	int leftSpacePadLen = 0;
	int rightSpacePadLen = 0;
	if (outLen < params->minWidth) {
		int padLen = params->minWidth - outLen;
		// "If the 0 and - flags both appear, the 0 flag is ignored."
		if (params->flags.leftjustified) {
			rightSpacePadLen = padLen;
		} else if (params->flags.leadingzeroes) {
			zeroPadLen += padLen; // add to existing zero padding
		} else {
			leftSpacePadLen = padLen;
		}
		outLen += padLen;
	}

	int outCount = 0;

	// left pad spaces
	if (leftSpacePadLen > 0) {
		WRITE_SPACES_OR_EXIT(leftSpacePadLen)
	}

	//sign
	if (params->sign != '\0') {
		WRITE_STRING_OR_EXIT(&params->sign, 1)
	}

	// alternative
	if (altLen > 0) {
		if (params->flags.upper) {
			WRITE_STRING_OR_EXIT("0X", 2)
		} else {
			WRITE_STRING_OR_EXIT("0x", 2)
		}
	}

	// '0' pad and precision zeros
	if (zeroPadLen > 0) {
		int cnt = xprintf__writeZeroes(xchpr, zeroPadLen);
		if (cnt < 0) {
			return cnt;
		}
		outCount += cnt;
	}

	// digits
	const char *hexDigits = hexDigitsLower;
	if (params->flags.upper) {
		hexDigits = hexDigitsUpper;
	}
	unsigned long long number = intArg;
	for (unsigned long long dp = leftmostDigitPower; dp > 0; dp /= dgtBase) {
		unsigned int digit = number / dp;
		number -= digit * dp;
		WRITE_STRING_OR_EXIT(hexDigits+digit, 1)
	}

	// right pad spaces
	if (rightSpacePadLen > 0) {
		WRITE_SPACES_OR_EXIT(rightSpacePadLen)
	}

	if (outLen != outCount) {
		DEBUG_FORMAT("", params, "%s(): outLen=%d, outCount=%d", __func__,
				outLen, outCount);
	}

	return outCount;
}

static int countCharVisual(const char c) {
	switch (c) {
	case '\0':
	case '\\':
	case '\n':
	case '\r':
	case '\t':
		return 2;

	default:
		if (isprint(c)) {
			return 1;
		} else {
			return 4; // \ooo
		}
	}
}

static int doprintVisual(struct xprintf_protectedChannel *xchpr,
		const char * restrict string, int len) {
	if (string == 0) {
		return ERRNO_NULL_ARG;
	}

	int outCount = 0;
	for (int i = 0; i < len; ++i) {
		char c = string[i];
		switch (c) {
		case '\0':
			WRITE_STRING_OR_EXIT("\\0", 2)
			break;
		case '\\':
			WRITE_STRING_OR_EXIT("\\\\", 2)
			break;
		case '\n':
			WRITE_STRING_OR_EXIT("\\n", 2)
			break;
		case '\r':
			WRITE_STRING_OR_EXIT("\\r", 2)
			break;
		case '\t':
			WRITE_STRING_OR_EXIT("\\t", 2)
			break;

		default:
			if (isprint(c)) {
				WRITE_STRING_OR_EXIT(&c, 1)
				break;
			} else { // \ooo
				WRITE_STRING_OR_EXIT("\\", 1)
				WRITE_STRING_OR_EXIT(hexDigitsLower+((c >> 6) & 07), 1)
				WRITE_STRING_OR_EXIT(hexDigitsLower+((c >> 3) & 07), 1)
				WRITE_STRING_OR_EXIT(hexDigitsLower+(c & 07), 1)
				break;
			}
		}
	}
	return outCount;
}

int xvprintf__printString(struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params, const char stringArg[]) {
	if (xchpr == 0 || xchpr->writeProtected == 0) {
		return ERRNO_NULL_ARG;
	}
	if (params == 0) {
		return ERRNO_NULL_ARG;
	}

	int outCount = 0;

	if (stringArg == 0) {
		// [ISO] behavior undefined
		WRITE_STRING_OR_EXIT("(null)", 6)
		return outCount;
	}

	int outLen = 0;
	int inLen = 0;
	while (stringArg[inLen]) {
		if (params->precision >= 0 && outLen >= params->precision) {
			break;
		}
		if (params->flags.alternative) {
			outLen += countCharVisual(stringArg[inLen]);
		} else {
			outLen += 1;
		}
		inLen += 1;
	}

	int spacePadLen = params->minWidth - outLen;

	if (spacePadLen > 0 && !params->flags.leftjustified) {
		WRITE_SPACES_OR_EXIT(spacePadLen)
	}

	if (outLen > 0) {
		int cnt;
		if (params->flags.alternative) {
			cnt = doprintVisual(xchpr, stringArg, inLen);
		} else {
			cnt = xchpr->writeProtected(xchpr, stringArg, inLen);
		}
		if (cnt < 0) {
			return cnt;
		}
		outCount += cnt;
	}

	if (spacePadLen > 0 && params->flags.leftjustified) {
		WRITE_SPACES_OR_EXIT(spacePadLen)
	}

	return outCount;
}

static inline unsigned int xvprintf__eatFlags(const char * restrict nptr,
		struct xprintf__conversionPparams *params) {
	if (nptr == 0 || params == 0) {
		return 0;
	}

	for (unsigned int eatCount = 0;; eatCount++) {
		switch (nptr[eatCount]) {
		case '#':
			params->flags.alternative = 1;
			break;
		case '0':
			params->flags.leadingzeroes = 1;
			break;
		case '-':
			params->flags.leftjustified = 1;
			break;
		case ' ':
			params->flags.signspace = 1;
			break;
		case '+':
			params->flags.signplus = 1;
			break;
		default:
			return eatCount;
		}
	}
}

// read a non-negative decimal integer -- no need for strtol()
static inline unsigned int xvprintf__eatNumber(const char * restrict nptr,
		int * restrict resultp) {
	if (nptr == 0 || resultp == 0) {
		return 0;
	}

	*resultp = 0;
	for (unsigned int eatCount = 0;; eatCount++) {
		switch (nptr[eatCount]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			*resultp *= 10;
			*resultp += nptr[eatCount] - '0';
			break;

		default:
			return eatCount;
		}
	}
}

int xvprintf_protected(struct xprintf_protectedChannel *xchpr,
		const char * format, va_list ap) {
	if (xchpr == 0 || xchpr->writeProtected == 0) {
		return ERRNO_NULL_ARG;
	}
	if (format == 0) {
		return ERRNO_NULL_ARG;
	}

	int outCount = 0;
	const char *litStart = format;

	for (;;) {
		// find first '%' at or after litStart
		for (int litCount = 0;; litCount++) {
			switch (litStart[litCount]) {
			case '\0':
			case '%':
				// found end of literal
				break;
			default:
				// found more of literal
				continue;
			}

			// first print the literal text before the '\0' or '%'
			if (litCount > 0) {
				WRITE_STRING_OR_EXIT(litStart, litCount)
			}
			litStart += litCount;
			break;
		}
		// litStart is at '\0' or '%'
		if (litStart[0] != '%') {
			break;
		}
		// litStart is at '%'

		//		int eatCount = xvprintf__eatConversion(litStart, &params, &ap);
		/*================*/

		struct xprintf__conversionPparams params;
		bzero(&params, sizeof params);
		params.precision = -1;
		params.convSpec = convUnsupportedUnknown;

		const char *cursor = litStart + 1; // right after the '%'

		cursor += xvprintf__eatFlags(cursor, &params);
		// cursor is at fst unparsed char after flags

		// digits for min field width
		if (*cursor == '*') {
			params.minWidth = va_arg(ap, int);
			// "A negative field width is treated as a left adjustment flag followed by a positive field width"
			if (params.minWidth < 0) {
				params.flags.leftjustified = 1;
				params.minWidth = -params.minWidth;
			}
			cursor += 1;
		} else {
			cursor += xvprintf__eatNumber(cursor, &params.minWidth);
		}

		// cursor is at fst unparsed char after min field width

		// dot+digits for precision
		if (*cursor == '.') {
			// "If the digit string is omitted, the precision is taken as zero."
			params.precision = 0;
			cursor += 1;
			if (*cursor == '*') {
				params.precision = va_arg(ap, int);
				// "a negative precision is treated as though it were missing."
				if (params.precision < 0) {
					params.precision = -1;
				}
				cursor += 1;
			} else {
				cursor += xvprintf__eatNumber(cursor, &params.precision);
			}
		}

		// cursor is at fst unparsed char after precision

		// length modifier: l ll
		params.argLength = sizeDefault;
		switch (*cursor) {
		case 'l':
			params.argLength = sizeLong;
			cursor += 1;
			if (*cursor == 'l') {
				params.argLength = sizeLongLong;
				cursor += 1;
			}
			break;
		case 'h':
			params.argLength = sizeShort;
			cursor += 1;
			if (*cursor == 'h') {
				params.argLength = sizeChar;
				cursor += 1;
			}
			break;
		case 'j':
			params.argLength = sizeIntmax_t;
			cursor += 1;
			break;
		case 'z':
			params.argLength = sizeSize_t;
			cursor += 1;
			break;
		case 't':
			params.argLength = sizePtrdiff_t;
			cursor += 1;
			break;
		case 'L':
			params.argLength = sizeLongDouble;
			cursor += 1;
			break;
		case 'q': // allow as synonym for "ll"
			params.argLength = sizeLongLong;
			cursor += 1;
			break;
		}

		// cursor is at fst unparsed char after length modifier

		// type of conversion
		params.convSpec = convUnsupportedUnknown;

		switch (*cursor) {
		case '\0':
			// end of format string
			DEBUG_FORMAT(litStart, &params,
					"EOS, no conversion specifier after \"%s\"", litStart);
			break;
		case 'd':
		case 'i':
			params.convSpec = convSignedIntegral;
			params.digitBase = 10;
			break;

		case 'b':
			params.convSpec = convUnsignedIntegral;
			params.digitBase = 2;
			break;
		case 'o':
			params.convSpec = convUnsignedIntegral;
			params.digitBase = 8;
			break;
		case 'u':
			params.convSpec = convUnsignedIntegral;
			params.digitBase = 10;
			break;
		case 'X':
			params.flags.upper = 1;
			// fall-through to 'x'
		case 'x':
			params.convSpec = convUnsignedIntegral;
			params.digitBase = 16;
			break;

		case 'E':
			params.flags.upper = 1;
			// fall-through to 'f'
		case 'e':
			params.convSpec = convDoubleExponent;
			params.digitBase = 10;
			break;

		case 'F':
			params.flags.upper = 1;
			// fall-through to 'f'
		case 'f':
			params.convSpec = convDoubleFixed;
			params.digitBase = 10;
			break;

		case 'G':
			params.flags.upper = 1;
			// fall-through to 'g' and 'a'
		case 'g':
			params.digitBase = 10;
			params.convSpec = convDoubleGeneral;
			break;

		case 'A':
			params.flags.upper = 1;
			// fall-through to 'g' and 'a'
		case 'a':
			params.convSpec = convDoubleUnsupported;
			break;

		case 'c':
			params.convSpec = convChar;
			break;

		case 's':
			params.convSpec = convString;
			break;

		case 'p':
			params.convSpec = convPointer;
			params.digitBase = 16;
			params.flags.alternative = 1;
			break;

		case 'n':
			params.convSpec = convStoreCount;
			break;

		case '%':
			params.convSpec = convPercent;
			break;

		default:
			DEBUG_FORMAT(litStart, &params,
					"unknown conversion specifier '%c'", *cursor);
			break;
		}

		if (params.convSpec == convUnsupportedUnknown) {
			// no valid conversion specifier
			break; // out of outer loop
		}

		// move past conversion specifier
		cursor += 1;
		int eatCount = cursor - litStart;

		// "A - overrides a 0 if both are given."
		if (params.flags.leftjustified /* && params.flags.leadingzeroes */) {
			params.flags.leadingzeroes = 0;
		}

		// "A + overrides a space if both are used."
		if (params.flags.signplus) {
			params.sign = '+';
		} else if (params.flags.signspace) {
			params.sign = ' ';
		} else {
			params.sign = '\0';
		}

		/*================*/

		if (params.convSpec == convStoreCount) {
			/*
			 * special case:
			 * store the number of characters written to the output stream so far by this call
			 */

#define PUTNINTYPE(type)    do { type *_p = va_arg(ap, type *); if (_p != 0) *_p = (type) outCount; } while (0)

			int status = 0; // OK so far
			switch (params.argLength) {
			case sizeLongLong:
				PUTNINTYPE(long long int);
				break;
			case sizeLong:
				PUTNINTYPE(long int);
				break;
			case sizeDefault:
				PUTNINTYPE(int);
				break;
			case sizeShort:
				PUTNINTYPE(short int);
				break;
			case sizeChar:
				PUTNINTYPE(signed char);
				break;
			case sizeIntmax_t:
				PUTNINTYPE(intmax_t);
				break;
			case sizeSize_t:
				PUTNINTYPE(size_t);
				break;
			case sizePtrdiff_t:
				PUTNINTYPE(ptrdiff_t);
				break;
			default:
				// error, failed to consume argument
				DEBUG_FORMAT(format, &params,
						"unanticipated size for '%%n': %d", params.argLength);
				// cannot break from a loop inside a switch...
				status = ERRNO_INVALID_RESULT;
				break;
			}

			if (status < 0) {
				break; // out of outer loop
			}

			// success, consumed argument
			// with convStoreCount there is no output, so continue directly with rest of format
			litStart += eatCount;
			continue;
		}

		// fetch arg
#define GETSIGNEDTYPE(type)    {type i = va_arg(ap, type); \
                                if (i < 0) {i = -i; params.sign = '-'; } intArg = i; }
#define GETSIGNEDINT(type)    {type i = va_arg(ap, int); \
                                if (i < 0) {i = -i; params.sign = '-'; } intArg = i; }

		char charString[2] = { '\0', '\0' };
		char * string = 0;
		unsigned long long int intArg = 0;
		double doubleArg = 0;

		switch (params.convSpec) {

		case convSignedIntegral:
			switch (params.argLength) {
			case sizeLongLong:
				GETSIGNEDTYPE(long long int)
				break;
			case sizeLong:
				GETSIGNEDTYPE(long int)
				break;
			case sizeDefault:
				GETSIGNEDTYPE(int)
				break;
			case sizeShort:
				GETSIGNEDINT(short int)
				break;
			case sizeChar:
				GETSIGNEDINT(signed char)
				break;
			case sizeIntmax_t:
				GETSIGNEDTYPE(intmax_t)
				break;
			case sizeSize_t:
				intArg = va_arg(ap, size_t);
				break;
			case sizePtrdiff_t:
				GETSIGNEDTYPE(ptrdiff_t)
				break;
			default:
				DEBUG_FORMAT(format, &params,
						"unanticipated size for signed int: %d",
						params.argLength);
				params.convSpec = convUnsupportedUnknown;
				break;
			}
			break;

		case convUnsignedIntegral: {
			switch (params.argLength) {
			case sizeLongLong:
				intArg = va_arg(ap, unsigned long long int);
				break;
			case sizeLong:
				intArg = va_arg(ap, unsigned long int);
				break;
			case sizeDefault:
				intArg = va_arg(ap, unsigned int);
				break;
			case sizeShort:
				intArg = (unsigned short int) va_arg(ap, unsigned int);
				break;
			case sizeChar:
				intArg = (unsigned char) va_arg(ap, unsigned int);
				break;
			case sizeIntmax_t:
				intArg = va_arg(ap, uintmax_t);
				break;
			case sizeSize_t:
				intArg = va_arg(ap, size_t);
				break;
			case sizePtrdiff_t:
				intArg = va_arg(ap, ptrdiff_t);
				break;
			default:
				DEBUG_FORMAT(format, &params,
						"unanticipated size for unsigned int: %d",
						params.argLength);
				params.convSpec = convUnsupportedUnknown;
				break;
			}
			break;
		}

		case convPointer:
			switch (params.argLength) {
			case sizeDefault:
				intArg = (uintptr_t) va_arg(ap, void *);
				break;

			default:
				DEBUG_FORMAT(format, &params,
						"unanticipated size for pointer: %d", params.argLength);
				params.convSpec = convUnsupportedUnknown;
				break;
			}
			break;

		case convDoubleUnsupported:
		case convDoubleExponent:
		case convDoubleFixed:
		case convDoubleGeneral:
			switch (params.argLength) {
			case sizeDefault:
				doubleArg = va_arg(ap, double);
				break;
			case sizeLongDouble:
				doubleArg = (double) va_arg(ap, long double);
				break;
			default:
				DEBUG_FORMAT(format, &params,
						"unanticipated size for double: %d", params.argLength);
				params.convSpec = convUnsupportedUnknown;
				break;
			}
			break;

		case convChar:
			charString[0] = va_arg(ap, int);
			string = charString;
			params.convSpec = convString;
			break;

		case convString:
			string = va_arg(ap, char *);
			break;

		case convPercent:
			string = "%";
			params.convSpec = convString;
			break;

		default:
			DEBUG_FORMAT(format, &params, "unanticipated conversion");
			params.convSpec = convUnsupportedUnknown;
			break;
		}

		if (params.convSpec == convUnsupportedUnknown) {
			break; // out of the loop
		}

		// print this conversion
		int cnt = 0;

		switch (params.convSpec) {
		case convSignedIntegral:
		case convUnsignedIntegral:
			/*
			 * ISO: "For d, i, o, u, x, and X conversions,
			 * if a precision is specified, the 0 flag is ignored"
			 */
			if (params.precision >= 0) {
				params.flags.leadingzeroes = 0;
			}
			cnt = xprintf__printNumber(xchpr, &params, intArg);
			break;

		case convPointer:
			cnt = xprintf__printNumber(xchpr, &params, intArg);
			break;

		case convDoubleUnsupported:
		case convDoubleExponent:
		case convDoubleFixed:
		case convDoubleGeneral:
			if (xprintf__printDouble == 0) {
				WRITE_STRING_OR_EXIT(litStart, eatCount)
			} else {
				cnt = xprintf__printDouble(xchpr, &params, doubleArg);
			}
			break;

		case convString:
			cnt = xvprintf__printString(xchpr, &params, string);
			break;

		default:
			// undefined / implementation defined behaviour
			DEBUG_FORMAT(format, &params, "cannot print");
			cnt = doprintVisual(xchpr, litStart, eatCount);
			break;
		}

		if (cnt < 0) {
			return cnt;
		}
		outCount += cnt;

		// continue with rest of format
		litStart += eatCount;
	}

	int litCount = 0;
	while (litStart[litCount] != '\0') {
		litCount++;
	}
	if (litCount > 0) {
		WRITE_STRING_OR_EXIT(litStart, litCount)
	}

	return outCount;
}
