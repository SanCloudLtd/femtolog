/*
 * femtolog
 * Copyright (c) 2021-2022 SanCloud Ltd
 * Based on log.c, Copyright (c) 2020 rxi
 * SPDX-License-Identifier: MIT
 */

/**
 * \file femtolog.h
 */

#pragma once

#include <stdarg.h>
#include <stdbool.h>

#ifdef DOXYGEN
#define FEMTOLOG_MIN_LEVEL 0
#define __attribute__(x)
#define static
#endif

/**
 * \defgroup management Management functions
 * @{
 */

/**
 * Prototype for an output function to be used by femtolog.
 *
 * When femtolog is initialised via femtolog_init(), a function of this
 * type is given. This function will be called by femtolog to output
 * each logging message, provided that the current log level is less
 * than or equal to the message log level.
 *
 * The program using femtolog is responsible for implementing such a
 * function and can therefore control where log messages are sent. For
 * example, messages could be sent to stderr or to a file in a typical
 * userspace program; or to a serial port in a freestanding application.
 *
 * An example of an appropriate function can be found in
 * femtolog-test.c.
 *
 * \param prefix    The message's log level, rendered as an upper-case
 *                  string.
 * \param fmt       The message's printf-style format string.
 * \param args      The message's additional arguments as a va_list, to
 *                  be interpreted according to the format string.
 */
typedef void (*femtolog_output_fn)(const char *prefix,
                                   const char *fmt,
                                   va_list args);

/**
 * Initialise the femtolog library.
 *
 * \note This function must be called at the start of program execution,
 *       before any other functions from the femtolog library are used.
 *
 * \param level     The initial log level. This will typically be
 *                  \ref FEMTOLOG_INFO when a program is used in
 *                  production or \ref FEMTOLOG_DEBUG when a program is
 *                  used in development.
 * \param output_fn The initial output function.
 */
void femtolog_init(int level, femtolog_output_fn output_fn);

/**
 * Determine if a given log level is valid.
 *
 * \param level The log level to test.
 *
 * \returns true if \a level is a valid log level, otherwise false.
 */
bool femtolog_level_is_valid(int level);

/**
 * Convert a log level to an upper-case string.
 *
 * \param level The log level to convert.
 *
 * \returns The name of the given log level or "(unknown)" if \a level
 *          is not a valid log level.
 */
const char *femtolog_level_to_name(int level);

/**
 * Convert an upper-case string to a log level.
 *
 * \param name The string to convert.
 *
 * \returns The log level, or \ref FEMTOLOG_UNKNOWN if \a name is not a
 *          valid log level.
 */
int femtolog_name_to_level(const char *name);

/**
 * Change the current log level.
 *
 * \param level The new log level.
 */
void femtolog_set_level(int level);

/**
 * Get the current log level.
 *
 * \returns The current log level.
 */
int femtolog_get_level();

/**
 * Set the current output function.
 *
 * \param output_fn The new output function.
 */
void femtolog_set_output_fn(femtolog_output_fn output_fn);

/**
 * Get the current output function.
 *
 * \returns The current output function.
 */
femtolog_output_fn femtolog_get_output_fn();

/**@}*/

/**
 * \defgroup logging Logging types and functions
 * @{
 */

/**
 * "TRACE": The lowest logging level, for highly detailed debug
 * output which will rarely be enabled.
 */
#define FEMTOLOG_TRACE 0

/**
 * "DEBUG": The logging level for verbose debug messages which are
 * intended for use by developers and when investigating issues.
 */
#define FEMTOLOG_DEBUG 1

/**
 * "INFO": The standard logging level for messages intended for
 * general users of the program.
 */
#define FEMTOLOG_INFO 2

/**
 * "WARN": The logging level for warnings which indicate a potential
 * issue.
 */
#define FEMTOLOG_WARN 3

/**
 * "ERROR": The logging level for program error messages, used when
 * the program can recover or otherwise continue to execute.
 */
#define FEMTOLOG_ERROR 4

/**
 * "FATAL": The logging level for program error messages, used when
 * the program cannot continue.
 */
#define FEMTOLOG_FATAL 5

/**
 * "UNKNOWN": A placeholder logging level used internally by
 * femtolog.
 */
#define FEMTOLOG_UNKNOWN -1

