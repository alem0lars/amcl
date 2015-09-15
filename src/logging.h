#ifndef __LOGGING_H__
#define __LOGGING_H__

/* {{{ Types. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * Available facilities.
 */
typedef enum log_facility {
  LOG_FAC_STD, /**< Use stdout, stderr. */
  LOG_FAC_SYS  /**< Use syslog. */
} log_facility_t;

/**
 * Available logging levels.
 */
typedef enum log_level {
  LOG_LVL_ERR, /**< An error message. */
  LOG_LVL_WRN, /**< A warning message. */
  LOG_LVL_DBG  /**< A debugging/info message. */
} log_level_t;

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/* {{{ Constants. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

#ifndef LOG_ERR_ENABLED
#define LOG_ERR_ENABLED 1
#endif
#ifndef LOG_WRN_ENABLED
#define LOG_WRN_ENABLED 1
#endif
#ifndef LOG_DBG_ENABLED
#define LOG_DBG_ENABLED 1
#endif

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/* {{{ Prototypes. ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/**
 * \brief
 *   Log a message in the provided logging facility with the provided logging
 *   level.
 *
 * The logging is ignored if the corresponding macro is set to 0 (false),
 * so you can choose to turn off or on the logging (by default is on).
 * The macro name is: LOG_<log_level>_ENABLED (e.g. LOG_DBG_ENABLED).
 *
 * @param logging_level The logging level to be used.
 * @param logging_facility The logging facility to be used.
 * @param ident The identity of the sender for the logging message.
 * @param msg The message to be logged. It can be used as a format string like
 *            the printf() functions family.
 *
 * @return The logging status:
 *   -  0 => Success.
 *   - -1 => Failure while logging the message (if the SYSLOG facility has
 *           been used, errno indicates the error type).
 *   - -2 => Failure while creating the output string.
 */
int
log_msg(
  const log_level_t log_level,
  const log_facility_t log_facility,
  const char *ident,
  const char *msg,
  ...
);

/* }}} ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

#endif
