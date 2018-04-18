/**
 * @file ipb_intf.c
 * @brief This file contains API for accessing to low level interface
 *        of the ingenia protocol bus (IPB)
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#include "ipb_intf.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define DFLT_TIMEOUT  100
#define SIZE_WORDS    2

static uint16_t bAbortFlag;

static bool IpbIntfUartReception(void);
static bool IpbIntfUartTransmission(void);

static EIpbStatus
IpbIntfReadUart(IpbIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
        uint16_t* pu16Data);

static EIpbStatus
IpbIntfWriteUart(IpbIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
        uint16_t* pu16Data, uint16_t* pu16Sz);

void IpbIntfInit(IpbIntf* ptInst, EIpbIntf eIntf)
{
    ptInst->eState = IPB_STANDBY;

    switch (eIntf)
    {
        case UART_BASED:
            /* UART Slave mode */
            ptInst->Write = &IpbIntfWriteUart;
            ptInst->Read = &IpbIntfReadUart;
            break;
        default:
            /* Nothing */
            break;
    }

    bAbortFlag = false;
}

void IpbIntfDeinit(IpbIntf* ptInst)
{
    ptInst->Write = NULL;
    ptInst->Read = NULL;
}

EIpbStatus IpbIntfReadUart(IpbIntf* ptInst, uint16_t* ptNode,
                            uint16_t* ptSubNode, uint16_t* ptAddr,
                            uint16_t* ptCmd, uint16_t* ptData)
{
    switch (ptInst->eState)
    {
        case IPB_STANDBY:
            ptInst->eState = IPB_READ_REQUEST;
            ptInst->u16Sz = 0;
            break;
        case IPB_READ_REQUEST:
            /** UART read */
            if (IpbIntfUartReception() != false)
            {
                bAbortFlag = false;

                if (IpbFrameCheckCRC(&ptInst->Rxfrm))
                {
                    *ptNode = IpbFrameGetNode(&ptInst->Rxfrm);
                    *ptSubNode = IpbFrameGetSubNode(&ptInst->Rxfrm);
                    *ptAddr = IpbFrameGetAddr(&ptInst->Rxfrm);
                    *ptCmd = IpbFrameGetCmd(&ptInst->Rxfrm);
                    ptInst->u16Sz += IpbFrameGetConfigData(&ptInst->Rxfrm, &ptData[ptInst->u16Sz]);

                    /** If request is segmented type */
                    if ((ptInst->u16Sz > IPB_FRM_CONFIG_SZ) || (IpbFrameGetSegmented(&ptInst->Rxfrm) != false))
                    {
                        if (ptInst->u16Sz > (size_t) IPB_FRM_MAX_DATA_SZ)
                        {
                            ptInst->eState = IPB_ERROR;
                        }
                        ptInst->eState = IPB_READ_REQUEST_ACK;
                    }
                    else
                    {
                        ptInst->eState = IPB_SUCCESS;
                    }
                }
                else
                {
                    /** CRC Error */
                    ptInst->eState = IPB_ERROR;
                }
            }
            break;
        case IPB_READ_REQUEST_ACK:
            IpbFrameCreate(&(ptInst->Txfrm), 0, 0, 0, IPB_READ_REQUEST_ACK, IPB_FRM_NOTSEG, NULL, NULL, 0, true);

            IpbIntfUartTransmission();

            if (IpbFrameGetSegmented(&ptInst->Rxfrm) == 0)
            {
                ptInst->eState = IPB_SUCCESS;
            }
            else
            {
                ptInst->eState = IPB_READ_REQUEST;
            }
            break;
        default:
            ptInst->eState = IPB_STANDBY;
            break;
    }
    return ptInst->eState;
}

EIpbStatus IpbIntfWriteUart(IpbIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
        uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t* pu16Sz)
{
    switch (ptInst->eState)
    {
        case IPB_STANDBY:
            ptInst->eState = IPB_WRITE_ANSWER;
            ptInst->u16Sz = *pu16Sz;
            break;
        case IPB_WRITE_ANSWER:
        case IPB_WRITE_ANSWER_PENDING:
            if (*pu16Sz <= IPB_FRM_CONFIG_SZ)
            {
                IpbFrameCreate(&ptInst->Txfrm, *pu16Node, *pu16SubNode, *pu16Addr, *pu16Cmd, IPB_FRM_NOTSEG,
                        &pu16Data[(ptInst->u16Sz - *pu16Sz)], NULL, 0, true);
            }
            else
            {
                IpbFrameCreate(&ptInst->Txfrm, *pu16Node, *pu16SubNode, *pu16Addr, *pu16Cmd, IPB_FRM_SEG,
                        &pu16Data[(ptInst->u16Sz - *pu16Sz)], NULL, 0, true);
            }
            uint16_t u16Tmp;
            for (int i = 0; i < ptInst->Txfrm.u16Sz; i++)
            {
                u16Tmp = ptInst->Txfrm.u16Buf[i];
                ptInst->Txfrm.u16Buf[i] = ((u16Tmp & 0x00ff) << 8) |
                                        ((u16Tmp & 0xff00) >> 8);
            }
            if (IpbIntfUartTransmission() != false)
            {
                ptInst->eState = IPB_SUCCESS;
            }
            else
            {
                ptInst->eState = IPB_ERROR;
            }
            break;
        default:
            ptInst->eState = IPB_STANDBY;
            break;
    }

    return ptInst->eState;
}

void TIM7_PeriodElapsedCallback(void)
{
    uint16_t u16PendingDMAFifoBytes = 0;
    //__HAL_DMA_GET_COUNTER(huart2.hdmarx);

    if (u16PendingDMAFifoBytes < IPB_FRAME_TOTAL_CFG_SIZE)
    {
        if (bAbortFlag)
        {
            //HAL_UART_Abort(&huart2);
            //HAL_DMA_Abort(huart2.hdmarx);
            bAbortFlag = false;
        }
        else
        {
            bAbortFlag = true;
        }
    }
    else
    {
        bAbortFlag = false;
    }
}

static bool IpbIntfUartReception(void)
{
    //HAL_UART_Receive_DMA(ptInst->phUsart, (uint8_t*) ptInst->Rxfrm.u16Buf,
    //        ((IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ) << 2));
    return false;
}

static bool IpbIntfUartTransmission(void)
{
    //HAL_UART_Transmit_DMA(ptInst->phUsart, (uint8_t*) ptInst->Txfrm.u16Buf, (ptInst->Txfrm.sz * SIZE_WORDS));
    return false;
}
