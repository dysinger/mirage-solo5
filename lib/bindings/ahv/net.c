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

static const struct mft *mft;

void net_init(const struct ahv_boot_info *bi)
{
    mft = bi->mft;
}

solo5_result_t solo5_net_acquire(const char *name, solo5_handle_t *handle,
        struct solo5_net_info *info)
{
    unsigned index;
    const struct mft_entry *e =
        mft_get_by_name(mft, name, MFT_DEV_NET_BASIC, &index);
    if (e == NULL)
        return SOLO5_R_EINVAL;
    assert(e->attached);

    *handle = index;
    info->mtu = e->u.net_basic.mtu;
    memcpy(info->mac_address, e->u.net_basic.mac,
            sizeof info->mac_address);
    return SOLO5_R_OK;
}

static solo5_result_t net_write(solo5_handle_t handle,
        const uint8_t *buf, size_t size)
{
    struct ahv_hc_net_write hc;
    hc.handle = handle;
    hc.data = (AHV_GUEST_PTR(const void *))buf;
    hc.len = size;
    hc.ret = -1;

    ahv_do_hypercall(AHV_HYPERCALL_NET_WRITE, &hc);
    return hc.ret;
}

static solo5_result_t net_read(solo5_handle_t handle,
        uint8_t *buf, size_t size, size_t *read_size)
{
    struct ahv_hc_net_read hc;
    hc.handle = handle;
    hc.data = (AHV_GUEST_PTR(void *))buf;
    hc.len = size;
    hc.ret = -1;

    ahv_do_hypercall(AHV_HYPERCALL_NET_READ, &hc);
    if (hc.ret == SOLO5_R_OK) {
        *read_size = hc.len;
    }
    return hc.ret;
}

solo5_result_t solo5_net_write(solo5_handle_t handle, const uint8_t *buf,
        size_t size)
{
    const struct mft_entry *e =
        mft_get_by_index(mft, handle, MFT_DEV_NET_BASIC);
    if (e == NULL)
        return SOLO5_R_EINVAL;

    return net_write(handle, buf, size);
}

solo5_result_t solo5_net_read(solo5_handle_t handle, uint8_t *buf,
        size_t size, size_t *read_size)
{
    const struct mft_entry *e =
        mft_get_by_index(mft, handle, MFT_DEV_NET_BASIC);
    if (e == NULL)
        return SOLO5_R_EINVAL;

    return net_read(handle, buf, size, read_size);
}

void solo5_yield(solo5_time_t deadline, solo5_handle_set_t *ready_set)
{
    struct ahv_hc_poll hc;
    hc.timeout_nsecs = deadline;
    hc.ready_set = 0;
    hc.ret = -1;

    ahv_do_hypercall(AHV_HYPERCALL_POLL, &hc);

    if (ready_set != NULL)
        *ready_set = hc.ready_set;
}