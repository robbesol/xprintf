
See printf in the ISO C99 standard with corrigenda TC1, TC2, and TC3 included, e.g.
	http://www.open-std.org/JTC1/SC22/WG14/www/docs/n1256.pdf

Note: common extensions (like positional args "%1$d") are not supported.

This is an original implementation, written from the ground up
by Markus van Dijk, employed by Technologies88.com.
No code was borrowed from any commercial and/or open source.

Conversions

%d, %i, %o, %u, %x, %X (integral conversions): fully implemented

%f, %F (double conversions): partially implemented
-- no inf/nan output
-- the 'L' length modifier (long double) is supported but only as accurate as a double.

%e, %E, %g, %G, %a, %A (double conversions): NOT implemented.

%c: (char conversions) : fully implemented

%s: (string conversions) : partially implemented
-- the 'l' length modifier for widechars (wint_t, wchar_t) is not supported
++ %#s:  an extra '#' flag is implemented that will print the string "visually", using
 *			"\n", "\r", "\t" and "\ooo" (octal) for non-printable chars and "\\" for '\'.

%p (pointers conversion): fully implemented (implementation dependent, but close to UNIX)

%n conversion: implemented for int* only (not for %lln)

%% (the constant '%'): fully implemented :-)

Flags
All flags are implemented [#0- +].
The minimum field width and precision are implemented, including the '*' specifier.



NOTE: these functions have to be reentrant, so:
[Wikipedia]
A computer program or routine is described as reentrant
if it can be safely executed concurrently; that is,
the routine can be re-entered while it is already running.
To be reentrant, a function
		must hold no static non-constant data,
		must not return the address to static non-constant data,
		must work only on the data provided to it by the caller,
		must not rely on locks to singleton resources, and
		must not call non-reentrant functions.
