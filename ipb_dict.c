/**
 * @file ipb_dict.c
 * @brief This file contains the dictionary access functions.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2019. All rights reserved.
 */

#include "ipb_dict.h"

/** User dictionary definitions */
#define IPB_DICT_DEF
#include "ipb_dict_usr.h"

#include <string.h>

void Ipb_DictInit(TIpbDictInst* ptIpbDictInst, uint16_t u16DictNodeInst)
{
    if (ptIpbDict != NULL)
    {
        for (uint16_t u16DictNodeIdx = (uint16_t)0U;
                u16DictNodeIdx < (sizeof(ptIpbDict) / sizeof(TIpbDictInst));
                ++u16DictNodeIdx)
        {
            if (u16DictNodeIdx == ptIpbDict[u16DictNodeIdx].u16Node)
            {
                memcpy((void*)ptIpbDictInst,
                       (const void*)&ptIpbDict[u16DictNodeIdx], sizeof(TIpbDictInst));
                break;
            }
        }
    }
}

uint8_t Ipb_DictRead(TIpbDictInst* ptIpbDictInst, Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NOT_SUPPORTED;

    uint16_t u16Idx;
    for (u16Idx = (uint16_t)0U; u16Idx < *(ptIpbDictInst->pu16DictCnt); ++u16Idx)
    {
        if (pIpbMsg->u16Addr == ptIpbDictInst->pIpbDict[u16Idx].u16Key)
        {
            if (ptIpbDictInst->pIpbDict[u16Idx].IpbRead != NULL)
            {
                u8Ret = ptIpbDictInst->pIpbDict[u16Idx].IpbRead(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            }

            break;
        }
    }

    return u8Ret;
}

uint8_t Ipb_DictWrite(TIpbDictInst* ptIpbDictInst, Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NOT_SUPPORTED;

    uint16_t u16Idx;
    for (u16Idx = (uint16_t)0U; u16Idx < *(ptIpbDictInst->pu16DictCnt); ++u16Idx)
    {
        if (pIpbMsg->u16Addr == ptIpbDictInst->pIpbDict[u16Idx].u16Key)
        {
            if (ptIpbDictInst->pIpbDict[u16Idx].IpbWrite != NULL)
            {
                u8Ret = ptIpbDictInst->pIpbDict[u16Idx].IpbWrite(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            }

            break;
        }
    }

    return u8Ret;
}
