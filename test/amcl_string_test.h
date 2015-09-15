#ifndef __AMCL_STRING_TEST_H__
#define __AMCL_STRING_TEST_H__

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

int
suite_string_init(
  void
);

int
suite_string_cleanup(
  void
);

void
test_get_string_length(
  void
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Test-cases definition for the 'amcl_string' module.
 */
CU_TestInfo testcases_string[] = {
  { "test_get_string_length", test_get_string_length },
  CU_TEST_INFO_NULL,
};

#endif
