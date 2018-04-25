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

#define SIZE_WORDS    2

static uint16_t bAbortFlag;

static EIpbStatus
Ipb_IntfReadUart(IpbIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
        uint16_t* pu16Data);

static EIpbStatus
Ipb_IntfWriteUart(IpbIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
        uint16_t* pu16Data, uint16_t* pu16Sz);

void Ipb_IntfInit(IpbIntf* ptInst, EIpbIntf eIntf, uint16_t u16Id)
{
    ptInst->eState = IPB_STANDBY;
    ptInst->u16Id = u16Id;

    switch (eIntf)
    {
        case UART_BASED:
            /* UART Slave mode */
            ptInst->Write = &Ipb_IntfWriteUart;
            ptInst->Read = &Ipb_IntfReadUart;
            break;
        default:
            /* Nothing */
            break;
    }

    bAbortFlag = false;
}

void Ipb_IntfDeinit(IpbIntf* ptInst)
{
    ptInst->Write = NULL;
    ptInst->Read = NULL;
}

EIpbStatus Ipb_IntfReadUart(IpbIntf* ptInst, uint16_t* ptNode, uint16_t* ptSubNode, uint16_t* ptAddr, uint16_t* ptCmd,
        uint16_t* ptData)
{
    switch (ptInst->eState)
    {
        case IPB_STANDBY:
            ptInst->eState = IPB_READ_REQUEST;
            ptInst->u16Sz = 0;
            break;
        case IPB_READ_REQUEST:
            /** UART read */
            if (Ipb_IntfUartReception(ptInst->u16Id, (uint8_t*) &ptInst->Rxfrm,
                    ((IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ) * SIZE_WORDS)) != 0)
            {
                bAbortFlag = false;
                ptInst->Rxfrm.u16Sz = (IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ);
                if (Ipb_FrameCheckCRC(&ptInst->Rxfrm))
                {
                    *ptNode = Ipb_FrameGetNode(&ptInst->Rxfrm);
                    *ptSubNode = Ipb_FrameGetSubNode(&ptInst->Rxfrm);
                    *ptAddr = Ipb_FrameGetAddr(&ptInst->Rxfrm);
                    *ptCmd = Ipb_FrameGetCmd(&ptInst->Rxfrm);
                    ptInst->u16Sz += Ipb_FrameGetConfigData(&ptInst->Rxfrm, &ptData[ptInst->u16Sz]);

                    /** If request is segmented type */
                    if ((ptInst->u16Sz > IPB_FRM_CONFIG_SZ) || (Ipb_FrameGetSegmented(&ptInst->Rxfrm) != false))
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
            Ipb_FrameCreate(&(ptInst->Txfrm), 0, 0, 0, IPB_READ_REQUEST_ACK, IPB_FRM_NOTSEG, NULL, NULL, 0, true);

            Ipb_IntfUartTransmission(ptInst->u16Id, (const uint8_t*) &(ptInst->Txfrm), (ptInst->u16Sz * SIZE_WORDS));

            if (Ipb_FrameGetSegmented(&ptInst->Rxfrm) == 0)
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

EIpbStatus Ipb_IntfWriteUart(IpbIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
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
                Ipb_FrameCreate(&ptInst->Txfrm, *pu16Node, *pu16SubNode, *pu16Addr, *pu16Cmd, IPB_FRM_NOTSEG,
                        &pu16Data[(ptInst->u16Sz - *pu16Sz)], NULL, 0, true);
            }
            else
            {
                Ipb_FrameCreate(&ptInst->Txfrm, *pu16Node, *pu16SubNode, *pu16Addr, *pu16Cmd, IPB_FRM_SEG,
                        &pu16Data[(ptInst->u16Sz - *pu16Sz)], NULL, 0, true);
            }

            if (Ipb_IntfUartTransmission(ptInst->u16Id, (const uint8_t*) &(ptInst->Txfrm),
                    (ptInst->u16Sz * SIZE_WORDS)) != false)
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
