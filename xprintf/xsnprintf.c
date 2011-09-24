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

#include "xsprintf.h"

#include "impl/xprintf-core.h"

#ifdef TESTDEBUG
#include "impl/xprintf-debug.h"
#define DEBUG2HOST	xprintf__debug2host
#else /* TESTDEBUG */
// make sure the compiler can check the calls and then optimize all debugging code away
static void DEBUG2HOST(const char * const format, ...) __attribute__ ((format (printf, 1, 2)));
static inline void DEBUG2HOST(const char * const format, ...) {
	// empty -- keep compiler happy
	(void) format;
}
#endif /* TESTDEBUG */

/**
 * Data structure for xvsnprintf__writeProtected().
 */
struct xsnprintf_channel {
	/// position for next write, initialized to start of output string
	char *nextCharPos;
	/// end character position of output string
	const char *maxNullPos;
};

/**
 * the routine to print chars to the string
 *
 * @param xchpr protected channel, xchpr->channel may be \c null
 * @param chars input string
 * @param length length of input string
 * @return length of input string
 */
static int xvsnprintf__writeProtected(struct xprintf_protectedChannel *xchpr,
		const char *chars, unsigned int length) {
	if (chars == 0) {
		DEBUG2HOST("%s(xchpr, NULL, %u) null chars arg\n", __func__, length);
		return ERRNO_NULL_ARG;
	}
	if (xchpr == 0) {
		DEBUG2HOST("%s(NULL, chars, %u) null xchpr arg", __func__, length);
		return ERRNO_NULL_ARG;
	}

	struct xsnprintf_channel *ch = xchpr->channel;

	// null channel indicates "do not write"
	if (ch == 0) {
		// return "input" length even though nothing was written
		return length;
	}

	const char *maxNullPos = ch->maxNullPos;

	// check parameters

	if (maxNullPos == 0) {
		DEBUG2HOST("%s(): NULL maxNullPos\n", __func__);
		return ERRNO_BAD_CHANNEL;
	}
	if (ch->nextCharPos == 0) {
		DEBUG2HOST("%s(): NULL nextCharPos\n", __func__);
		return ERRNO_BAD_CHANNEL;
	}
	if (ch->nextCharPos > maxNullPos) {
		DEBUG2HOST("%s(): nextCharPos > maxNullPos\n", __func__);
		return ERRNO_BAD_CHANNEL;
	}

	// all OK -- do write

	// copy chars up to, but not overwriting, maxNullPos
	for (unsigned int i = 0; i < length; ++i) {
		if (ch->nextCharPos < maxNullPos) {
			*ch->nextCharPos = chars[i];
			ch->nextCharPos += 1;
		} else {
			break;
		}
	}

	// place terminating NUL
	if (ch->nextCharPos <= maxNullPos) {
		*ch->nextCharPos = '\0';
	}

	// return input length, regardless of how much was written
	return length;
}

int xvsnprintf(char * restrict str, size_t size, const char * restrict format,
		va_list ap) {
	if (format == 0) {
		return ERRNO_NULL_ARG;
	}

	struct xprintf_protectedChannel xchpr = {
	//
			.writeProtected = xvsnprintf__writeProtected, //
			.protectionCredentials = 0, // not used
			.channel = 0, // set below
			};

	/*
	 * ISO -C says:
	 *  "If size is zero, nothing is written, and str may be a null pointer."
	 */
	if (size == 0) {
		// use null channel to indicate "do not write"
		xchpr.channel = 0;
	} else if (str == 0) {
		return ERRNO_NULL_ARG;
	} else {
		struct xsnprintf_channel data = {
		//
				.nextCharPos = str, //
				.maxNullPos = str + size - 1 //
				};
		xchpr.channel = &data;
	}

	return xvprintf_protected(&xchpr, format, ap);
}

int xsnprintf(char * restrict str, size_t size, const char * restrict format,
		...) {
	va_list ap;
	va_start(ap, format);
	int outCount = xvsnprintf(str, size, format, ap);
	va_end(ap);
	return outCount;
}

