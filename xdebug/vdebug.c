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

/**
 * This file is an example implementation of the vdebug functions.
 *
 * In this implementation, the output channel is not locked before use.
 * You can use this implementation if you have a single-thread program environment
 * and if the channel's write function does its own locking.
 *
 * In a multi-thread environment, debug statements from different threads may interrupt
 * each other, causing problems on HW level or causing output to be mangled.
 * Locking will prevent this.
 */

#include "impl/xvdebug.h"
#include "xdebugchannel.h"
#include "impl/xprintf-core.h"

static inline int vdebug_start(struct xprintf_protectedChannel *xchpr,
		const char *format, va_list ap) {
	if (format == 0) {
		return 0; // OK, 0 chars written
	}

	xchpr->channel = debug_getChannel();
	if (xchpr->channel == 0) {
		// OK, no output channel to print to
		return 0; // no chars printed
	}

	/*
	 * Obtain lock for HW and/or SW.
	 * In case of failure or timeout, return negative result.
	 */
	// In this implementation: no lock used, so nothing to obtain -> success.

	return xvprintf_protected(xchpr, format, ap);
}

static inline int vdebug_end(struct xprintf_protectedChannel *xchpr,
		const char *format, ...) {
	int outCount = 0;

	/*
	 * write line terminator, if needed
	 */
	if (format != 0) {
		va_list ap;
		va_start(ap, format);
		outCount = xvprintf_protected(xchpr, format, ap);
		va_end(ap);
	}

	/*
	 * Release lock. Should not fail. (or ignore failure?)
	 */
	// In this implementation: no lock used, so nothing to release.

	return outCount;
}

int vdebugNonl(const char *format, va_list ap) {
	struct xprintf_protectedChannel xchpr;
	int outCount = vdebug_start(&xchpr, format, ap);
	outCount += vdebug_end(&xchpr, 0);
	return outCount;
}

int vdebugnl(const char *format, va_list ap) {
	struct xprintf_protectedChannel xchpr;
	int outCount = vdebug_start(&xchpr, format, ap);
	outCount += vdebug_end(&xchpr, "\r\n");
	return outCount;
}
