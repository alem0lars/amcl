#include <stdio.h>

#include "amcl_string.h"
#include "amcl_log.h"


void
exec_shellcode(
  const char *shellcode
) {
  log_msg(LOG_LVL_DBG, LOG_FAC_STD, "Shellcode executor",
          "Executing shellcode (length: %d)",
          get_string_length(shellcode));
  int (*ret)() = (int(*)())shellcode;
  ret();
}
