/**
 * @file dictionary.c
 * @brief This file contains the COCO dictionary management.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#include "ipb_dict.h"
#include "coco_dictionary_registers.h"
#include "ipb_dict_usr.h"


uint8_t Ipb_DictRead(Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NO_ERROR;

    uint16_t u16Idx;
    for (u16Idx = 0; u16Idx < sizeof(ptCocoDict); ++u16Idx)
    {
        if (pIpbMsg->u16Addr == ptCocoDict[u16Idx].u16Key)
        {
            if (ptCocoDict[u16Idx].CocoRead != NULL)
            {
                u8Ret = ptCocoDict[u16Idx].CocoRead(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            }

            break;
        }
    }

    if (u16Idx == sizeof(ptCocoDict))
    {
        u8Ret = NOT_SUPPORTED;
    }

    return u8Ret;
}

uint8_t Ipb_DictWrite(Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NO_ERROR;

    uint16_t u16Idx;
    for (u16Idx = 0; u16Idx < sizeof(ptCocoDict); ++u16Idx)
    {
        if (pIpbMsg->u16Addr == ptCocoDict[u16Idx].u16Key)
        {
            if (ptCocoDict[u16Idx].CocoWrite != NULL)
            {
                u8Ret = ptCocoDict[u16Idx].CocoWrite(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            }

            break;
        }
    }

    if (u16Idx == sizeof(ptCocoDict))
    {
        u8Ret = NOT_SUPPORTED;
    }

    return u8Ret;
}
