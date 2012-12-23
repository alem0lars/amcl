#ifndef __LOGGING_TEST_H__
#define __LOGGING_TEST_H__


void test_log_msg(void);

int suite_logging_init(void);
int suite_logging_cleanup(void);

CU_TestInfo testcases_logging[] = {
  { "test_log_msg", test_log_msg },
  CU_TEST_INFO_NULL,
};


#endif
