/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-24     Meco Man     first version
 */

#include <rtthread.h>
#include "ConsoleSerial.h"

void ConsoleSerial::begin(uint32_t baud)
{
    begin(baud, SERIAL_8N1);
}

void ConsoleSerial::begin(uint32_t baud, uint8_t config)
{
    /* Nothing need to do */
}

/*Code to display letter when given the ASCII code for it*/
size_t ConsoleSerial::write(uint8_t c)
{
    rt_kprintf("%c", c);
    return 1;
}

/*Code to display array of chars when given a pointer to the beginning of the array and a size
    -- this will not end with the null character*/
size_t ConsoleSerial::write(const uint8_t *buffer, size_t size)
{
    return rt_device_write(rt_console_get_device(), 0, buffer, size);
}

ConsoleSerial Serial;