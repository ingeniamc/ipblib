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

static uint16_t pTxData[IPB_FRM_CONFIG_SZ];

static Ipb_EStatus
Ipb_IntfReadUart(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
        uint16_t* pu16Data);

static Ipb_EStatus
Ipb_IntfWriteUart(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
        uint16_t* pu16Data, uint16_t* pu16Sz, bool isExt, void* pExtData);

static Ipb_EStatus
Ipb_IntfReadUsb(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
        uint16_t* pu16Data);

static Ipb_EStatus
Ipb_IntfWriteUsb(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
        uint16_t* pu16Data, uint16_t* pu16Sz, bool isExt, void* pExtData);

void Ipb_IntfInit(Ipb_TIntf* ptInst, Ipb_EIntf eIntf, uint16_t u16Id)
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
        case USB_BASED:
            /** USB slave mode */
            ptInst->Write = &Ipb_IntfWriteUsb;
            ptInst->Read = &Ipb_IntfReadUsb;
            break;
        default:
            /* Nothing */
            break;
    }

}

void Ipb_IntfDeinit(Ipb_TIntf* ptInst)
{
    ptInst->Write = NULL;
    ptInst->Read = NULL;
}

Ipb_EStatus Ipb_IntfReadUart(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
        uint16_t* pu16Cmd, uint16_t* pu16Data)
{

    if (ptInst->eState == IPB_STANDBY)
    {
        ptInst->eState = IPB_READ_REQUEST;
        ptInst->u16Sz = 0;
        pTxData[0] = 0;
    }

    switch (ptInst->eState)
    {
        case IPB_READ_REQUEST:
            /** UART read */
            if (Ipb_IntfUartReception(ptInst->u16Id, (uint8_t*) &ptInst->Rxfrm,
                    ((IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ) * sizeof(uint16_t))) != 0)
            {
                ptInst->Rxfrm.u16Sz = (IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ);

                if (Ipb_FrameCheckCRC(&ptInst->Rxfrm) != false)
                {
                    *pu16Node = Ipb_FrameGetNode(&ptInst->Rxfrm);
                    *pu16SubNode = Ipb_FrameGetSubNode(&ptInst->Rxfrm);
                    *pu16Addr = Ipb_FrameGetAddr(&ptInst->Rxfrm);
                    *pu16Cmd = Ipb_FrameGetCmd(&ptInst->Rxfrm);
                    ptInst->u16Sz += Ipb_FrameGetConfigData(&ptInst->Rxfrm, &pu16Data[ptInst->u16Sz]);

                    /** If request is segmented type */
                    if (Ipb_FrameGetSegmented(&ptInst->Rxfrm) != false)
                    {
                        if (ptInst->u16Sz > (size_t) IPB_FRM_MAX_DATA_SZ)
                        {
                            ptInst->eState = IPB_ERROR;
                        }
                        ptInst->eState = IPB_READ_ANSWER;
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
                    Ipb_IntfUartDiscardData(ptInst->u16Id);
                }
            }
            break;
        case IPB_READ_ANSWER:
            /** On segmented transmission an ACK per received message is required */
            pTxData[0] += 1;
            Ipb_FrameCreate(&(ptInst->Txfrm), *pu16Node, *pu16SubNode, *pu16Addr, IPB_REP_ACK, IPB_FRM_NOTEXT, (const void*)pTxData, NULL, 0, true);
            Ipb_IntfUartTransmission(ptInst->u16Id, (const uint8_t*) &(ptInst->Txfrm),
                    ((IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ) * sizeof(uint16_t)));
            ptInst->eState = IPB_READ_REQUEST;
            break;
        default:
            ptInst->eState = IPB_STANDBY;
            break;
    }
    return ptInst->eState;
}

Ipb_EStatus Ipb_IntfWriteUart(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
        uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t* pu16Sz, bool isExt, void* pExtData)
{
    if (ptInst->eState == IPB_STANDBY)
    {
        ptInst->eState = IPB_WRITE_REQUEST;
        ptInst->u16Sz = *pu16Sz;
    }

    switch (ptInst->eState)
    {
        case IPB_WRITE_REQUEST:
            if (ptInst->u16Sz <= IPB_FRM_CONFIG_SZ)
            {
                Ipb_FrameCreate(&ptInst->Txfrm, *pu16Node, *pu16SubNode, *pu16Addr, *pu16Cmd, isExt,
                        &pu16Data[(ptInst->u16Sz - *pu16Sz)], NULL, 0, true);
                ptInst->eState = IPB_SUCCESS;
            }
            else
            {
                ptInst->eState = IPB_ERROR;
            }

            if (Ipb_IntfUartTransmission(ptInst->u16Id, (const uint8_t*) &(ptInst->Txfrm),
                    (ptInst->Txfrm.u16Sz * sizeof(uint16_t))) != false)
            {
                ptInst->eState = IPB_ERROR;
            }
            else if (isExt)
            {
                /* Copy from config data slot the size of extended data */
                uint16_t u16ExtDataSzBy;
                memcpy((void*)&u16ExtDataSzBy, (const void*)pu16Data, sizeof(uint16_t));

                if (Ipb_IntfUartTransmission(ptInst->u16Id, (const uint8_t*)pExtData,
                                             u16ExtDataSzBy) != false)
                {
                    ptInst->eState = IPB_ERROR;
                }
            }
            break;
        default:
            ptInst->eState = IPB_STANDBY;
            break;
    }

    return ptInst->eState;
}

Ipb_EStatus Ipb_IntfReadUsb(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
        uint16_t* pu16Cmd, uint16_t* pu16Data)
{

    if (ptInst->eState == IPB_STANDBY)
    {
        ptInst->eState = IPB_READ_REQUEST;
        ptInst->u16Sz = 0;
        pTxData[0] = 0;
    }

    switch (ptInst->eState)
    {
        case IPB_READ_REQUEST:
            /** UART read */
            if (Ipb_IntfUsbReception(ptInst->u16Id, (uint8_t*) &ptInst->Rxfrm,
                    ((IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ) * sizeof(uint16_t))) != 0)
            {
                ptInst->Rxfrm.u16Sz = (IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ);

                if (Ipb_FrameCheckCRC(&ptInst->Rxfrm) != false)
                {
                    *pu16Node = Ipb_FrameGetNode(&ptInst->Rxfrm);
                    *pu16SubNode = Ipb_FrameGetSubNode(&ptInst->Rxfrm);
                    *pu16Addr = Ipb_FrameGetAddr(&ptInst->Rxfrm);
                    *pu16Cmd = Ipb_FrameGetCmd(&ptInst->Rxfrm);
                    ptInst->u16Sz += Ipb_FrameGetConfigData(&ptInst->Rxfrm, &pu16Data[ptInst->u16Sz]);

                    /** If request is segmented type */
                    if (Ipb_FrameGetSegmented(&ptInst->Rxfrm) != false)
                    {
                        if (ptInst->u16Sz > (size_t) IPB_FRM_MAX_DATA_SZ)
                        {
                            ptInst->eState = IPB_ERROR;
                        }
                        ptInst->eState = IPB_READ_ANSWER;
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
                    Ipb_IntfUsbDiscardData(ptInst->u16Id);
                }
            }
            break;
        case IPB_READ_ANSWER:
            /** On segmented transmission an ACK per received message is required */
            Ipb_FrameCreate(&(ptInst->Txfrm), *pu16Node, *pu16SubNode, *pu16Addr, IPB_REP_ACK, IPB_FRM_NOTEXT, (const void*)pTxData, NULL, 0, true);
            Ipb_IntfUsbTransmission(ptInst->u16Id, (const uint8_t*) &(ptInst->Txfrm),
                    ((IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ) * sizeof(uint16_t)));
            ptInst->eState = IPB_READ_REQUEST;
            break;
        default:
            ptInst->eState = IPB_STANDBY;
            break;
    }
    return ptInst->eState;
}

Ipb_EStatus Ipb_IntfWriteUsb(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
        uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t* pu16Sz, bool isExt, void* pExtData)
{
    if (ptInst->eState == IPB_STANDBY)
    {
        ptInst->eState = IPB_WRITE_REQUEST;
        ptInst->u16Sz = *pu16Sz;
    }

    switch (ptInst->eState)
    {
        case IPB_WRITE_REQUEST:
            if (ptInst->u16Sz <= IPB_FRM_CONFIG_SZ)
            {
                Ipb_FrameCreate(&ptInst->Txfrm, *pu16Node, *pu16SubNode, *pu16Addr, *pu16Cmd, isExt,
                        &pu16Data[(ptInst->u16Sz - *pu16Sz)], NULL, 0, true);
                ptInst->eState = IPB_SUCCESS;
            }
            else
            {
                ptInst->eState = IPB_ERROR;
            }

            if (Ipb_IntfUsbTransmission(ptInst->u16Id, (const uint8_t*) &(ptInst->Txfrm),
                    (ptInst->Txfrm.u16Sz * sizeof(uint16_t))) != false)
            {
                ptInst->eState = IPB_ERROR;
            }
            else if (isExt)
            {
                /* Copy from config data slot the size of extended data */
                uint16_t u16ExtDataSzBy;
                memcpy((void*)&u16ExtDataSzBy, (const void*)pu16Data, sizeof(uint16_t));

                if (Ipb_IntfUsbTransmission(ptInst->u16Id, (const uint8_t*)pExtData,
                                             u16ExtDataSzBy) != false)
                {
                    ptInst->eState = IPB_ERROR;
                }
            }

            break;
        default:
            ptInst->eState = IPB_STANDBY;
            break;
    }

    return ptInst->eState;
}
