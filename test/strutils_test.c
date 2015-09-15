#include <limits.h>
#include <stdlib.h>
#include <CUnit/CUnit.h>

#include "../src/strutils.h"


int
suite_strutils_init(
) {
  return 0;
}

int
suite_strutils_cleanup(
) {
  return 0;
}

void
test_get_string_length(
) {
  char *str1 = "Hello";
  char *str2 = "Hello World!";
  char *str3 = "";

  CU_ASSERT(5  == get_string_length(str1));
  CU_ASSERT(12 == get_string_length(str2));
  CU_ASSERT(0  == get_string_length(str3));
}

void
test_str_to_uint(
) {
  int res;
  char *long_digit = malloc(20 * sizeof(char)); /* No digits occupy more. */
  memset(long_digit, 0xff, 20 * sizeof(char));
  sprintf(long_digit, "%lld",  (long long int) UINT_MAX + 1);

  /* {{{ 1. Test against invalid strings (that aren't valid numbers). */
  CU_ASSERT(-1 == str_to_uint("",      0, UINT_MAX, &res));
  CU_ASSERT(-1 == str_to_uint("foo",   0, UINT_MAX, &res));
  CU_ASSERT(-1 == str_to_uint("12foo", 0, UINT_MAX, &res));
  CU_ASSERT(-1 == str_to_uint("foo12", 0, UINT_MAX, &res));
  CU_ASSERT(-1 == str_to_uint("1foo2", 0, UINT_MAX, &res));
  /* }}} */

  /* {{{ 2. Test against UINT under/over-flows. */
  CU_ASSERT(-2 == str_to_uint("-1",       0, UINT_MAX, &res));
  CU_ASSERT(-2 == str_to_uint(long_digit, 0, UINT_MAX, &res));
  /* }}} */

  /* {{{ 3. Test against provided limits under/over-flows. */
  CU_ASSERT(-3 == str_to_uint("0",  1,  1,  &res));
  CU_ASSERT(-3 == str_to_uint("2",  1,  1,  &res));
  CU_ASSERT(-3 == str_to_uint("8",  10, 20, &res));
  CU_ASSERT(-3 == str_to_uint("12", 0,  10, &res));
  /* }}} */

  /* {{{ 4. Test against valid numbers. */
  res = -1;
  str_to_uint("10", 0, UINT_MAX, &res);
  CU_ASSERT(res == 10);
  str_to_uint("0xff", 0, UINT_MAX, &res);
  CU_ASSERT(res == 255);
  /* }}} */

  free(long_digit);
}
