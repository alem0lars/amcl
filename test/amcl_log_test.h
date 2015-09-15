#ifndef __AMCL_LOG_TEST_H__
#define __AMCL_LOG_TEST_H__

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

int
suite_log_init(
  void
);

int
suite_log_cleanup(
  void
);

void
test_log_msg(
  void
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Test-cases definition for the 'amcl_log' module.
 */
CU_TestInfo testcases_log[] = {
  { "test_log_msg", test_log_msg },
  CU_TEST_INFO_NULL,
};

#endif
