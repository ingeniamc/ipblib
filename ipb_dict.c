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

/** Max number of dictionaries */
#define MAX_NODES       (uint16_t)4U

#if !defined(DICT_IDX_0_NODE) || !defined(DICT_IDX_0_DO_POINTER) || !defined(DICT_IDX_0_SIZE_POINTER) \
     || !defined(DICT_IDX_1_NODE) || !defined(DICT_IDX_1_DO_POINTER) || !defined(DICT_IDX_1_SIZE_POINTER) \
     || !defined(DICT_IDX_2_NODE) || !defined(DICT_IDX_2_DO_POINTER) || !defined(DICT_IDX_2_SIZE_POINTER) \
     || !defined(DICT_IDX_3_NODE) || !defined(DICT_IDX_3_DO_POINTER) || !defined(DICT_IDX_3_SIZE_POINTER)

    #error "Ipb dictionaries not defined"

#endif

/** Dictionaries instances array */
TIpbDictInst ptIpbDict[MAX_NODES] =
{
     { DICT_IDX_0_NODE, DICT_IDX_0_DO_POINTER, DICT_IDX_0_SIZE_POINTER },
     { DICT_IDX_1_NODE, DICT_IDX_1_DO_POINTER, DICT_IDX_1_SIZE_POINTER },
     { DICT_IDX_2_NODE, DICT_IDX_2_DO_POINTER, DICT_IDX_2_SIZE_POINTER },
     { DICT_IDX_3_NODE, DICT_IDX_3_DO_POINTER, DICT_IDX_3_SIZE_POINTER }
};

/**
 * Function to search entry by key in an Ipb dictionary
 *
 * @param[in] ptIpbDictInst
 *  Ipb dictionary instance pointer
 * @param[in] u16Key
 *  Ipb entry key
 *
 * @retval Ipb dictionary entry pointer if success, NULL otherwise
 */
static TIpbDictEntry*
SearchByKey(TIpbDictInst* ptIpbDictInst, uint16_t u16Key);

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

    TIpbDictEntry* ptIpbDictEnt = SearchByKey(ptIpbDictInst, pIpbMsg->u16Addr);
    if (ptIpbDictEnt != NULL)
    {
        if (ptIpbDictEnt->IpbRead != NULL)
        {
            u8Ret = ptIpbDictEnt->IpbRead(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
        }
    }

    return u8Ret;
}

uint8_t Ipb_DictWrite(TIpbDictInst* ptIpbDictInst, Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NOT_SUPPORTED;

    TIpbDictEntry* ptIpbDictEnt = SearchByKey(ptIpbDictInst, pIpbMsg->u16Addr);
    if (ptIpbDictEnt != NULL)
    {
        if (ptIpbDictEnt->IpbWrite != NULL)
        {
            u8Ret = ptIpbDictEnt->IpbWrite(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
        }
    }

    return u8Ret;
}

void* Ipb_DictReadPoint(TIpbDictInst* ptIpbDictInst, uint16_t u16Key)
{
    void* pRet = NULL;

    TIpbDictEntry* ptIpbDictEnt = SearchByKey(ptIpbDictInst, u16Key);
    if (ptIpbDictEnt != NULL)
    {
        if (ptIpbDictEnt->IpbReadPoint != NULL)
        {
            pRet = ptIpbDictEnt->IpbReadPoint();
        }
    }

    return pRet;
}

static TIpbDictEntry* SearchByKey(TIpbDictInst* ptIpbDictInst, uint16_t u16Key)
{
    TIpbDictEntry* ptIpbDictEnt = NULL;

    if (ptIpbDictInst != NULL)
    {
        register uint16_t u16Idx;
        for (u16Idx = (uint16_t)0U; u16Idx < *(ptIpbDictInst->pu16DictCnt); ++u16Idx)
        {
            if (u16Key == ptIpbDictInst->pIpbDict[u16Idx].u16Key)
            {
                ptIpbDictEnt = &ptIpbDictInst->pIpbDict[u16Idx];
                break;
            }
        }
    }

    return ptIpbDictEnt;
}

