/*
 * femtolog
 * Copyright (c) 2020 rxi
 * Copyright (c) 2021-2022 SanCloud Ltd
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    FEMTOLOG_TRACE,
    FEMTOLOG_DEBUG,
    FEMTOLOG_INFO,
    FEMTOLOG_WARN,
    FEMTOLOG_ERROR,
    FEMTOLOG_FATAL,
    FEMTOLOG_UNKNOWN = -1
};

void femtolog_init(FILE *dest, int level);
int femtolog_level_is_valid(int level);
const char *femtolog_level_to_name(int level);
int femtolog_level_from_name(const char *name);
void femtolog_set_level(int level);
int femtolog_get_level();
void femtolog_set_dest(FILE *dest);
FILE *femtolog_get_dest();
void femtolog_vlog(int level, const char *fmt, va_list args);
void femtolog_log(int level, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

#ifndef FEMTOLOG_NO_MACROS
#define log_trace(...) femtolog_log(FEMTOLOG_TRACE, __VA_ARGS__)
#define log_debug(...) femtolog_log(FEMTOLOG_DEBUG, __VA_ARGS__)
#define log_info(...) femtolog_log(FEMTOLOG_INFO, __VA_ARGS__)
#define log_warn(...) femtolog_log(FEMTOLOG_WARN, __VA_ARGS__)
#define log_error(...) femtolog_log(FEMTOLOG_ERROR, __VA_ARGS__)
#define log_fatal(...) femtolog_log(FEMTOLOG_FATAL, __VA_ARGS__)
#define log_fatal_exit(...) do { femtolog_log(FEMTOLOG_FATAL, __VA_ARGS__); exit(1); } while (0)
#define log_dynamic(level, ...) femtolog_log(level, __VA_ARGS__)
#endif
