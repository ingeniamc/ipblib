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
 * @param[in] u16Id
 *  Identification of the IPB instance
 * @param[in] pu8Buf
 *  Pointer to buffer to be recevied
 * @param[in] u16Size
 *  Size to receive bytes
 *
 * @retval number of read bytes
 */
uint16_t
Ipb_IntfUartReception(uint16_t u16Id, uint8_t *pu8Buf, uint16_t u16Size);

/**
 * UART transmission
 *
 * @note Non Blocking function
 *
 * @param[in] u16Id
 *  Identification of the IPB instance
 * @param[in] pu8Buf
 *  Pointer to buffer to be transmited
 * @param[in] u16Size
 *  Size to transmit bytes
 *
 * @retval number of read bytes
 */
uint16_t
Ipb_IntfUartTransmission(uint16_t u16Id, const uint8_t *pu8Buf, uint16_t u16Size);

/**
 * Discard accumulated data of the uart buffer
 *
 * @param[in] u16Id
 *  Identification of the IPB instance
 */
void
Ipb_IntfUartDiscardData(uint16_t u16Id);

/**
 * USB reception
 *
 * @note Non Blocking function
 *
 * @param[in] u16Id
 *  Identification of the IPB instance
 * @param[in] pu8Buf
 *  Pointer to buffer to be recevied
 * @param[in] u16Size
 *  Size to receive bytes
 *
 * @retval number of read bytes
 * @note Function called when reception errors occurs,
 *  the buffered data need to be removed.
 *
 */
uint16_t
Ipb_IntfUsbReception(uint16_t u16Id, uint8_t *pu8Buf, uint16_t u16Size);

/**
 * USB transmission
 *
 * @note Non Blocking function
 *
 * @param[in] u16Id
 *  Identification of the IPB instance
 * @param[in] pu8Buf
 *  Pointer to buffer to be transmited
 * @param[in] u16Size
 *  Size to transmit bytes
 *
 * @retval number of read bytes
 */
uint16_t
Ipb_IntfUsbTransmission(uint16_t u16Id, const uint8_t *pu8Buf, uint16_t u16Size);

#endif /* IPB_USR_H */
