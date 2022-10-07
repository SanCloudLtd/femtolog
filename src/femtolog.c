/*
 * femtolog
 * Copyright (c) 2020 rxi
 * Copyright (c) 2021-2022 SanCloud Ltd
 * Freestanding strcmp implementation copyright © 2005-2020 Rich Felker, et al.
 * SPDX-License-Identifier: MIT
 */

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
};

static struct log_state L;

static const char *level_names[] = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

void femtolog_init(int level)
{
    femtolog_set_level(level);
    log_trace("femtolog v%s", FEMTOLOG_VERSION);
}

int femtolog_level_is_valid(int level)
{
    if ((level >= 0) && (level <= FEMTOLOG_FATAL))
        return 1;
    else
        return 0;
}

const char *femtolog_level_to_name(int level)
{
    if (femtolog_level_is_valid(level))
        return level_names[level];
    else
        return "(unknown)";
}

int femtolog_level_from_name(const char *name)
{
    int i;
    for (i = 0; i <= FEMTOLOG_FATAL; i++) {
        if (!strcmp(name, level_names[i]))
            return i;
    }

    return -1;
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

void femtolog_vlog(int level, const char *fmt, va_list args)
{
    if (level < L.level) {
        return;
    }

    femtolog_printf("%s: ", femtolog_level_to_name(level));
    femtolog_printf(fmt, args);
    femtolog_printf("\n");
    femtolog_flush();
}

void femtolog_log(int level, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(level, fmt, args);
    va_end(args);
}
