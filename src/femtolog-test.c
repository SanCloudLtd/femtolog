/*
 * femtolog
 * Copyright (c) 2021-2022 SanCloud Ltd
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include "femtolog.h"

void printline(const char *prefix, const char *fmt, va_list args)
{
    fprintf(stderr, "%s: ", prefix);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    fflush(stderr);
}

int main()
{
    femtolog_init(FEMTOLOG_TRACE, printline);

    log_trace("t0");
    log_debug("t1");
    log_info("t2");
    log_warn("t3");
    log_error("t4");
    log_fatal("t5");
    return 0;
}
