/*
 * femtolog
 * Copyright (c) 2021-2022 SanCloud Ltd
 * SPDX-License-Identifier: MIT
 */

#define FEMTOLOG_MIN_LEVEL FEMTOLOG_TRACE

#include <stdio.h>
#include "femtolog.h"

void output(const char *prefix, const char *fmt, va_list args)
{
    fprintf(stderr, "%s: ", prefix);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    fflush(stderr);
}

int main()
{
    femtolog_init(FEMTOLOG_INFO, output);

    log_trace("t%d", 0);
    log_debug("t%d", 1);
    log_info("t%d", 2);
    log_warn("t%d", 3);
    log_error("t%d", 4);
    log_fatal("t%d", 5);
    return 0;
}
