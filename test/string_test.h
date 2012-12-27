#ifndef __STRING_TEST_H__
#define __STRING_TEST_H__


void test_get_string_length(void);

int suite_string_init(void);
int suite_string_cleanup(void);

CU_TestInfo testcases_string[] = {
  { "test_get_string_length", test_get_string_length },
  CU_TEST_INFO_NULL,
};


#endif
