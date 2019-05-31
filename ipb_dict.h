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
#define NO_ERROR        (uint8_t)0x00
/** Operation not supported */
#define NOT_SUPPORTED   (uint8_t)0x01
/** Write error */
#define WRITE_ERROR     (uint8_t)0x02

typedef struct TIpbDictEntry
{
    /** Dictionary entry key value */
    uint16_t u16Key;
    /** Read function callback */
    uint8_t (*IpbRead)(uint16_t* u16Data, uint16_t* u16Size);
    /** Write function callback */
    uint8_t (*IpbWrite)(uint16_t* u16Data, uint16_t* u16Size);
} TIpbDictEntry;

typedef struct
{
    /** Dictionary node */
    uint16_t u16Node;
    /** Dictionary struct pointer */
    TIpbDictEntry* pIpbDict;
    /** Dictionary struct number of entries */
    uint16_t* pu16DictCnt;
} TIpbDictInst;

/**
 * Init the Ipb dictionary instance
 *
 * @param[in] ptIpbDictInst
 *  Ipb dictionary instance pointer
 * @param[out] u16NodeInst
 *  Ipb node instance
 */
void
Ipb_DictInit(TIpbDictInst* ptIpbDictInst, uint16_t u16DictNodeInst);

/**
 * Function to read the value of a Ipb register
 *
 * @param[in] ptIpbDictInst
 *  Ipb dictionary instance pointer
 * @param[out] pIpbMsg
 *  Message buffer to store the read register
 *
 * @retval result of the access
 */
uint8_t
Ipb_DictRead(TIpbDictInst* ptIpbDictInst, Ipb_TMsg* pIpbMsg);

/**
 * Function to write the value of a Ipb register
 *
 * @param[in] ptIpbDictInst
 *  Ipb dictionary instance pointer
 * @param[in] pIpbMsg
 *  Message buffer containing register value
 *
 * @retval result of the access
 */
uint8_t
Ipb_DictWrite(TIpbDictInst* ptIpbDictInst, Ipb_TMsg* pIpbMsg);

#endif /* IPB_DICT_H */
