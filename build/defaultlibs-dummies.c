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

/*
 * Provide dummy implementations for the symbols allowed in the restricted libc.
 * This is used to check that a library does not need any symbols outside that set.
 *
 * Use weak linking (\code __attribute__ ((weak)) \endcode),
 * because some symbols may be overridden by implementations.
 *
 * No need to cater for all strict warnings of the c compiler.
 */

#define WEAKDEF(decl)	decl __attribute__ ((weak));; decl { /* dummy body */ }

struct xprintf_channel;
struct xprintf_protectedChannel;

WEAKDEF(int xprintf_beginProtectedIO(struct xprintf_channel *xch,
				struct xprintf_protectedChannel *xchpr))
WEAKDEF(int xprintf_endProtectedIO(struct xprintf_channel *xch,
				struct xprintf_protectedChannel *xchpr))

//#include <ctype.h>
//const char *__ctype_ptr; // macros in <ctype.h> need this global variable
//const char *__ctype_ptr__; // same for gcc 4.3.2

#include <string.h> // for memset()
// <string.h> strl*(), not standard c, but we need them anyway
//WEAKDEF(size_t strlcat(char *str1, const char *str2, size_t len))
//WEAKDEF(size_t strlcpy(char *str1, const char *str2, size_t len))
// <string.h> str*() that are needed:
//WEAKDEF(size_t strlen(const char *str))
//WEAKDEF(int strcmp(const char *str1, const char *str2))

// <string.h> str*(), not yet needed:
//WEAKDEF(char *strchr(const char *str, int i))
//WEAKDEF(char *strrchr(const char *str, int i))
//WEAKDEF(char *strstr(const char *str1, const char *str2))

// I sincerely hope these are not needed.
// If you need to ask why, please find yourself another profession.
// char *strncat(char *str1, const char *str2, size_t len);
// int strncmp(const char *str1, const char str2*, size_t len);
// char *strncpy(char *str1, const char *str2, size_t len);

// <string.h> mem*() that are needed:
//WEAKDEF(int memcmp(const void *p1, const void *p2, size_t len))
#ifdef memset
#undef memset
#endif
WEAKDEF(void *memset(void *p, int val, size_t len))
//WEAKDEF(void *memcpy(void *p1, const void *p2, size_t size))

// <string.h> mem*(), not yet needed:
//WEAKDEF(void *memchr(const void *, int, size_t))
//WEAKDEF(void *memmove(void *, const void *, size_t))

#ifndef XPRINTF_NO_DOUBLES

#include <math.h>
WEAKDEF(int __fpclassifyf(float x))
// needed for fpclassify() macro
WEAKDEF(int __fpclassifyd(double x))
// needed for fpclassify() macro
WEAKDEF(int __signbitf (float x))
// for signbit() macro
WEAKDEF(int __signbitd (double x))
// for signbit() macro
WEAKDEF(double modf(double d, double *dp))
WEAKDEF(double fmod(double d1, double d2))
WEAKDEF(double log10(double d))
WEAKDEF(double floor(double d))
WEAKDEF(double ceil(double d))

WEAKDEF(double fabs(double x))

#endif /* ! XPRINTF_NO_DOUBLES */
