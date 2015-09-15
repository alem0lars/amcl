#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <CUnit/CUnit.h>

#include "../src/amcl_log.h"
#include "../src/amcl_string.h"


int
suite_log_init(
) {
  return 0;
}

int
suite_log_cleanup(
) {
  return 0;
}

void
test_log_msg(
) {
  int ret = 0;
  char *test_file_name = "test_log_msg.txt";
  char *ident = "test_log_msg()";

  /* {{{ Calculate lengths. */
  int ident_length = get_string_length(ident);
  int msg_length = 13;
  int pid_length = snprintf(NULL, 0, "%d", getpid());
  int expected_length = 18 + pid_length + 9 + ident_length + 2 + msg_length + 1;
  /* }}} */

  /* {{{ Redirect the stdout. */
  int stdout_dupfd = dup(1);
  FILE *temp_out = fopen(test_file_name, "w");
  dup2(fileno(temp_out), 1);
  /* }}} */

  /* Log the message in the file. */
  ret = log_msg(LOG_LVL_DBG, LOG_FAC_STD, "test_log_msg()",
                "Hello %s %d", "World", 1);
  fflush(stdout); /* Flush output so it goes to our file. */

  /* {{{ Restore stdout. */
  fclose(temp_out);
  dup2(stdout_dupfd, 1);
  close(stdout_dupfd);
  /* }}} */

  /* Test that the log_msg doesn't fail. */
  CU_ASSERT(!ret);

  FILE *stdout_file = fopen(test_file_name, "r");
  char *read_buffer = malloc((expected_length + 1) * sizeof(char));
  /* Try to read the expected_length + 1 so we can check if more chars than
   * those expected are written.
   */
  int read_length = fread(read_buffer, sizeof(char), expected_length + 1,
                          stdout_file);
  free(read_buffer);
  fclose(stdout_file);
  unlink(test_file_name);

  /* Test that we only read the written bytes. */
  CU_ASSERT(read_length == expected_length);
}
