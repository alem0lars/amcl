#ifndef __LOGGING_TEST_H__
#define __LOGGING_TEST_H__

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

int
suite_logging_init(
  void
);

int
suite_logging_cleanup(
  void
);

void
test_log_msg(
  void
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Test-cases definition for the logging module.
 */
CU_TestInfo testcases_logging[] = {
  { "test_log_msg", test_log_msg },
  CU_TEST_INFO_NULL,
};

#endif
