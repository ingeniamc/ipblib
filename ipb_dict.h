/**
 * @file dictionary.h
 * @brief Header file for the COCO dictionary.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#ifndef IPB_DICT_H
#define IPB_DICT_H

#include <stdint.h>
#include "ipb.h"

/** No error */
#define NO_ERROR        (uint8_t)0x00U
/** Operation not supported */
#define NOT_SUPPORTED   (uint8_t)0x01U
/** Write error */
#define WRITE_ERROR     (uint8_t)0x02U

/**
 * Function to read the value of a Ipb register
 */
uint8_t
Ipb_DictRead(Ipb_TMsg* pIpbMsg);

/**
 * Function to write the value of a Ipb register
 */
uint8_t
Ipb_DictWrite(Ipb_TMsg* pIpbMsg);

#endif /* IPB_DICT_H */
