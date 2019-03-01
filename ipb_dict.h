/**
 * @file ipb_dict.h
 * @brief Header file for the COCO dictionary.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2019. All rights reserved.
 */

#ifndef IPB_DICT_H
#define IPB_DICT_H

#include <stdint.h>
#include "ipb.h"

/** No error */
#define NO_ERROR        (uint8_t)0x00
/** Operation not supported */
#define NOT_SUPPORTED   (uint8_t)0x01
/** Write error */
#define WRITE_ERROR     (uint8_t)0x02

/**
 * Function to read the value of a Ipb register
 *
 * @param[out] pIpbMsg
 *  Message buffer to store the read register
 *
 * @retval result of the access
 */
uint8_t
Ipb_DictRead(Ipb_TMsg* pIpbMsg);

/**
 * Function to write the value of a Ipb register
 *
 * @param[in] pIpbMsg
 *  Message buffer containing register value
 *
 * @retval result of the access
 */
uint8_t
Ipb_DictWrite(Ipb_TMsg* pIpbMsg);

#endif /* IPB_DICT_H */
