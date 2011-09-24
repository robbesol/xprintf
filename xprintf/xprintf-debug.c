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

#include "impl/xprintf-debug.h"

void xprintf__debug2host(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	xprintf__debug2hostV(format, ap);
	va_end(ap);
}

void xprintf__debugFormatV(const char * const format,
		const struct xprintf__conversionPparams *params, char consSpec,
		const char *msgFmt, va_list ap) {

	xprintf__debug2host("***** xvprintf(\"%s\"", format);
	if (params == 0) {
		xprintf__debug2host(" == ?)\n");
		return;
	}

	xprintf__debug2host(" -> ");
	if (params->sign != '\0') {
		xprintf__debug2host("%c ", params->sign);
	}
	xprintf__debug2host("\"%%");
	if (params->flags.alternative) {
		xprintf__debug2host("#");
	}
	if (params->flags.signspace) {
		xprintf__debug2host(" ");
	}
	if (params->flags.signplus) {
		xprintf__debug2host("+");
	}
	if (params->flags.leftjustified) {
		xprintf__debug2host("-");
	}
	if (params->flags.leadingzeroes) {
		xprintf__debug2host("0");
	}
	if (params->minWidth != 0) {
		xprintf__debug2host("%d", params->minWidth);
	}
	if (params->precision >= 0) {
		xprintf__debug2host(".%d", params->precision);
	}
	xprintf__debug2host("%c\"", consSpec);
	xprintf__debug2host(" base %d): ", params->digitBase);
	xprintf__debug2hostV(msgFmt, ap);
	xprintf__debug2host("\n");
}
