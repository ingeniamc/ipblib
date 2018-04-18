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
#include <stdio.h>
#include <string.h>

void IpbInit(IpbInst* ptInst, EIpbIntf eIntf, EIpbMode eMode)
{
    ptInst->eIntf = eIntf;
    ptInst->isCyclic = false;
    ptInst->eMode = eMode;

    IpbIntfInit(&ptInst->tIntf, eIntf);
}

void IpbDeinit(IpbInst* ptInst)
{
    ptInst->eIntf = UART_BASED;
    ptInst->isCyclic = false;
    ptInst->eMode = IPB_BLOCKING;
    IpbIntfDeinit(&ptInst->tIntf);
}

EIpbReqStatus IpbWrite(IpbInst* ptInst, IpbMsg* mcbMsg, uint32_t u32Timeout)
{
    EIpbReqStatus eResult = IPB_MESSAGE_ERROR;
    EIpbStatus eStatus;
    size_t sz;

    if (ptInst->isCyclic == false)
    {
        if (ptInst->eMode == IPB_BLOCKING)
        {
            uint32_t u32Millis = HAL_GetTick();
            do
            {
                eStatus = ptInst->tIntf.Write(&ptInst->tIntf, &mcbMsg->u16Node,
                                            &mcbMsg->u16SubNode,
                                            &mcbMsg->u16Addr, &mcbMsg->u16Cmd,
                                            &mcbMsg->u16Data[0], &mcbMsg->u16Size);

            } while ((eStatus != IPB_ERROR) && (eStatus != IPB_SUCCESS)
                    && ((HAL_GetTick() - u32Millis) < u32Timeout));
        }
        else
        {
            /** No blocking mode */
            eStatus = ptInst->tIntf.Write(&ptInst->tIntf, &mcbMsg->u16Node,
                                        &mcbMsg->u16SubNode, &mcbMsg->u16Addr,
                    &mcbMsg->u16Cmd, &mcbMsg->u16Data[0], &sz);
        }
        if (eStatus == IPB_SUCCESS)
        {
            eResult = IPB_MESSAGE_SUCCESS;
        }
        else
        {
            eResult = IPB_MESSAGE_ERROR;
        }
    }
    else
    {
        /* Cyclic mode */
    }

    return eResult;
}

EIpbReqStatus IpbRead(IpbInst* ptInst, IpbMsg* mcbMsg, uint32_t u32Timeout)
{
    EIpbReqStatus eResult = 0;
    EIpbStatus eStatus = IPB_ERROR;

    if (ptInst->isCyclic == false)
    {
        if (ptInst->eMode == IPB_BLOCKING)
        {
            uint32_t u32Millis = HAL_GetTick();
            do
            {
                eStatus = ptInst->tIntf.Read(&ptInst->tIntf, &mcbMsg->u16Node,
                                           &mcbMsg->u16SubNode,
                                           &mcbMsg->u16Addr, &mcbMsg->u16Cmd,
                                           &mcbMsg->u16Data[0]);

            } while ((eStatus != IPB_ERROR) && (eStatus != IPB_SUCCESS)
                    && ((HAL_GetTick() - u32Millis) < u32Timeout));
            mcbMsg->u16Size = ptInst->tIntf.u16Sz;
        }
        else
        {
            /** No blocking mode */
            eStatus = ptInst->tIntf.Read(&ptInst->tIntf, &mcbMsg->u16Node,
                                       &mcbMsg->u16SubNode, &mcbMsg->u16Addr,
                    &mcbMsg->u16Cmd, &mcbMsg->u16Data[0]);
        }

        if (eStatus == IPB_SUCCESS)
        {
            eResult = IPB_MESSAGE_SUCCESS;
        }
        else
        {
            eResult = IPB_MESSAGE_ERROR;
        }
    }
    else
    {
        /* Cyclic mode */
    }

    if (eStatus == IPB_SUCCESS)
    {
        eResult = IPB_MESSAGE_SUCCESS;
    }
    else
    {
        eResult = IPB_MESSAGE_ERROR;
    }

    return eResult;
}

