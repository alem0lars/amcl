#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>
#include <time.h>

#include "amcl_log.h"
#include "amcl_string.h"


int
log_msg(
  const log_level_t log_level,
  const log_facility_t log_facility,
  const char* ident,
  const char* msg,
  ...
) {
  int ret = 0;
  int msg_formatted_length;
  char* msg_formatted;
  va_list args;

  if (   ((log_level == LOG_LVL_ERR) && LOG_ERR_ENABLED)
      || ((log_level == LOG_LVL_WRN) && LOG_WRN_ENABLED)
      || ((log_level == LOG_LVL_DBG) && LOG_DBG_ENABLED)) {

    /* {{{ Get the output string. */
    va_start(args, msg);
    /* Get the length of the output string. */
    msg_formatted_length = vsnprintf(NULL, 0, msg, args);
    va_end(args);
    /* Save the output string in the 'msg_formatted' buffer. */
    if (msg_formatted_length < 0) {
      ret = -2;
    } else {
      msg_formatted_length = msg_formatted_length + 1;
      msg_formatted = (char*) malloc(msg_formatted_length * sizeof(char));
      va_start(args, msg);
      /* Setup the 'msg_formatted' string. */
      if (vsnprintf(msg_formatted, msg_formatted_length, msg, args) < 0) {
        ret = -2;
      } else {
        ret = 0;
      }
      va_end(args);
    }
    /* }}} */

    /* {{{ Try to print the output string. */
    if (!ret) {
      time_t time_now;
      int time_string_size = get_string_length(ctime(&time_now));
      char* time_string = malloc(time_string_size * sizeof(char));

      time(&time_now);
      strftime(time_string, time_string_size, "%H:%M:%S", localtime(&time_now));

      if (log_facility == LOG_FAC_STD) { /* Log with the standard files. */
        FILE* out_file;

        if (log_level == LOG_LVL_ERR) {
          out_file = stderr;
        } else if (log_level == LOG_LVL_WRN) {
          out_file = stdout;
        } else if (log_level == LOG_LVL_DBG) {
          out_file = stdout;
        }

        if (fprintf(out_file, ">> [%s][pid=%d][%s][%s] %s\n", time_string,
                    getpid(),
                    (log_level == LOG_LVL_ERR ? "ERROR" :
                    (log_level == LOG_LVL_WRN ? "WARNING" :
                    (log_level == LOG_LVL_DBG ? "DEBUG" : "undefined"))),
                    ident,
                    msg_formatted) < 0) {
          ret = -1;
        }

        free(time_string);

      } else if (log_facility == LOG_FAC_SYS) { /* Log with syslog. */
        openlog(ident, LOG_CONS | LOG_NDELAY | LOG_NOWAIT | LOG_PID,
            LOG_USER);

        errno = 0;
        syslog((log_level == LOG_LVL_ERR ? LOG_ERR :
               (log_level == LOG_LVL_WRN ? LOG_WARNING :
               (LOG_DEBUG))),
            msg_formatted, msg_formatted_length);

        closelog(); /* Close the log descriptor. */
      }
    }
    /* }}} */
  }

  free(msg_formatted);

  return ret;
}
