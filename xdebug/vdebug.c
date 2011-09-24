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
#include "xdebugchannel.h"
#include "impl/xvdebug.h"
#include "impl/xprintf-rsrc.h"

int vdebugNonl(const char *format, va_list ap) {
	if (format == 0) {
		return 0; // OK, 0 chars written
	}
	struct xprintf_channel *ch = debug_getChannel();
	if (ch == 0) {
		return 0; // OK, 0 chars written
	}
	struct xprintf_protectedChannel xchpr;
	int status = xprintf_beginProtectedIO(ch, &xchpr);
	if (status < 0) {
		return status;
	}
	status = xvprintf_protected(&xchpr, format, ap);
	xprintf_endProtectedIO(&xchpr);
	return status;
}

static inline int debug_printf_protected(
		struct xprintf_protectedChannel *xchpr, const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int outCount = xvprintf_protected(xchpr, format, ap);
	va_end(ap);
	return outCount;
}

int vdebugnl(const char *format, va_list ap) {
	if (format == 0) {
		return 0; // OK, 0 chars written
	}
	struct xprintf_channel *ch = debug_getChannel();
	if (ch == 0) {
		return 0; // OK, 0 chars written
	}
	struct xprintf_protectedChannel xchpr;
	int status = xprintf_beginProtectedIO(ch, &xchpr);
	if (status < 0) {
		return status;
	}
	status = xvprintf_protected(&xchpr, format, ap);
	debug_printf_protected(&xchpr, "%s", "\r\n");
	xprintf_endProtectedIO(&xchpr);
	return status;
}
