/*
 * femtolog
 * Copyright (c) 2021-2022 SanCloud Ltd
 * Based on log.c, Copyright (c) 2020 rxi
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdarg.h>

enum {
    FEMTOLOG_TRACE,
    FEMTOLOG_DEBUG,
    FEMTOLOG_INFO,
    FEMTOLOG_WARN,
    FEMTOLOG_ERROR,
    FEMTOLOG_FATAL,
    FEMTOLOG_UNKNOWN = -1
};

typedef void (*femtolog_printline_fn)(const char *, const char *, va_list);

void femtolog_init(int level, femtolog_printline_fn printline);
int femtolog_level_is_valid(int level);
const char *femtolog_level_to_name(int level);
int femtolog_level_from_name(const char *name);
void femtolog_set_level(int level);
int femtolog_get_level();
void femtolog_set_printline_fn(femtolog_printline_fn printline);
femtolog_printline_fn femtolog_get_printline_fn();
void femtolog_vlog(int level, const char *fmt, va_list args);
void femtolog_log(int level, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

#ifndef FEMTOLOG_NO_MACROS
#ifndef FEMTOLOG_MIN_LEVEL
#define FEMTOLOG_MIN_LEVEL 0
#endif

#if (FEMTOLOG_MIN_LEVEL < FEMTOLOG_TRACE) || (FEMTOLOG_MIN_LEVEL > FEMTOLOG_FATAL)
#error FEMTOLOG_MIN_LEVEL is invalid
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_TRACE
#define log_trace(...) femtolog_log(FEMTOLOG_TRACE, __VA_ARGS__)
#else
#define log_trace(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_DEBUG
#define log_debug(...) femtolog_log(FEMTOLOG_DEBUG, __VA_ARGS__)
#else
#define log_debug(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_INFO
#define log_info(...) femtolog_log(FEMTOLOG_INFO, __VA_ARGS__)
#else
#define log_info(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_WARN
#define log_warn(...) femtolog_log(FEMTOLOG_WARN, __VA_ARGS__)
#else
#define log_warn(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_ERROR
#define log_error(...) femtolog_log(FEMTOLOG_ERROR, __VA_ARGS__)
#else
#define log_error(...)
#endif

#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_FATAL
#define log_fatal(...) femtolog_log(FEMTOLOG_FATAL, __VA_ARGS__)
#else
#define log_fatal(...)
#endif

#define log_dynamic(level, ...) femtolog_log(level, __VA_ARGS__)
#endif

#ifndef FEMTOLOG_FREESTANDING
#include <stdlib.h>
#ifndef FEMTOLOG_NO_MACROS
#if FEMTOLOG_MIN_LEVEL <= FEMTOLOG_FATAL
#define log_fatal_exit(...) do { femtolog_log(FEMTOLOG_FATAL, __VA_ARGS__); exit(1); } while (0)
#else
#define log_fatal_exit(...) exit(1)
#endif
#endif /* FEMTOLOG_NO_MACROS */
#endif /* FEMTOLOG_FREESTANDING */
