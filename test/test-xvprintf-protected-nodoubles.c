/*
 * testing xvprintf()
 */

#include "test-rig.h"

extern test_printf_fn vtest_xprintf_core;

int main(void) {
	DEFINE_test_printf_info("xvprintf_protected(nodoubles)", vtest_xprintf_core);

	test_iso_ints(tpi);
	test_iso_strings(tpi);
	test_nodoubles(tpi);

	if (endAllTests(tpi) != 0) {
		return 1;
	}

	test_pointers(tpi);
	return 0;
}
