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

#ifndef XPRINTF_INTL_H_
#define XPRINTF_INTL_H_

#include "impl/xprintf-core.h"

/**
 * \file
 *
 * Declarations internal to xprintf-core.c and xprintf-double.c.
 *
 */

/*
 * Format size specifier.
 */
enum conversionSize {
	sizeDefault, // no size specified
	sizeChar, // "%hh_"
	sizeShort, // "%h_"
	sizeLong, // "%l_"
	sizeLongLong, // "%ll_"
	sizeIntmax_t, // "%j_"
	sizeSize_t, // "%z_"
	sizePtrdiff_t, // "%t_"
	sizeLongDouble, // "%L_"
	_size_count
};

/*
 * Format conversion specifier.
 * Notice that a value for \%n is missing because it is a special case: no output.
 */
enum xprintf__conversionSpec {
	convUnsupportedUnknown, // not a recognized conversion
	convSignedIntegral, // "%d", "%i"
	convUnsignedIntegral, // "%b", "%o", "%u", "%x", "%X"
	convDoubleExponent, // "%e", "%E"
	convDoubleFixed, // "%f", "%F"
	convDoubleGeneral, // "%g", "%G"
	convChar, // "%c"
	convString, // "%s"
	convPercent, // "%\%"
	convDoubleUnsupported, // (for now:) "%a", "%A"
	convPointer, // "%p"
	convStoreCount, // "%n"
	_conv_count
};

/**
 * Format specifier flags
 * \see impl/xprintf-intl.h
 */
struct xprintf__conversionFlags {
	unsigned leftjustified :1; ///< \c "%-5_"
	unsigned signplus :1; ///< \c "%+_"
	unsigned signspace :1; ///< \c "% 5_"
	unsigned alternative :1; ///< \c "%#_"
	unsigned leadingzeroes :1; ///< \c "%05_"
	unsigned upper :1; ///< \c "%X", \c "%A", \c "%E, $F", \c "%G"
};

/**
 * Format specifier collected parameters.
 * \see impl/xprintf-intl.h
 */
struct xprintf__conversionPparams {
	struct xprintf__conversionFlags flags; ///< format specifier flags
	int minWidth; ///< \c "%5_"
	int precision; ///< \c "%.3_" <br> -1: not specified
	unsigned int digitBase; ///< 0: not a number conversion <br> 8, 10, 16: octal, decimal, hex
	char sign; ///< \c '\\0': no sign <br> \c '-' or \c '+': sign to display
	enum xprintf__conversionSpec convSpec;
	enum conversionSize argLength;
};

/**
 * Utility for \c xvprintf_protected() and xprintf__printDouble(): prints a string to the channel.
 *
 * \param xchpr protected channel to print to
 * \param params parameters used for printing
 * \param stringArg string to print (\c NUL terminaled)
 * \return&lt;0: error code <br> ≥0: number of characters printed
 */
int
		xvprintf__printString(struct xprintf_protectedChannel *xchpr,
				const struct xprintf__conversionPparams *params,
				const char stringArg[]);

/**
 * Utility for \c xvprintf_protected() and xprintf__printDouble(): prints an integral number to the channel.
 *
 * \param xchpr protected channel to print to
 * \param params parameters used for printing
 * \param intArg number to print
 * \return&lt;0: error code <br> ≥0: number of characters printed
 */
int xprintf__printNumber(struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params,
		unsigned long long int intArg);

/**
 * Utility for \c xvprintf_protected(): prints a floating point number to the channel.
 * This pointer can be \c null to indicate that no floating point is used,
 * which saves binary code linked in for floating point support.
 *
 * \param xchpr protected channel to print to
 * \param params parameters used for printing
 * \param doubleArg number to print
 * \return &lt;0: error code <br> ≥0: number of characters printed
 */
int (* const xprintf__printDouble)(struct xprintf_protectedChannel *xchpr,
		const struct xprintf__conversionPparams *params, double doubleArg);

#ifdef TESTDEBUG
#include "impl/xprintf-debug.h"
#endif /* TESTDEBUG */

#endif /* XPRINTF_INTL_H_ */
