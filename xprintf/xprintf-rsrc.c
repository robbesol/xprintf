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

#include "impl/xprintf-rsrc.h"

static int xprintf_rsrc__writeProtected(struct xprintf_protectedChannel *xchpr,
		const char *pdata, unsigned int length);

static inline int xprintf_rsrc__checkChannel(struct xprintf_channel *ch) {
	if (ch == 0) {
		return ERRNO_INVALID_ARG;
	}
	if (ch->Write == 0) {
		return ERRNO_BAD_CHANNEL;
	}
	if (ch->Write == xprintf_rsrc__writeProtected) {
		// would cause loop
		return ERRNO_BAD_CHANNEL;
	}
	return 0;
}

int xprintf_rsrc__get_xprintf_channel(
		const struct xprintf_protectedChannel *xchpr,
		struct xprintf_channel **xchp, int(*write)(
				struct xprintf_protectedChannel *, const char *, unsigned int)) {
	if (xchpr == 0) {
		return ERRNO_NULL_ARG;
	}
	if (xchpr->writeProtected != xprintf_rsrc__writeProtected) {
		return ERRNO_BAD_CHANNEL;
	}
	// now we know that xchpr->channel is our struct xprintf_channel
	struct xprintf_channel *xch = xchpr->channel;

	int result = xprintf_rsrc__checkChannel(xch);
	if (result < 0) {
		return result;
	}

	if (write != 0 && xch->Write != write) {
		return ERRNO_BAD_CHANNEL;
	}

	*xchp = xch;
	return 0;
}

static int xprintf_rsrc__writeProtected(struct xprintf_protectedChannel *xchpr,
		const char *pdata, unsigned int length) {
	struct xprintf_channel *xch;
	int result = xprintf_rsrc__get_xprintf_channel(xchpr, &xch, 0);
	if (result < 0) {
		return result;
	}
	result = rsrc_checkCredentials(xch->compoundIOToken,
			xchpr->protectionCredentials);
	if (result < 0) {
		return result;
	}
	return xch->Write(xchpr, pdata, length);
}

// from impl/xprintfchannel.h:

int xprintf_beginProtectedIO(struct xprintf_channel *xch,
		struct xprintf_protectedChannel *xchpr) {
	if (xchpr == 0) {
		return ERRNO_NULL_ARG;
	}
	int result = xprintf_rsrc__checkChannel(xch);
	if (result < 0) {
		return result;
	}
	int credentials = rsrc_acquireToken(xch->compoundIOToken, 1);
	if (credentials < 0) {
		return credentials;
	}
	if (credentials == 0) {
		// ! should not happen !
		return ERRNO_INVALID_RESULT;
	}

	// we now own the token -- fill result
	xchpr->writeProtected = xprintf_rsrc__writeProtected;
	xchpr->protectionCredentials = credentials;
	xchpr->channel = xch;
	return 0;
}

void xprintf_endProtectedIO(struct xprintf_protectedChannel *xchpr) {
	if (xchpr == 0) {
		return;
	}
	const struct xprintf_channel *xch = xchpr->channel;
	if (xch == 0) {
		return;
	}
	rsrc_releaseToken(xch->compoundIOToken, xchpr->protectionCredentials);
}

