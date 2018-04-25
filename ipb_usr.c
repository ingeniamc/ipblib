/**
 * @file ipb_usr.c
 * @brief This file contains functions to be implemented by user to
 *        migrate platform dependencies
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#include "ipb_usr.h"

__attribute__((weak))uint32_t Ipb_GetMillis(void)
{
    /** Return millisecons */
    return 0;
}

__attribute__((weak))uint16_t Ipb_IntfUartReception(uint16_t u16Id, uint8_t *pu8Buf, uint16_t u16Size)
{
    /** Receive data */

    /** Return read bytes */
    return 0;
}

__attribute__((weak))uint16_t Ipb_IntfUartTransmission(uint16_t u16Id, const uint8_t *pu8Buf, uint16_t u16Size)
{
    /** Trasmit data */

    /** Return write bytes */
    return 0;
}
