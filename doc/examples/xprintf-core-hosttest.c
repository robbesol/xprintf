// example of implementation of your own print system

#include "impl/xprintf-core.h"

static int my_write(struct xprintf_protectedChannel *xprch, const char *pData,
		unsigned int length) {
	int outCount = 0;
	// here code to write pData[0..length] to its final destination
	return outCount;
}

static struct xprintf_protectedChannel my_chan = { .writeProtected = my_write };

int my_vprintf(const char *format, va_list ap) {
	// here code to lock against simultaneous use
	int outCount = xvprintf_protected(&my_chan, format, ap);
	// here code to release lock
	return outCount;
}

#ifdef __GNUC__
// with GCC  you would want to use this attribute:
int my_printf(const char *format, ...) __attribute__ ((format (printf, 1, 2)));
#endif

int my_printf(const char *format, ...) {
	va_list ap;
	va_start(ap, format);
	int outCount = my_vprintf(format, ap);
	va_end(ap);
	return outCount;
}
