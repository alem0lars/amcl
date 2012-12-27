#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>
#include <time.h>

#include "logging.h"
#include "string.h"


int log_msg(const logging_level_t logging_level,
    const logging_facility_t logging_facility,
    const char* ident, const char* msg, ...) {
  int ret = 0;
  int msg_formatted_length;
  char* msg_formatted;
  va_list args;

  if (   ((logging_level == ERROR) && LOGGING_ERROR_ENABLED)
      || ((logging_level == WARNING) && LOGGING_WARNING_ENABLED)
      || ((logging_level == DEBUG) && LOGGING_DEBUG_ENABLED)) {

    // { Get the output string
    va_start(args, msg);
    // Get the length of the output string
    msg_formatted_length = vsnprintf(NULL, 0, msg, args);
    va_end(args);
    // Save the output string in the 'msg_formatted' buffer
    if (msg_formatted_length < 0) {
      ret = -2;
    } else {
      msg_formatted_length = msg_formatted_length + 1;
      msg_formatted = (char*) malloc(msg_formatted_length * sizeof(char));
      va_start(args, msg);
      // Setup the 'msg_formatted' string
      ret = (vsnprintf(msg_formatted, msg_formatted_length, msg, args) < 0) ? -2 : 0;
      va_end(args);
    }
    // }
    
    // { Try to print the output string
    if (!ret) {
      time_t time_now;
      int time_string_size = get_string_length(ctime(&time_now));
      char* time_string = malloc(time_string_size * sizeof(char));

      time(&time_now);
      strftime(time_string, time_string_size, "%H:%M:%S", localtime(&time_now));

      if (logging_facility == STANDARD) { // Log with the standard files
        FILE* out_file;

        if (logging_level == ERROR) {
          out_file = stderr;
        } else if (logging_level == WARNING) {
          out_file = stdout;
        } else if (logging_level == DEBUG) {
          out_file = stdout;
        }

        if (fprintf(out_file, ">> [%s][pid=%d][%s][%s] %s\n", time_string,
              getpid(),
              (logging_level == ERROR ? "ERROR" :
                (logging_level == WARNING ? "WARNING" :
                  (logging_level == DEBUG ? "DEBUG" : "undefined")))
              , ident, msg_formatted) < 0) {
          ret = -1;
        }

        free(time_string);

      } else if (logging_facility == SYSLOG) { // Log with syslog
        openlog(ident, LOG_CONS | LOG_NDELAY | LOG_NOWAIT | LOG_PID,
            LOG_USER);

        errno = 0;
        syslog(
            (logging_level == ERROR) ? LOG_ERR :
                ((logging_level == WARNING) ? LOG_WARNING : LOG_DEBUG),
            msg_formatted, msg_formatted_length);

        closelog(); // Close the logging descriptor
      }
    }
    // }
  }

  free(msg_formatted);

  return ret;
}

