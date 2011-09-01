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

#ifndef IMPL_XPRINTF_CORE_H_
#define IMPL_XPRINTF_CORE_H_

#define ERRNO_NULL_ARG			(-1)
#define ERRNO_INVALID_ARG		(-1)
#define ERRNO_INVALID_RESULT	(-1)
#define ERRNO_BAD_CHANNEL		(-1)

#include <stdarg.h>

/**
 * \file
 * xvprintf_protected() formats according to ISO-C \c printf() specs and prints the result to an abstract channel.
 * The abstract channel approach allows printing to any output medium.
 * The caller must ensure that the channel is protected so that concurrent invocations of xvprintf_protected() will not disturb each other.
 *
 * \ingroup xprintf_core
 */

/**
 * Information for the routine that writes the output of xprintf().
 */
struct xprintf_protectedChannel {
	/**
	 * May used by .writeProtected() to check the protection. Not used by xvprintf_protected().
	 */
	int protectionCredentials;

	/**
	 * Pointer to the routine that does the actual writing to the channel.
	 * Invoked by xvprintf_protected() as often as needed.
	 *
	 * @param xprch destination for writing
	 * @param pData start of the string to be written
	 * @param length length of the string to be written
	 * @return &lt;0: error code, caller should stop processing and return error code <br> ≥0: the count of characters written (should equal \c length)
	 */
	int (*writeProtected)(struct xprintf_protectedChannel *xprch,
			const char *pData, unsigned int length);
	/**
	 * The abstract channel that .writeProtected() works on. Not used by xvprintf_protected().
	 */
	void *channel;
};

/**
 * Prints to a protected channel, analogous to vfprintf().
 * %xvprintf_protected() will call \c xprch's xprintf_protectedChannel.writeProtected() as often as necessary.
 *
 * @sa vfprintf() and fprintf() specifications in ISO C99
 *
 * @param xprch the protected channel
 * @param format the vfprintf() compatible format argument
 * @param ap the vfprintf() compatible va_list argument
 * @return &lt;0: error code <br> ≥0: count of chars printed (sum of values returned by calls to xprintf_protectedChannel.writeProtected())
 */
int xvprintf_protected(struct xprintf_protectedChannel *xprch,
		const char * format, va_list ap);

#endif /* IMPL_XPRINTF_CORE_H_ */
