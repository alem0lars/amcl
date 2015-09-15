#ifndef __AMCL_SHCODE_TEST_H__
#define __AMCL_SHCODE_TEST_H__

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

int
suite_shcode_init(
  void
);

int
suite_shcode_cleanup(
  void
);

void
test_exec_shcode(
  void
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Test-cases definition for the 'amcl_shcode' module.
 */
CU_TestInfo testcases_shcode[] = {
  { "test_exec_shcode", test_exec_shcode },
  CU_TEST_INFO_NULL,
};

#endif
