/*
 * femtolog
 * Copyright (c) 2021-2022 SanCloud Ltd
 * Based on log.c, Copyright (c) 2020 rxi
 * Freestanding strcmp implementation copyright © 2005-2020 Rich Felker, et al.
 * SPDX-License-Identifier: MIT
 */

/* Retain all internal log messages. */
#define FEMTOLOG_MIN_LEVEL FEMTOLOG_TRACE

#include "femtolog.h"

#ifdef FEMTOLOG_FREESTANDING
static inline int strcmp(const char *l, const char *r)
{
	for (; *l==*r && *l; l++, r++);
	return *(unsigned char *)l - *(unsigned char *)r;
}
#else
#include <string.h>
#endif

struct log_state {
    int level;
    femtolog_output_fn output_fn;
};

static struct log_state L = { FEMTOLOG_UNKNOWN, (femtolog_output_fn)NULL };

static const char *level_names[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

void femtolog_init(int level, femtolog_output_fn output_fn)
{
    femtolog_set_level(level);
    femtolog_set_output_fn(output_fn);
    log_trace("femtolog v%s", FEMTOLOG_VERSION);
}

bool femtolog_level_is_valid(int level)
{
    if ((level >= 0) && (level <= FEMTOLOG_FATAL))
        return true;
    else
        return false;
}

const char *femtolog_level_to_name(int level)
{
    if (femtolog_level_is_valid(level))
        return level_names[level];
    else
        return "(unknown)";
}

int femtolog_name_to_level(const char *name)
{
    int i;
    for (i = 0; i <= FEMTOLOG_FATAL; i++) {
        if (!strcmp(name, level_names[i]))
            return i;
    }

    return FEMTOLOG_UNKNOWN;
}

void femtolog_set_level(int level)
{
    if (femtolog_level_is_valid(level))
        L.level = level;
}

int femtolog_get_level()
{
    return L.level;
}

void femtolog_set_output_fn(femtolog_output_fn output_fn)
{
    L.output_fn = output_fn;
}

femtolog_output_fn femtolog_get_output_fn()
{
    return L.output_fn;
}

void femtolog_vlog(int level, const char *fmt, va_list args)
{
    if (level < L.level) {
        return;
    }

    if(NULL != L.output_fn)
    {
        L.output_fn(femtolog_level_to_name(level), fmt, args);
    }
}
