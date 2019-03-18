/**
 * @file dictionary.c
 * @brief This file contains the dictionary access functions.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#include "ipb_dict.h"
#include "ipb_dict_usr.h"

uint8_t Ipb_DictRead(TIpbDictEntry* ptIpbDict, uint16_t u16DictSz, Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NO_ERROR;

    uint16_t u16Idx;
    for (u16Idx = 0; u16Idx < u16DictSz; ++u16Idx)
    {
        if (pIpbMsg->u16Addr == ptIpbDict[u16Idx].u16Key)
        {
            if (ptIpbDict[u16Idx].IpbRead != NULL)
            {
                u8Ret = ptIpbDict[u16Idx].IpbRead(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            }

            break;
        }
    }

    if (u16Idx == u16DictSz)
    {
        u8Ret = NOT_SUPPORTED;
    }

    return u8Ret;
}

uint8_t Ipb_DictWrite(TIpbDictEntry* ptIpbDict, uint16_t u16DictSz, Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NO_ERROR;

    uint16_t u16Idx;
    for (u16Idx = 0; u16Idx < u16DictSz; ++u16Idx)
    {
        if (pIpbMsg->u16Addr == ptIpbDict[u16Idx].u16Key)
        {
            if (ptIpbDict[u16Idx].IpbWrite != NULL)
            {
                u8Ret = ptIpbDict[u16Idx].IpbWrite(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            }

            break;
        }
    }

    if (u16Idx == u16DictSz)
    {
        u8Ret = NOT_SUPPORTED;
    }

    return u8Ret;
}
