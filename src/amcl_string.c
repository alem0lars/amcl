#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "amcl_string.h"
#include "amcl_log.h"


int
get_string_length(
  const char *string
) {
  return snprintf(NULL, 0, "%s", string);
}

int
str_to_uint(
  const char *src,
  unsigned int min_value,
  unsigned int max_value,
  unsigned int *dest
) {
  char* end;
  errno = 0;
  long n = strtol(src, &end, 0);
  if (   (errno != 0 && n == 0) /* Error while parsing src. */
      || (errno == EINVAL) /* The base has unsupported values. */
      || (errno == ERANGE && (n == LONG_MAX || n == LONG_MIN)) /* Under/Over. */
      || (*src == '\0' || *end != '\0')) { /* Can't convert chars to long. */
    if (errno != 0) {
      log_msg(LOG_LVL_ERR, LOG_FAC_STD, "str_to_uint()",
              "Invalid input argument: %s", strerror(errno));
    } else {
      log_msg(LOG_LVL_ERR, LOG_FAC_STD, "str_to_uint()",
              "Invalid input argument");
    }
    return -1;
  }
  if (n < 0 || n > UINT_MAX) { /* Can long be converted in unsigned int? */
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "str_to_uint()",
            "Conversion of %ld to unsigned int", n);
    return -2;
  }
  if (n < min_value || n > max_value) {
    log_msg(LOG_LVL_ERR, LOG_FAC_STD, "str_to_uint()",
            "%ld isn't inside the range (%u, %u)", n, min_value, max_value);
    return -3;
  }
  *dest = (unsigned int) n;
  log_msg(LOG_LVL_DBG, LOG_FAC_STD, "str_to_uint()",
          "Conversion of %s to unsigned int success", src);
  return 0;
}
