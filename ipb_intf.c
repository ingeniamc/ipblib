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

static Ipb_EStatus
Ipb_IntfReadUart(Ipb_TIntf* ptInst, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
                 uint16_t* pu16Data, uint16_t* u16Sz);

static Ipb_EStatus
Ipb_IntfWriteUart(Ipb_TIntf* ptInst, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
                  uint16_t* pu16Data, uint16_t u16Sz);

static Ipb_EStatus
Ipb_IntfReadUsb(Ipb_TIntf* ptInst, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
                uint16_t* pu16Data, uint16_t* u16Sz);

static Ipb_EStatus
Ipb_IntfWriteUsb(Ipb_TIntf* ptInst, uint16_t* pu16SubNode, uint16_t* pu16Addr, uint16_t* pu16Cmd,
                 uint16_t* pu16Data, uint16_t u16Sz);

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

Ipb_EStatus Ipb_IntfReadUart(Ipb_TIntf* ptInst, uint16_t* pu16SubNode, uint16_t* pu16Addr,
                             uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t* pu16Sz)
{

    if (ptInst->eState == IPB_STANDBY)
    {
        ptInst->eState = IPB_READ_REQUEST;
    }

    switch (ptInst->eState)
    {
        case IPB_READ_REQUEST:
            /** UART read */
            if (Ipb_IntfUartReception(ptInst->u16Id, (uint8_t*)&ptInst->Rxfrm,
                    ((IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ) * sizeof(uint16_t))) != 0)
            {
                ptInst->Rxfrm.u16Sz = (IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ);

                if (Ipb_FrameCheckCRC(&ptInst->Rxfrm) != false)
                {
                    *pu16SubNode = Ipb_FrameGetSubNode(&ptInst->Rxfrm);
                    *pu16Addr = Ipb_FrameGetAddr(&ptInst->Rxfrm);
                    *pu16Cmd = Ipb_FrameGetCmd(&ptInst->Rxfrm);
                    *pu16Sz = Ipb_FrameGetConfigData(&ptInst->Rxfrm, pu16Data);

                    if (Ipb_FrameGetExtended(&ptInst->Rxfrm) != false)
                    {
                        uint16_t u16ExtSz = Ipb_IntfUartReception(ptInst->u16Id,
                                             (uint8_t*)(ptInst->Rxfrm.pu16Buf + IPB_FRAME_TOTAL_CFG_SIZE), *pu16Data);
                        memcpy((void*)(pu16Data + (*pu16Sz)),
                               (const void*)(ptInst->Rxfrm.pu16Buf + IPB_FRAME_TOTAL_CFG_SIZE), (u16ExtSz * sizeof(uint16_t)));
                        ptInst->Rxfrm.u16Sz += u16ExtSz;
                        *pu16Sz += u16ExtSz;
                    }

                    ptInst->eState = IPB_SUCCESS;
                }
                else
                {
                    /** CRC Error */
                    ptInst->eState = IPB_ERROR;
                    Ipb_IntfUartDiscardData(ptInst->u16Id);
                }
            }
            break;
        default:
            ptInst->eState = IPB_STANDBY;
            break;
    }
    return ptInst->eState;
}

Ipb_EStatus Ipb_IntfWriteUart(Ipb_TIntf* ptInst, uint16_t* pu16SubNode, uint16_t* pu16Addr,
                              uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t u16Sz)
{
    if (ptInst->eState == IPB_STANDBY)
    {
        ptInst->eState = IPB_WRITE_REQUEST;
    }

    switch (ptInst->eState)
    {
        case IPB_WRITE_REQUEST:
        {
            Ipb_FrameCreate(&ptInst->Txfrm, *pu16SubNode, *pu16Addr, *pu16Cmd, pu16Data, u16Sz, true);
            ptInst->eState = IPB_SUCCESS;

            if (Ipb_IntfUartTransmission(ptInst->u16Id, (const uint8_t*)ptInst->Txfrm.pu16Buf,
                                         (ptInst->Txfrm.u16Sz * sizeof(uint16_t))) != false)
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

Ipb_EStatus Ipb_IntfReadUsb(Ipb_TIntf* ptInst, uint16_t* pu16SubNode, uint16_t* pu16Addr,
                            uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t* u16Sz)
{

    if (ptInst->eState == IPB_STANDBY)
    {
        ptInst->eState = IPB_READ_REQUEST;
    }

    switch (ptInst->eState)
    {
        case IPB_READ_REQUEST:
            /** UART read */
            if (Ipb_IntfUsbReception(ptInst->u16Id, (uint8_t*)&ptInst->Rxfrm,
                    ((IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ) * sizeof(uint16_t))) != 0)
            {
                ptInst->Rxfrm.u16Sz = (IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + IPB_FRM_CRC_SZ);

                if (Ipb_FrameCheckCRC(&ptInst->Rxfrm) != false)
                {
                    *pu16SubNode = Ipb_FrameGetSubNode(&ptInst->Rxfrm);
                    *pu16Addr = Ipb_FrameGetAddr(&ptInst->Rxfrm);
                    *pu16Cmd = Ipb_FrameGetCmd(&ptInst->Rxfrm);
                    *u16Sz = Ipb_FrameGetConfigData(&ptInst->Rxfrm, pu16Data);

                    if (Ipb_FrameGetExtended(&ptInst->Rxfrm) != false)
                    {
                        uint16_t u16ExtSz = Ipb_IntfUsbReception(ptInst->u16Id,
                                             (uint8_t*)(ptInst->Rxfrm.pu16Buf + IPB_FRAME_TOTAL_CFG_SIZE), *pu16Data);
                        memcpy((void*)(pu16Data + (*pu16Sz)),
                               (const void*)(ptInst->Rxfrm.pu16Buf + IPB_FRAME_TOTAL_CFG_SIZE), (u16ExtSz * sizeof(uint16_t)));
                        ptInst->Rxfrm.u16Sz += u16ExtSz;
                        *pu16Sz += u16ExtSz;
                    }

                    ptInst->eState = IPB_SUCCESS;
                }
                else
                {
                    /** CRC Error */
                    ptInst->eState = IPB_ERROR;
                    Ipb_IntfUsbDiscardData(ptInst->u16Id);
                }
            }
            break;
        default:
            ptInst->eState = IPB_STANDBY;
            break;
    }
    return ptInst->eState;
}

Ipb_EStatus Ipb_IntfWriteUsb(Ipb_TIntf* ptInst, uint16_t* pu16SubNode, uint16_t* pu16Addr,
                             uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t u16Sz)
{
    if (ptInst->eState == IPB_STANDBY)
    {
        ptInst->eState = IPB_WRITE_REQUEST;
    }

    switch (ptInst->eState)
    {
        case IPB_WRITE_REQUEST:
        {

            if (u16Sz <= IPB_MAX_DATA_SZ)
            {
                Ipb_FrameCreate(&ptInst->Txfrm, *pu16SubNode, *pu16Addr, *pu16Cmd, pu16Data, u16Sz, true);
                ptInst->eState = IPB_SUCCESS;

                if (Ipb_IntfUsbTransmission(ptInst->u16Id, (const uint8_t*)ptInst->Txfrm.pu16Buf, ptInst->Txfrm.u16Sz)
                        != false)
                {
                    ptInst->eState = IPB_ERROR;
                }
            }
            else
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
