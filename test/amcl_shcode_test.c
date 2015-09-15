#include <CUnit/CUnit.h>

#include "../src/amcl_shcode.h"
#include "../src/amcl_log.h"


int
suite_shcode_init(
) {
  return 0;
}

int
suite_shcode_cleanup(
) {
  return 0;
}

void
test_exec_shcode(
  void
) {
#if defined(__i386__)
  char *shcode = ""; /* TODO: shellcode: echo "hello world" */
#elif defined(__x86_64__)
  char *shcode = ""; /* TODO: shellcode: echo "hello world" */
#else
  log_msg(LOG_LVL_ERR, LOG_FAC_STD, "test_exec_shcode()",
          "Unsupported architecture");
#endif
  exec_shcode(shcode);
}
