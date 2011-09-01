/*
 * testing 1, 2
 */

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

#include "test-runner.h"
#include "test-rig.h"

#define TESTPCTN(type, lenspec) { \
		type result; \
		TEST("abcd12", "abc%#" lenspec "nd%d", &result, 12); \
		if (result != 3) { \
			logErrorRemark(tpi, "abc%#" lenspec "nd", "expected result 3, got %" lenspec "d", result); \
		} }

void test_iso_ints(struct test_printf_info *tpi) {
	startTests(tpi, __func__);

	/* return value */
	TEST("abc", "abc");

//	TEST("50", "%d", 5);

	/* %d, basic formatting */
	TEST("5", "%d", 5);
	TEST("0", "%d", 0);
	TEST("", "%.0d", 0);
	TEST("1", "%.0d", 1);
	TEST("2", "%.d", 2);
	TEST("-1", "%d", -1);
	TEST("005", "%.3d", 5);
	TEST("-005", "%.3d", -5);
	TEST("  005", "%5.3d", 5);
	TEST("-005 ", "%-5.3d", -5);

	/* %d, length modifiers */
	TEST("-5", "%hhd", (signed char) -5);
	TEST("-5", "%hd", (short) -5);
	TEST("-5", "%d", (int) -5);
	TEST("-5", "%ld", (long) -5);
	TEST("-5", "%lld", (long long) -5);
	TEST("-5", "%jd", (intmax_t) -5);
	TEST("5", "%jd", (intmax_t) 5);
	TEST("-5", "%td", (ptrdiff_t) -5);
	TEST("5", "%td", (ptrdiff_t) 5);

	/* %d, flags */
	TEST("5", "%-d", 5);
	TEST("+5", "%-+d", 5);
	TEST("+5", "%+-d", 5);
	TEST("-5", "%+d", -5);
	TEST(" 5", "% d", 5);
	TEST(" ", "% .0d", 0);
	TEST("+5", "% +d", 5);
	TEST("005", "%03d", 5);
	TEST("-5 ", "%-03d", -5);
	TEST("-05", "%03d", -5);

	/* %u, basic formatting */
	TEST("5", "%u", 5);
	TEST("0", "%u", 0);
	TEST("", "%.0u", 0);
	TEST("1", "%.0u", 1);
	TEST("005", "%.3u", 5);
	TEST("  005", "%5.3u", 5);

	/* %u, length */
	TEST("5", "%hhu", (unsigned char) 5);
	TEST("5", "%hu", (unsigned short) 5);
	TEST("5", "%u", (unsigned int) 5);
	TEST("5", "%lu", (unsigned long) 5);
	TEST("5", "%llu", (unsigned long long) 5);
	TEST("5", "%ju", (uintmax_t) 5);
	TEST("5", "%zu", (size_t) 5);

	/* macros */
	TEST("-5", "%"PRId8, (int8_t) -5);
	TEST("5", "%"PRIu8, (uint8_t) 5);
	TEST("-5", "%"PRId16, (int16_t) -5);
	TEST("5", "%"PRIu16, (uint16_t) 5);
	TEST("-5", "%"PRId32, (int32_t) -5);
	TEST("5", "%"PRIu32, (uint32_t) 5);
	TEST("-5", "%"PRId64, (int64_t) -5); // PRId32 expands to "qd": non-ISO but supported
	TEST("5", "%"PRIu64, (uint64_t) 5); // PRIu32 expands to "qu": non-ISO but supported

	/* %o, basic formatting */
	TEST("5", "%o", 5);
	TEST("10", "%o", 8);
	TEST("0", "%o", 0);
	TEST("", "%.0o", 0);
	TEST("1", "%.0o", 1);
	TEST("005", "%.3o", 5);
	TEST("010", "%.3o", 8);
	TEST("  005", "%5.3o", 5);
	TEST("0", "%o", 0);
	TEST("0", "%0o", 0);
	TEST("5", "%o", 5);
	TEST("5", "%0o", 5);
	TEST("5", "%1o", 5);
	TEST("5", "%01o", 5);
	TEST("0", "%1o", 0);
	TEST("0", "%01o", 0);
	TEST(" 5", "%2o", 5);
	TEST("05", "%02o", 5);
	TEST("    5", "%5o", 5);
	TEST("00005", "%05o", 5);

	TEST("0", "%#o", 0);
	TEST("0", "%#0o", 0);
	TEST("05", "%#o", 5);
	TEST("05", "%#0o", 5);
	TEST("05", "%#1o", 5);
	TEST("05", "%#01o", 5);
	TEST("0", "%#1o", 0);
	TEST("0", "%#01o", 0);
	TEST("05", "%#2o", 5);
	TEST("05", "%#02o", 5);
	TEST("   05", "%#5o", 5);
	TEST("00005", "%#05o", 5);

	/* %o, length */
	TEST("15", "%hho", (unsigned char) 13);
	TEST("15", "%ho", (unsigned short) 13);
	TEST("15", "%o", (unsigned int) 13);
	TEST("15", "%lo", (unsigned long) 13);
	TEST("15", "%llo", (unsigned long long) 13);
	TEST("15", "%jo", (uintmax_t) 13);
	TEST("15", "%zo", (size_t) 13);

	/* %x, basic formatting */
	TEST("5", "%x", 5);
	TEST("1f", "%x", 31);
	TEST("0", "%x", 0);
	TEST("", "%.0x", 0);
	TEST("1", "%.0x", 1);
	TEST("005", "%.3x", 5);
	TEST("01f", "%.3x", 31);
	TEST("  005", "%5.3x", 5);
	/* %x, flags */
	TEST("5", "%-x", 5);
	TEST("005", "%03x", 5);
	TEST("0x1f", "%#x", 31);
	TEST("0", "%#x", 0);

	TEST("00000000", "%08x", 0);
	TEST("00000000", "%#08x", 0);
	TEST("00000000", "%08X", 0);
	TEST("00000000", "%#08X", 0);

	/* %X, basic formatting */
	TEST("5", "%X", 5);
	TEST("1F", "%X", 31);
	TEST("0", "%X", 0);
	TEST("", "%.0X", 0);
	TEST("1", "%.0X", 1);
	TEST("005", "%.3X", 5);
	TEST("01F", "%.3X", 31);
	TEST("  005", "%5.3X", 5);
	/* %X, flags */
	TEST("5", "%-X", 5);
	TEST("005", "%03X", 5);
	TEST("0X1F", "%#X", 31);
	TEST("0", "%#X", 0);

	/* %n */
	TESTPCTN(signed char, "hh")
	TESTPCTN(short, "h")
	TESTPCTN(short, "h")
	TESTPCTN(int, "")
	TESTPCTN(long, "l")
	TESTPCTN(long long, "ll")
	TESTPCTN(intmax_t, "j")
	TESTPCTN(size_t, "z")
	TESTPCTN(ptrdiff_t, "t")

	for (int i = 0; i <= 0xFF; ++i) {
		TESTHOST("%.2hho", (char) i);
	}

	for (int i = 0; i <= 0xFF; ++i) {
		TESTHOST("%#.2hhX", (char) i);
	}

	for (int i = -103; i <= 103; ++i) {
		TESTHOST("%d", i);
	}

	TEST("00000000", "%08x", 0);
	TEST("00000136", "%08x", 310);
	TEST("00000000", "%#08x", 0);
	TEST("0x000136", "%#08x", 310);
	TEST("00000000", "%#08.8x", 0);
	TEST("0x00000136", "%#08.8x", 310);

	endTests(tpi);
}

void test_iso_strings(struct test_printf_info *tpi) {
	startTests(tpi, __func__);

	/* %c */
	TEST("a", "%c", 'a');

	/* %s */
	TEST("ab", "%.2s", "abc");
	TEST("abc", "%.6s", "abc");
	TEST("  abc", "%5s", "abc");
	TEST("abc  ", "%-5s", "abc");
	TEST("   ab", "%5.2s", "abc");
	TEST("  abc", "%*s", 5, "abc");
	TEST("abc  ", "%*s", -5, "abc");
	TEST("   ab", "%*.*s", 5, 2, "abc");

	/* %% */
	TEST("%", "%%");

	//    /* positional parameters */
	//    TEST("a b", "%2$c %1$c", 'b', 'a');
	//    TEST("   ab", "%1$*2$.*3$s", "abc", 5, 2);
	//    TEST("abcabc", "%1$s%1$s", "abc");

	endTests(tpi);
}

void test_all_iso(struct test_printf_info *tpi) {
	test_iso_ints(tpi);
	test_iso_strings(tpi);
	test_iso_doubles(tpi);
	test_BrunoHaible(tpi);
}

