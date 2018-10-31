/**
 * @file ipb.c
 * @brief This file contains API functions of the
 *        ingenia protocol bus (IPB)
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#include "ipb.h"
#include <stdint.h>
#include <string.h>

void Ipb_Init(Ipb_TInst* ptInst, Ipb_EIntf eIntf, Ipb_EMode eMode)
{
    ptInst->eIntf = eIntf;
    ptInst->isCyclic = false;
    ptInst->eMode = eMode;

    Ipb_IntfInit(&ptInst->tIntf, eIntf, 0);
}

void Ipb_Deinit(Ipb_TInst* ptInst)
{
    ptInst->eIntf = UART_BASED;
    ptInst->isCyclic = false;
    ptInst->eMode = IPB_BLOCKING;
    Ipb_IntfDeinit(&ptInst->tIntf);
}

Ipb_EStatus Ipb_Write(Ipb_TInst* ptInst, Ipb_TMsg* ptMsg, uint32_t u32Timeout)
{
    ptMsg->eStatus = IPB_ERROR;

    if (ptInst->isCyclic == false)
    {
        if (ptInst->eMode == IPB_BLOCKING)
        {
            uint32_t u32Millis = Ipb_GetMillis();
            do
            {
                ptMsg->eStatus = ptInst->tIntf.Write(&ptInst->tIntf, &ptMsg->u16SubNode, &ptMsg->u16Addr,
                                                     &ptMsg->u16Cmd, ptMsg->pu16Data, ptMsg->u16Size);

            } while ((ptMsg->eStatus != IPB_ERROR) && (ptMsg->eStatus != IPB_SUCCESS)
                    && ((Ipb_GetMillis() - u32Millis) < u32Timeout));
        }
        else
        {
            /** No blocking mode */
            ptMsg->eStatus = ptInst->tIntf.Write(&ptInst->tIntf, &ptMsg->u16SubNode, &ptMsg->u16Addr,
                                                 &ptMsg->u16Cmd, ptMsg->pu16Data, ptMsg->u16Size);
        }
    }
    else
    {
        /* Cyclic mode */
    }

    return ptMsg->eStatus;
}

Ipb_EStatus Ipb_Read(Ipb_TInst* ptInst, Ipb_TMsg* ptMsg, uint32_t u32Timeout)
{
    ptMsg->eStatus = IPB_ERROR;

    if (ptInst->isCyclic == false)
    {
        if (ptInst->eMode == IPB_BLOCKING)
        {
            uint32_t u32Millis = Ipb_GetMillis();
            do
            {
                ptMsg->eStatus = ptInst->tIntf.Read(&ptInst->tIntf, &ptMsg->u16SubNode, &ptMsg->u16Addr,
                                                    &ptMsg->u16Cmd, ptMsg->pu16Data, &ptMsg->u16Size);

            } while ((ptMsg->eStatus != IPB_ERROR) && (ptMsg->eStatus != IPB_SUCCESS)
                    && ((Ipb_GetMillis() - u32Millis) < u32Timeout));
        }
        else
        {
            /** No blocking mode */
            ptMsg->eStatus = ptInst->tIntf.Read(&ptInst->tIntf, &ptMsg->u16SubNode, &ptMsg->u16Addr,
                                                &ptMsg->u16Cmd, ptMsg->pu16Data, &ptMsg->u16Size);
        }
    }
    else
    {
        /* Cyclic mode */
    }

    return ptMsg->eStatus;
}

