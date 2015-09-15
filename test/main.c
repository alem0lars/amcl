#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>

#include "amcl_log_test.h"
#include "amcl_remote_test.h"
#include "amcl_string_test.h"
#include "amcl_shcode_test.h"


/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

void run_tests(void);
void generate_report(void);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/* {{{ Test Suites. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

CU_SuiteInfo suites[] = {
  { "log",       suite_log_init,    suite_log_cleanup,    testcases_log },
  { "remote",    suite_remote_init, suite_remote_cleanup, testcases_remote },
  { "string",    suite_string_init, suite_string_cleanup, testcases_string },
  { "shellcode", suite_shcode_init, suite_shcode_cleanup, testcases_shcode },
  CU_SUITE_INFO_NULL,
};

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

int
main(
  int argc,
  char **argv
) {
  /* 1. Initialize the CUnit test registry. */
  if (CU_initialize_registry() != CUE_SUCCESS) {
    return CU_get_error();
  }
  /* 2. Register test suites. */
  CU_ErrorCode error = CU_register_suites(suites);

  /* 3. Run tests. */
  run_tests();
  generate_report();

  /* 4. Clean up registry and return. */
  CU_cleanup_registry();
  return CU_get_error();
}

void
run_tests(
) {
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  printf("\n");
  CU_basic_show_failures(CU_get_failure_list());
  printf("\n");
}

void
generate_report(
) {
  CU_automated_run_tests();
  CU_list_tests_to_file();
}
