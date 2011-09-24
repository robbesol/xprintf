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

#ifndef XDEBUG_H_
#define XDEBUG_H_

/**
 * \file
 * Debug routines.
 *
 * @section debug_disabled Debugging enable/disable
 *
 * The include file xdebug-masterswitch.h sets two preprocessor defines:
 * @c DEBUG_ENABLED and @c DEBUG_DISABLED to 0 and 1 or to 1 and 0, respectively.
 *
 * If flagged to disable debug calls (@c DEBUG_DISABLED==1, @c DEBUG_ENABLED==0),
 * this module provides dummy @c static @c inline bodies for the debug functions
 * so that compile-time checks are still done but no code is generated:
 * @code
 * static inline int debugnl(const char *format, ...) {
 *     (void) format; // prevent compiler error "unused parameter 'format'"
 *     return 0;
 * }
 * @endcode
 *
 * Compare the dummy @c static @c inline body to a @c \#define that skips the compile-time checks:
 * @code
 * #define debugnl(...) (0) // no code, but also no compile-time checks
 * @endcode
 *
 * Try it out and compare the code size: the compiler is clever enough to
 * generate no code for the dummy @c static @c inline body.
 */

#include "xdebug-masterswitch.h"

// check DEBUG_ENABLED and DEBUG_DISABLED set by xdebug-masterswitch.h and not he same
#if !defined DEBUG_ENABLED || !defined DEBUG_DISABLED
#error xdebug-masterswitch.h did not define either DEBUG_ENABLED or DEBUG_DISABLED
#elif DEBUG_ENABLED == DEBUG_DISABLED
#error xdebug-masterswitch.h defines DEBUG_ENABLED conflicting with DEBUG_DISABLED
#endif

#if DEBUG_DISABLED
#define DEBUG_FN_QUALIFIERS		static inline
#else
#define DEBUG_FN_QUALIFIERS
#endif

#include <stdint.h>

/**
 * Prints a message to a debug channel, like \c fprintf() would
 *
 * \param format \c printf()-style format
 * \param ... \c printf()-style arguments to match the format
 * \return &lt0: error <br> >=0: number of chars written
 */
DEBUG_FN_QUALIFIERS
int debugNonl(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

/**
 * Prints a message to a debug channel, followed by a newline (CR,NL to please MS users).
 *
 * \param format \c printf()-style format
 * \return &lt0: error <br> >=0: number of chars written (excluding the added newline)
 *
 */
DEBUG_FN_QUALIFIERS
int debugnl(const char *format, ...) __attribute__ ((format (printf, 1, 2)));

/**
 * Prints a newline to a debug channel (CR,NL to please MS users).
 *
 * \return &lt0: error <br> 0: success
 */
DEBUG_FN_QUALIFIERS
int debugNL(void);

/**
 * Prints \c value/divider with sign in 3 decimals, as if with a "%+.3f" format.
 * @param value any
 * @param divider should be &gt;0:if ≤0 will print value as integer followed by "/" divider
 * @return &lt0: error <br> ≥0: number of chars written
 */
DEBUG_FN_QUALIFIERS
int debugF3(int64_t value, int divider);

/**
 * Prints string \c s immediately followed by \c value/divider with sign in 3 decimals, as if with a "%s%+.3f" format.
 * If arg \c s is null, behaves identical to debugF3().
 * @param s string pointer
 * @param value any
 * @param divider should be &gt;0:if ≤0 will print value as integer followed by "/" divider
 * @return &lt0: error <br> ≥0: number of chars written
 */
DEBUG_FN_QUALIFIERS
int debugStringF3(const char *s, int64_t value, int divider);

/**
 * Prints a formatted debug message followed by a colon, a space,
 * \c value/divider with sign in 3 decimals and a newline.
 * If arg \c format is null, behaves identical to debugF3().
 * @param format \c printf()-style format
 * @param ... \c printf()-style arguments to match the format
 * @param value any
 * @param divider should be &gt;0:if ≤0 will print value as integer followed by "/" divider
 * @return &lt0: error <br> ≥0: number of chars written
 */
DEBUG_FN_QUALIFIERS
int debugF3nl(int64_t value, int divider, const char *format, ...);

#if DEBUG_DISABLED
/*
 * Provide dummy inline bodies for the debug functions.
 */

DEBUG_FN_QUALIFIERS
int debugNonl(const char *format, ...) {
	(void) format;
	return 0;
}

DEBUG_FN_QUALIFIERS
int debugnl(const char *format, ...) {
	(void) format;
	return 0;
}

DEBUG_FN_QUALIFIERS
int debugNL(void) {
	return 0;
}

DEBUG_FN_QUALIFIERS
int debugF3(int64_t value, int divider) {
	(void) value;
	(void) divider;
	return 0;
}

DEBUG_FN_QUALIFIERS
int debugStringF3(const char *s, int64_t value, int divider) {
	(void) s;
	(void) value;
	(void) divider;
	return 0;
}

DEBUG_FN_QUALIFIERS
int debugF3nl(int64_t value, int divider, const char *format, ...) {
	(void) value;
	(void) divider;
	(void) format;
	return 0;
}

#endif /* DEBUG_DISABLED */

#endif /* XDEBUG_H_ */