/**
 * Emit a log message with a dynamic log level and arguments in va_list
 * form.
 *
 * \note Calls to this function are always retained during compilation,
 *       regardless of the value of \ref FEMTOLOG_MIN_LEVEL.
 *
 * \param level     The message log level.
 * \param fmt       A printf-style format string.
 * \param args      Additional arguments as a va_list, interpreted
 *                  according to the format string.
 */
void femtolog_vlog(int level, const char *fmt, va_list args);

/**
 * Emit a log message with a dynamic log level.
 *
 * \note Calls to this function are always retained during compilation,
 *       regardless of the value of \ref FEMTOLOG_MIN_LEVEL.
 *
 * \param level     The message log level.
 * \param fmt       A printf-style format string.
 * \param ...       Additional arguments, interpreted according to the
 *                  format string.
 */
__attribute__((format(printf, 2, 3)))
static inline void femtolog_log(int level, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(level, fmt, args);
    va_end(args);
}

#ifndef FEMTOLOG_NO_MACROS
#ifndef FEMTOLOG_MIN_LEVEL
#define FEMTOLOG_MIN_LEVEL FEMTOLOG_INFO
#endif

#if (FEMTOLOG_MIN_LEVEL < FEMTOLOG_TRACE) || (FEMTOLOG_MIN_LEVEL > FEMTOLOG_FATAL)
#error FEMTOLOG_MIN_LEVEL is invalid
#endif

#if (FEMTOLOG_MIN_LEVEL <= FEMTOLOG_TRACE)
/**
 * Emit a log message at the level \ref FEMTOLOG_TRACE.
 *
 * \note This function is not available if \ref FEMTOLOG_NO_MACROS is
 *       defined.
 *
 * \note This function is a no-op if \ref FEMTOLOG_MIN_LEVEL is set to a
 *       higher level than \ref FEMTOLOG_TRACE.
 *
 * \param fmt A printf-style format string.
 * \param ... Additional arguments, interpreted according to the format
 *            string.
 */
__attribute__((format(printf, 1, 2)))
static inline void log_trace(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(FEMTOLOG_TRACE, fmt, args);
    va_end(args);
}
#else
#define log_trace(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_DEBUG
/**
 * Emit a log message at the level \ref FEMTOLOG_DEBUG.
 *
 * \note This function is not available if \ref FEMTOLOG_NO_MACROS is
 *       defined.
 *
 * \note This function is a no-op if \ref FEMTOLOG_MIN_LEVEL is set to a
 *       higher level than \ref FEMTOLOG_DEBUG.
 *
 * \param fmt A printf-style format string.
 * \param ... Additional arguments, interpreted according to the format
 *            string.
 */
__attribute__((format(printf, 1, 2)))
static inline void log_debug(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(FEMTOLOG_DEBUG, fmt, args);
    va_end(args);
}
#else
#define log_debug(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_INFO
/**
 * Emit a log message at the level \ref FEMTOLOG_INFO.
 *
 * \note This function is not available if \ref FEMTOLOG_NO_MACROS is
 *       defined.
 *
 * \note This function is a no-op if \ref FEMTOLOG_MIN_LEVEL is set to a
 *       higher level than \ref FEMTOLOG_INFO.
 *
 * \param fmt A printf-style format string.
 * \param ... Additional arguments, interpreted according to the format
 *            string.
 */
__attribute__((format(printf, 1, 2)))
static inline void log_info(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(FEMTOLOG_INFO, fmt, args);
    va_end(args);
}
#else
#define log_info(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_WARN
/**
 * Emit a log message at the level \ref FEMTOLOG_WARN.
 *
 * \note This function is not available if \ref FEMTOLOG_NO_MACROS is
 *       defined.
 *
 * \note This function is a no-op if \ref FEMTOLOG_MIN_LEVEL is set to a
 *       higher level than \ref FEMTOLOG_WARN.
 *
 * \param fmt A printf-style format string.
 * \param ... Additional arguments, interpreted according to the format
 *            string.
 */
