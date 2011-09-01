/*
 * testing xvprintf()
 */

#include "test-rig.h"

extern test_printf_fn vtest_xprintf_core;

int main(void) {
	DEFINE_test_printf_info("xvprintf_protected()", vtest_xprintf_core);

	test_all_iso(tpi);

	if (endAllTests(tpi) != 0) {
		return 1;
	}

	test_pointers(tpi);
	return 0;
}
