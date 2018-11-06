/**
 * @file ipb_frame.c
 * @brief This file contains useful functions for framing purpose
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#include "ipb_frame.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ipb_checksum.h"

/** Frame description
 * Word 0..1    - Header
 * Word 2..5    - Static data
 * Word 6..N-1  - Dynamic data (optional)
 * Word N       - CRC
 */

typedef struct
{
    union
    {
        struct
        {
            /** Internal network node */
            uint16_t u4SubNode :4;
            /** Reserved bits */
            uint16_t u12Reserved :12;
        };
        uint16_t u16NodeAll;
    } NodeId;

    union
    {
        struct
        {
            /** Extended message */
            uint16_t u1Extended :1;
            /** Frame command identification */
            uint16_t u3Cmd :3;
            /** Address of the Static Data */
            uint16_t u12Addr :12;
        };
        uint16_t u16All;
    } Command;
} THeader;

uint16_t
Ipb_FrameCRC(const Ipb_TFrame* tFrame, uint16_t u16Sz);

int32_t Ipb_FrameCreate(Ipb_TFrame* tFrame, uint16_t u16SubNode, uint16_t u16Addr, uint8_t u8Cmd,
                        const uint16_t* pu16Buf, uint16_t u16Sz, bool calcCRC)
{
    int32_t i32Err = 0L;

    while (1)
    {
        THeader tHeader;

        if (tFrame == NULL)
        {
            i32Err = -1L;
            break;
        }

        /* Check max data size */
        if (u16Sz > IPB_MAX_DATA_SZ)
        {
            i32Err = -2L;
            break;
        }

        /* Build header and assign it to u16Buffer */
        tHeader.NodeId.u12Reserved = (uint16_t)10U;
        tHeader.NodeId.u4SubNode = u16SubNode;
        tFrame->pu16Buf[0] = tHeader.NodeId.u16NodeAll;
        tHeader.Command.u12Addr = u16Addr;
        tHeader.Command.u3Cmd = u8Cmd;
        tHeader.Command.u1Extended = (u16Sz > IPB_FRM_CONFIG_SZ);
        tFrame->pu16Buf[1] = tHeader.Command.u16All;

        /* Copy static & extended u16Buffer (if any) */
        if (pu16Buf != NULL)
        {
            memcpy(&tFrame->pu16Buf[IPB_FRM_HEAD_SZ], (const void*)pu16Buf,
                   (sizeof(tFrame->pu16Buf[0]) * u16Sz));
        }
        else
        {
            memset(&tFrame->pu16Buf[IPB_FRM_HEAD_SZ], 0L,
                   (sizeof(tFrame->pu16Buf[0]) * u16Sz));
        }

        tFrame->u16Sz = IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ;

        if (calcCRC != false)
        {
            /* Compute CRC and add it to u16Buffer */
            tFrame->pu16Buf[IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ] =
                        Ipb_FrameCRC(tFrame, (tFrame->u16Sz << 1));
            tFrame->u16Sz += IPB_FRM_CRC_SZ;
        }

        if (u16Sz > IPB_FRM_CONFIG_SZ)
        {
            memcpy(&tFrame->pu16Buf[tFrame->u16Sz], (const void*)(pu16Buf + IPB_FRM_CONFIG_SZ),
                   (sizeof(tFrame->pu16Buf[0]) * (u16Sz - IPB_FRM_CONFIG_SZ)));

            tFrame->u16Sz += (u16Sz - IPB_FRM_CONFIG_SZ);
        }

        break;
    }

    return i32Err;
}

uint16_t Ipb_FrameGetSubNode(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.NodeId.u16NodeAll = tFrame->pu16Buf[IPB_FRM_NODE_IDX];

    return (uint16_t) tHeader.NodeId.u4SubNode;
}

bool Ipb_FrameGetExtended(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.Command.u16All = tFrame->pu16Buf[IPB_FRM_CMD_IDX];
    return (bool) tHeader.Command.u1Extended;
}

uint16_t Ipb_FrameGetAddr(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.Command.u16All = tFrame->pu16Buf[IPB_FRM_CMD_IDX];

    return (uint16_t) tHeader.Command.u12Addr;
}

uint8_t Ipb_FrameGetCmd(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.Command.u16All = tFrame->pu16Buf[IPB_FRM_CMD_IDX];

    return (uint8_t) tHeader.Command.u3Cmd;
}

uint16_t Ipb_FrameGetConfigData(const Ipb_TFrame* tFrame, uint16_t* u16Buf)
{
    memcpy(u16Buf, &tFrame->pu16Buf[IPB_FRM_HEAD_SZ], (sizeof(tFrame->pu16Buf[0]) * IPB_FRM_CONFIG_SZ));
    return IPB_FRM_CONFIG_SZ;
}

bool Ipb_FrameCheckCRC(const Ipb_TFrame* tFrame)
{
    bool bRet = true;
	uint16_t u16CRCInx = tFrame->u16Sz - IPB_FRM_CRC_SZ;
    uint16_t u16CalcCRC = Ipb_FrameCRC(tFrame, u16CRCInx << 1);

    if (u16CalcCRC != tFrame->pu16Buf[u16CRCInx])
    {
        bRet = false;
    }

    return bRet;
}

uint16_t Ipb_FrameCRC(const Ipb_TFrame* tFrame, uint16_t u16Sz)
{
    uint16_t crc = CRC_START_XMODEM;
    uint8_t* pu8In = (uint8_t*)tFrame->pu16Buf;

    for (uint16_t u16Idx = 0; u16Idx < u16Sz; u16Idx++)
    {
        crc = update_crc_ccitt(crc, pu8In[u16Idx]);
    }

    return crc;
}
