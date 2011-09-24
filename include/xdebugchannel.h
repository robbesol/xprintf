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

#ifndef XDEBUGCHANNEL_H_
#define XDEBUGCHANNEL_H_

/**
 * \file
 * Debug routines.
 *
 * For convenience, %debugchannel.h also causes an include of debug.h, xstrerror.h and errnocode.h.
 *
 * @section debugchannel_disabled Debugging disabled
 *
 * If flagged to disable debug calls, this module provides
 * dummy @c static @c inline bodies for the debug functions
 * so that compile-time checks are still done but no code is generated.
 *
 * @sa @ref debug_disabled
 */

#include "xdebug.h"
// xdebug.h defines DEBUG_DISABLED and DEBUG_FN_QUALIFIERS

struct xprintf_channel;

/**
 * Sets (changes) the channel for debugging.
 * \param channel pointer to new debug channel, \c null will skip debug output
 */
DEBUG_FN_QUALIFIERS
void debug_setChannel(struct xprintf_channel *channel);

/**
 * Returns the current debugging channel.
 * \return pointer to debug channel, \c null will skip debug output
 */
DEBUG_FN_QUALIFIERS
struct xprintf_channel *debug_getChannel(void);

#if DEBUG_DISABLED
// dummy inline bodies

DEBUG_FN_QUALIFIERS
void debug_setChannel(struct xprintf_channel *channel) {
	(void) channel;
}

DEBUG_FN_QUALIFIERS
struct xprintf_channel *debug_getChannel(void) {
	return 0;
}

#endif /* DEBUG_DISABLED */

#endif /* XDEBUGCHANNEL_H_ */
