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

void Ipb_Init(IpbInst* ptInst, EIpbIntf eIntf, EIpbMode eMode)
{
    ptInst->eIntf = eIntf;
    ptInst->isCyclic = false;
    ptInst->eMode = eMode;

    Ipb_IntfInit(&ptInst->tIntf, eIntf);
}

void Ipb_Deinit(IpbInst* ptInst)
{
    ptInst->eIntf = UART_BASED;
    ptInst->isCyclic = false;
    ptInst->eMode = IPB_BLOCKING;
    IpbIntfDeinit(&ptInst->tIntf);
}

EIpbReqStatus Ipb_Write(IpbInst* ptInst, IpbMsg* mcbMsg, uint32_t u32Timeout)
{
    EIpbReqStatus eResult = IPB_MESSAGE_ERROR;
    EIpbStatus eStatus;
    uint16_t u16Sz;

    if (ptInst->isCyclic == false)
    {
        if (ptInst->eMode == IPB_BLOCKING)
        {
            uint32_t u32Millis = Ipb_GetMillis();
            do
            {
                eStatus = ptInst->tIntf.Write(&ptInst->tIntf, &mcbMsg->u16Node,
                                            &mcbMsg->u16SubNode,
                                            &mcbMsg->u16Addr, &mcbMsg->u16Cmd,
                                            &mcbMsg->u16Data[0], &mcbMsg->u16Size);

            } while ((eStatus != IPB_ERROR) && (eStatus != IPB_SUCCESS)
                    && ((Ipb_GetMillis() - u32Millis) < u32Timeout));
        }
        else
        {
            /** No blocking mode */
            eStatus = ptInst->tIntf.Write(&ptInst->tIntf, &mcbMsg->u16Node,
                                        &mcbMsg->u16SubNode, &mcbMsg->u16Addr,
                    &mcbMsg->u16Cmd, &mcbMsg->u16Data[0], &u16Sz);
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

#include "stm32f4xx_hal.h"
EIpbReqStatus Ipb_Read(IpbInst* ptInst, IpbMsg* mcbMsg, uint32_t u32Timeout)
{
    EIpbReqStatus eResult = 0;
    EIpbStatus eStatus = IPB_ERROR;

    if (ptInst->isCyclic == false)
    {
        if (ptInst->eMode == IPB_BLOCKING)
        {
            uint32_t u32Millis = Ipb_GetMillis();
            do
            {
                eStatus = ptInst->tIntf.Read(&ptInst->tIntf, &mcbMsg->u16Node,
                                           &mcbMsg->u16SubNode,
                                           &mcbMsg->u16Addr, &mcbMsg->u16Cmd,
                                           &mcbMsg->u16Data[0]);

                HAL_Delay(1);
            } while ((eStatus != IPB_ERROR) && (eStatus != IPB_SUCCESS)
                    && ((Ipb_GetMillis() - u32Millis) < u32Timeout));

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

