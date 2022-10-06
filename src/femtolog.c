/*
 * femtolog
 * Copyright (c) 2020 rxi
 * Copyright (c) 2021-2022 SanCloud Ltd
 * SPDX-License-Identifier: MIT
 */

#include <string.h>

#include "femtolog.h"

struct log_state {
    int level;
    FILE *dest;
};

static struct log_state L;

static const char *level_names[] = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

void femtolog_init(FILE *dest, int level)
{
    femtolog_set_dest(dest);
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

void femtolog_set_dest(FILE *dest)
{
    L.dest = dest;
}

FILE *femtolog_get_dest()
{
    return L.dest;
}

void femtolog_vlog(int level, const char *fmt, va_list args)
{
    if (level < L.level) {
        return;
    }

    fputs(femtolog_level_to_name(level), L.dest);
    fputs(": ", L.dest);
    vfprintf(L.dest, fmt, args);
    fputc('\n', L.dest);
    fflush(L.dest);
}

void femtolog_log(int level, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    femtolog_vlog(level, fmt, args);
    va_end(args);
}
