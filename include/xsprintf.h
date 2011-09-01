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

#ifndef XSPRINTF_H_
#define XSPRINTF_H_

#include <stddef.h>
#include <stdarg.h>

/**
 * @file
 *
 * The \c %xsnprintf() and \c %xvsnprintf() functions are equivalent to ISO C99's \c snprintf() and \c vsnprintf().
 * The functions behave as xprintf(), except that the output is written into an array (specified by argument \c str) rather than to a channel.
 *
 * If \c size is zero, nothing is written, and \c str may be a \c null pointer.
 * Otherwise, output characters beyond the \c size-1st are discarded rather than being written to the array,
 * and a \c null character is written at the end of the characters actually written into the array.
 *
 * Thus, the \c null terminated output has been completely written if and only if the returned value is nonnegative and less than \c size.
 */

/**
 * The %xsnprintf() function is equivalent to ISO C99's \c snprintf().
 *
 * @param str pointer to destination buffer
 * @param size \c sizeof \c str
 * @param format a \c printf() compatible format string
 * @param ... variable argument list for \c format
 * @return &lt;0: error code <br> ≥0: the number of characters that would have been written had \c size been sufficiently large, not counting the terminating null character
 * @sa snprintf() specification in ISO C99
 */
int
xsnprintf(char *restrict str, size_t size, const char *restrict format, ...) __attribute__ ((format (printf, 3, 4)));

/**
 * The %xvsnprintf() function is equivalent to ISO C99's \c vsnprintf().
 *
 * @param str pointer to destination buffer
 * @param size \c sizeof \c str
 * @param format a \c printf() compatible format string
 * @param ap \c va_list to the arguments for \c format
 * @return &lt;0: error code <br> ≥0: the number of characters that would have been written had \c size been sufficiently large, not counting the terminating null character
 * @sa vsnprintf() specification in ISO C99
 */
int xvsnprintf(char *restrict str, size_t size, const char *restrict format,
		va_list ap);

#endif /*XSPRINTF_H_*/
