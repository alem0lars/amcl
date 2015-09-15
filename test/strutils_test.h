#ifndef __STRUTILS_TEST_H__
#define __STRUTILS_TEST_H__

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

int
suite_strutils_init(
  void
);

int
suite_strutils_cleanup(
  void
);

void
test_get_string_length(
  void
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Test-cases definition for the string module.
 */
CU_TestInfo testcases_string[] = {
  { "test_get_string_length", test_get_string_length },
  CU_TEST_INFO_NULL,
};

#endif
