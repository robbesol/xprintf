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

#ifndef XPRINTFDEBUG_H_
#define XPRINTFDEBUG_H_

#if defined __arm__ || defined __ARMEL__
#error "TESTDEBUG can only be used by UNIX hosted execution"
#endif

#include <stdarg.h>
#include "impl/xprintf-intl.h"

/**
 * \file
 *
 * Debugging for host-based testing of xprintf(), not for embedded use.
 *
 * \ingroup xprintf_core
 */

/**
 * A routine that prints debugging information to a test environment.
 * Must be implemented by the test rig's environment and print to a suitable stream e.g.:
 * \code
 * #include "impl/xprintf-debug.h"
 *
 * void xprintf__debug2hostV(const char *format, va_list ap) {
 *   vfprintf(stderr, format, ap));
 * }
 * \endcode
 * \param format a \c printf() type format
 * \param ap the \c va_list for the \c format
 */
void xprintf__debug2hostV(const char *format, va_list ap);

void xprintf__debug2host(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

void xprintf__debugFormatV(const char * const format,
		const struct xprintf__conversionPparams *params, char consSpec,
		const char *msgFmt, va_list ap);

#endif /* XPRINTFDEBUG_H_ */
