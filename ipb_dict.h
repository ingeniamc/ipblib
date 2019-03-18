/**
 * @file ipb_dict.h
 * @brief Header file for the Ipb dictionary.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2019. All rights reserved.
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

typedef struct TIpbDictEntry
{
    /** Dictionary entry key value */
    uint16_t u16Key;
    /** Read function callback */
    uint8_t (*IpbRead)(uint16_t* u16Data, uint16_t* u16Size);
    /** Write function callback */
    uint8_t (*IpbWrite)(uint16_t* u16Data, uint16_t* u16Size);
} TIpbDictEntry;

/**
 * Function to read the value of a Ipb register
 *
 * @param[in] ptIpbDict
 *  Ipb dictionary pointer
 * @param[in] u16DictSz
 *  Size in number of elements of the dictionary
 * @param[out] pIpbMsg
 *  Message buffer to store the read register
 *
 * @retval result of the access
 */
uint8_t
Ipb_DictRead(TIpbDictEntry* ptIpbDict, uint16_t u16DictSz, Ipb_TMsg* pIpbMsg);

/**
 * Function to write the value of a Ipb register
 *
 * @param[in] ptIpbDict
 *  Ipb dictionary pointer
 * @param[in] u16DictSz
 *  Size in number of elements of the dictionary
 * @param[in] pIpbMsg
 *  Message buffer containing register value
 *
 * @retval result of the access
 */
uint8_t
Ipb_DictWrite(TIpbDictEntry* ptIpbDict, uint16_t u16DictSz, Ipb_TMsg* pIpbMsg);

#endif /* IPB_DICT_H */
