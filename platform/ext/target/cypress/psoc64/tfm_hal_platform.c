/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 * Copyright (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <string.h>
#include <stdint.h>

#include "cmsis.h"
#include "cy_pra.h"
#include "region_defs.h"
#include "target_cfg.h"
#include "tfm_hal_platform.h"
#include "uart_stdout.h"

/* FIXME:
 * Instead of TFM-customized mcuboot, at this moment psoc64 uses
 * Cypress version of it - CypressBootloader (CYBL). CYBL doesn't
 * populate BOOT_TFM_SHARED_DATA.
 * As a temp workaround, mock mcuboot shared data to pass
 * initialization checks.
*/
void mock_tfm_shared_data(void)
{
    const uint32_t mock_data[] = {
        0x00D92016,    0x00071103,     0x00455053,     0x30000911,
        0x302E302E,    0x00081102,     0x00000000,     0x00241108,
        0x6C170A97,    0x5645665E,     0xDB6E2BA6,     0xA4FF4D74,
        0xFD34D7DB,    0x67449A82,     0x75FD0930,     0xAA15A9F9,
        0x000A1109,    0x32414853,     0x11013635,     0xE6BF0024,
        0x26886FD8,    0xFB97FFF4,     0xFBE6C496,     0x463E99C4,
        0x5D56FC19,    0x34DF6AA2,     0x9A4829C3,     0x114338DC,
        0x534E0008,    0x11404550,     0x2E300009,     0x42302E30,
        0x00000811,    0x48000000,     0x7E002411,     0x5FD9229A,
        0xE9672A5F,    0x31AAE1EA,     0x8514D772,     0x7F3B26BC,
        0x2C7EF27A,    0x9C6047D2,     0x4937BB9F,     0x53000A11,
        0x35324148,    0x24114136,     0xCA60B300,     0x6B8CC9F5,
        0x82482A94,    0x23489DFA,     0xA966B1EF,     0x4A6E6AEF,
        0x19197CA3,    0xC0CC1FED,     0x00000049,     0x00000000
    };
    uint32_t *boot_data = (uint32_t*)BOOT_TFM_SHARED_DATA_BASE;
    memcpy(boot_data, mock_data, sizeof(mock_data));
}

enum tfm_hal_status_t tfm_hal_platform_init(void)
{
    platform_init();

#if defined(CY_DEVICE_SECURE)
    /* Initialize Protected Register Access driver. */
    Cy_PRA_Init();
#endif /* defined(CY_DEVICE_SECURE) */

    /* FIXME: Use the actual data from mcuboot */
    mock_tfm_shared_data();

    __enable_irq();
    stdio_init();

    return TFM_HAL_SUCCESS;
}
