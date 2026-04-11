/*
 * Copyright (c) 2024 Contributors as noted in the AUTHORS file
 *
 * This file is part of Solo5, a sandboxed execution environment.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "bindings.h"
#include "ahv_abi.h"

static uint64_t time_base;
static uint64_t tsc_base;
static uint8_t tsc_shift;
static uint32_t tsc_mult;

int tscclock_init(uint64_t tsc_freq)
{
    tsc_shift = 32;
    uint64_t tmp;
    do {
        tmp = (NSEC_PER_SEC << tsc_shift) / tsc_freq;
        if ((tmp & 0xFFFFFFFF00000000L) == 0L)
            tsc_mult = (uint32_t)tmp;
        else
            tsc_shift--;
    } while (tsc_shift > 0 && tsc_mult == 0L);
    assert(tsc_mult != 0L);
    log(DEBUG, "Solo5: ahv_tscclock_init(): tsc_freq=%llu tsc_mult=%u tsc_shift=%u\n",
        (unsigned long long)tsc_freq, tsc_mult, tsc_shift);

    tsc_base = cpu_cntvct();
    time_base = mul64_32(tsc_base, tsc_mult, tsc_shift);

    return 0;
}

uint64_t tscclock_monotonic(void)
{
    uint64_t tsc_now, tsc_delta;

    tsc_now = cpu_cntvct();
    tsc_delta = tsc_now - tsc_base;
    time_base += mul64_32(tsc_delta, tsc_mult, tsc_shift);
    tsc_base = tsc_now;

    return time_base;
}

static uint64_t cpu_cycle_freq;

void time_init(const struct ahv_boot_info *bi)
{
    cpu_cycle_freq = bi->cpu_cycle_freq;
    assert(tscclock_init(cpu_cycle_freq) == 0);
}

uint64_t solo5_clock_monotonic(void)
{
    return tscclock_monotonic();
}

uint64_t solo5_clock_wall(void)
{
    struct ahv_hc_walltime t;
    ahv_do_hypercall(AHV_HYPERCALL_WALLTIME, &t);
    return t.nsecs;
}