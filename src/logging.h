#ifndef __LOGGING_H__
#define __LOGGING_H__


/**
 * Available facilities
 */
typedef enum logging_facility {
  STANDARD, /**< Use stdout, stderr */
  SYSLOG    /**< Use syslog */
} logging_facility_t;

/**
 * Available logging levels
 */
typedef enum logging_level {
  ERROR,    /**< An error message */
  WARNING,  /**< A warning message */
  DEBUG     /**< A debugging/info message */
} logging_level_t;

#ifndef LOGGING_ERROR_ENABLED
#define LOGGING_ERROR_ENABLED 1
#endif
#ifndef LOGGING_WARNING_ENABLED
#define LOGGING_WARNING_ENABLED 1
#endif
#ifndef LOGGING_DEBUG_ENABLED
#define LOGGING_DEBUG_ENABLED 1
#endif

// { Prototypes

/**
 * \brief
 *   Log a message in the provided logging facility with the provided logging
 *   level.
 *
 * The logging is ignored if the corresponding macro is set to 0 (false),
 * so you can choose to turn off or on the logging (by default is on).
 * The macro name is: LOGGING_<logging_level>_ENABLED
 * (e.g. LOGGING_DEBUG_ENABLED)
 *
 * @param logging_level The logging level to be used
 * @param logging_facility The logging facility to be used
 * @param ident The identity of the sender for the logging message
 * @param msg The message to be logged. It can be used as a format string like
 *            the printf() functions family
 *
 * @return The logging status
 *   - 0 => Success
 *   - -1 => Failure while logging the message (if the SYSLOG facility has
 *           been used, errno indicates the error type)
 *   - -2 => Failure while creating the output string
 */
int log_msg(const logging_level_t logging_level,
    const logging_facility_t logging_facility,
    const char* ident, const char* msg, ...);

// }


#endif

