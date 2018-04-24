/**
 * @file ipb_usr.h
 * @brief This file contains functions to be implemented by user to
 *        migrate platform dependencies
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#ifndef IPB_USR_H
#define IPB_USR_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Gets the number of milliseconds since system was started
 *
 * @retval milliseconds
 */
uint32_t
Ipb_GetMillis(void);

/**
 * UART reception
 *
 * @note Non Blocking function
 *
 * @param[in] pu16Buf
 *  Pointer to buffer to be recevied
 * @param[in] u16Size
 *  Size to receive bytes
 *
 * @retval number of read bytes
 */
uint16_t
Ipb_IntfUartReception(uint8_t *pu8Buf, uint16_t u16Size);

/**
 * UART transmission
 *
 * @note Non Blocking function
 *
 * @param[in] pu16Buf
 *  Pointer to buffer to be transmited
 * @param[in] u16Size
 *  Size to transmit bytes
 *
 * @retval number of read bytes
 */
uint16_t
Ipb_IntfUartTransmission(const uint8_t *pu8Buf, uint16_t u16Size);

#endif /* IPB_USR_H */
