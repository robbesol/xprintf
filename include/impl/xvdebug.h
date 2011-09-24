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

#ifndef IMPL_XVDEBUG_H_
#define IMPL_XVDEBUG_H_

#include <stdarg.h>

/**
 * \file
 *
 * For convenience, %xvdebug.h also causes an include of xdebug.h.
 */

#include "xdebug.h"
// xdebug.h defines DEBUG_DISABLED and DEBUG_FN_QUALIFIERS

/**
 * @sa debugnl()
 * @param format vfprintf() compatible format string
 * @param ap vfprintf() compatible va_list
 * \return &lt0: error <br> >=0: number of chars written (excluding the added newline)
 */
DEBUG_FN_QUALIFIERS
int vdebugnl(const char *format, va_list ap);

/**
 * @sa debugNonl()
 * @param format vfprintf() compatible format string
 * @param ap vfprintf() compatible va_list
 * \return &lt0: error <br> >=0: number of chars written
 */
DEBUG_FN_QUALIFIERS
int vdebugNonl(const char *format, va_list ap);

#if DEBUG_DISABLED
// dummy inline bodies

DEBUG_FN_QUALIFIERS
int vdebugnl(const char *format, va_list ap) {
	(void) format;
	(void) ap;
	return 0;
}

DEBUG_FN_QUALIFIERS
int vdebugNonl(const char *format, va_list ap) {
	(void) format;
	(void) ap;
	return 0;
}

#endif

#endif /*IMPL_XVDEBUG_H_*/