__attribute__((format(printf, 1, 2)))
static inline void log_warn(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(FEMTOLOG_WARN, fmt, args);
    va_end(args);
}
#else
#define log_warn(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_ERROR
/**
 * Emit a log message at the level \ref FEMTOLOG_ERROR.
 *
 * \note This function is not available if \ref FEMTOLOG_NO_MACROS is
 *       defined.
 *
 * \note This function is a no-op if \ref FEMTOLOG_MIN_LEVEL is set to a
 *       higher level than \ref FEMTOLOG_ERROR.
 *
 * \param fmt A printf-style format string.
 * \param ... Additional arguments, interpreted according to the format
 *            string.
 */
__attribute__((format(printf, 1, 2)))
static inline void log_error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(FEMTOLOG_ERROR, fmt, args);
    va_end(args);
}
#else
#define log_error(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_FATAL
/**
 * Emit a log message at the level \ref FEMTOLOG_FATAL.
 *
 * \note This function is not available if \ref FEMTOLOG_NO_MACROS is
 *       defined.
 *
 * \note This function is a no-op if \ref FEMTOLOG_MIN_LEVEL is set to a
 *       higher level than \ref FEMTOLOG_FATAL.
 *
 * \param fmt A printf-style format string.
 * \param ... Additional arguments, interpreted according to the format
 *            string.
 */
__attribute__((format(printf, 1, 2)))
static inline void log_fatal(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(FEMTOLOG_FATAL, fmt, args);
    va_end(args);
}
#else
#define log_fatal(...)
#endif

/**
 * Emit a log message at a dynamic level.
 *
 * \note This function is not available if \ref FEMTOLOG_NO_MACROS is
 *       defined.
 *
 * \note Calls to this function are always retained during compilation,
 *       regardless of the value of \ref FEMTOLOG_MIN_LEVEL.
 *
 * \param level     The message log level.
 * \param fmt       A printf-style format string.
 * \param ...       Additional arguments, interpreted according to the
 *                  format string.
 */
__attribute__((format(printf, 2, 3)))
static inline void log_dynamic(int level, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(level, fmt, args);
    va_end(args);
}
#endif

#ifndef FEMTOLOG_FREESTANDING
#include <stdlib.h>
#ifndef FEMTOLOG_NO_MACROS
#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_FATAL
/**
 * Emit a log message at the level \ref FEMTOLOG_FATAL and exit the
 * program with an error condition of 1.
 *
 * \note This function is not available if \ref FEMTOLOG_NO_MACROS is
 *       defined or if \ref FEMTOLOG_FREESTANDING is defined.
 *
 * \note This function does not emit a log message if \ref
 *       FEMTOLOG_MIN_LEVEL is set to a higher level than \ref
 *       FEMTOLOG_FATAL. In this case the program will still exit.
 *
 * \param fmt A printf-style format string.
 * \param ... Additional arguments, interpreted according to the format
 *            string.
 */
__attribute__((format(printf, 1, 2)))
static inline void log_fatal_exit(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(FEMTOLOG_FATAL, fmt, args);
    va_end(args);

    exit(1);
}
#else
#define log_fatal_exit(...) exit(1)
#endif
#endif /* FEMTOLOG_NO_MACROS */
#endif /* FEMTOLOG_FREESTANDING */

/**@}*/

/**
 * \defgroup config Configuration macros
 * @{
 */

#ifdef DOXYGEN

/**
 * Disable definition of log_*() functions by femtolog.
 *
 * This macro may be used if the log_*() function names clash with some
 * other library used by a program. If this macro is defined, log
 * messages must instead be emitted by calling femtolog_log() or
 * femtolog_vlog().
 */
#define FEMTOLOG_NO_MACROS

/**
 * Build femtolog for a freestanding environment.
 *
 * A freestanding environment is one where no runtime library is
 * provided. Defining this macro allows femtolog to be used in OS
 * development, bootloaders, UEFI applications or similar environments.
 */
#define FEMTOLOG_FREESTANDING

/**
 * \def FEMTOLOG_MIN_LEVEL
 * Set the minimum level for which log messages will be retained during
 * compilation.
 *
 * All log_*() function calls with a log level lower than the given
 * minimum will be discarded during compilation. This can be useful when
 * using femtolog in memory constrained environments or time sensitive
 * applications.
 *
 * If this macro is not defined by the program using femtolog, it will
 * default to \ref FEMTOLOG_INFO.
 */

#endif

/**@}*/
