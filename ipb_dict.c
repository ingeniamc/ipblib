/**
 * @file ipb_dict.c
 * @brief This file contains the dictionary access functions.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2019. All rights reserved.
 */

#include "ipb_dict.h"

/** User dictionary definitions */
#include "ipb_dict_usr.h"

#include <string.h>

#define MAX_NODES       (uint16_t)4U

#if !defined(DICT_IDX_0_NODE) || !defined(DICT_IDX_0_DO_POITNER) || !defined(DICT_IDX_0_SIZE) \
     || !defined(DICT_IDX_1_NODE) || !defined(DICT_IDX_1_DO_POITNER) || !defined(DICT_IDX_1_SIZE) \
     || !defined(DICT_IDX_2_NODE) || !defined(DICT_IDX_2_DO_POITNER) || !defined(DICT_IDX_2_SIZE) \
     || !defined(DICT_IDX_3_NODE) || !defined(DICT_IDX_3_DO_POITNER) || !defined(DICT_IDX_3_SIZE)

    #error "Ipb dictionaries not defined"

#endif

TIpbDictInst ptIpbDict[MAX_NODES] =
{
     { DICT_IDX_0_NODE, DICT_IDX_0_DO_POITNER, DICT_IDX_0_SIZE },
     { DICT_IDX_1_NODE, DICT_IDX_1_DO_POITNER, DICT_IDX_1_SIZE },
     { DICT_IDX_2_NODE, DICT_IDX_2_DO_POITNER, DICT_IDX_2_SIZE },
     { DICT_IDX_3_NODE, DICT_IDX_3_DO_POITNER, DICT_IDX_3_SIZE }
};

void Ipb_DictInit(TIpbDictInst* ptIpbDictInst, int16_t i16DictNodeInst)
{
    if (ptIpbDict != NULL)
    {
        for (uint16_t u16DictIdx = (uint16_t)0U;
                u16DictIdx < MAX_NODES;
                ++u16DictIdx)
        {
            if ((ptIpbDict[u16DictIdx].i16Node >= (int16_t)0)
                && (i16DictNodeInst == ptIpbDict[u16DictIdx].i16Node))
            {
                memcpy((void*)ptIpbDictInst,
                       (const void*)&ptIpbDict[u16DictIdx], sizeof(TIpbDictInst));
                break;
            }
        }
    }
}

uint8_t Ipb_DictRead(TIpbDictInst* ptIpbDictInst, Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NOT_SUPPORTED;

    if (ptIpbDictInst != NULL)
    {
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
    }

    return u8Ret;
}

uint8_t Ipb_DictWrite(TIpbDictInst* ptIpbDictInst, Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NOT_SUPPORTED;

    if (ptIpbDictInst != NULL)
    {
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
    }

    return u8Ret;
}
