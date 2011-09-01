/*
 * Copyright(c) 2007 Technologies88 B.V.
 * All rights reserved.
 */

#include <stdio.h>
#include <stdarg.h>

#include "impl/xprintf-debug.h"

void xprintf__debug2hostV(const char * format, va_list ap) {
	 vfprintf(stderr, format, ap);
}
