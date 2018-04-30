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
            /** Node address */
            uint16_t u12Node :12;
            /** Internal network node */
            uint16_t u4SubNode :4;
        };
        uint16_t u16NodeAll;
    } NodeId;

    union
    {
        struct
        {
            /** Segmented message */
            uint16_t u1Pending :1;
            /** Frame command identification */
            uint16_t u3Cmd :3;
            /** Address of the Static Data */
            uint16_t u12Addr :12;
        };
        uint16_t u16All;
    } Command;
} THeader;

/** Ingenia protocol frame header size */
#define IPB_FRM_HDR_SZ         1U
/** Ingenia protocol frame CRC size */
#define IPB_FRM_CRC_SZ         1U
/** Ingenia protocol frame dynamic u16Buffer size */
#define IPB_FRM_MAX_DYN_SZ     (IPB_FRM_MAX_DATA_SZ - IPB_FRM_HDR_SZ - \
                                IPB_FRM_CONFIG_SZ - IPB_FRM_CRC_SZ)

uint16_t
Ipb_FrameCRC(const Ipb_TFrame* tFrame);

int32_t Ipb_FrameCreate(Ipb_TFrame* tFrame, uint16_t u16Node, uint16_t u16SubNode, uint16_t u16Addr, uint8_t u8Cmd,
        uint8_t u8Pending, const void* pConfigBuf, const void* pCyclicBuf, uint16_t u16CycliSz, bool calcCRC)
{
    int32_t err = 0;

    while (1)
    {
        THeader tHeader;

        if (tFrame == NULL)
        {
            err = -1;
            break;
        }

        /* Check dynamic u16Buffer size */
        if (u16CycliSz > IPB_FRM_MAX_DYN_SZ)
        {
            err = -2;
            break;
        }

        /* Build header and assign it to u16Buffer */
        tHeader.NodeId.u12Node = u16Node;
        tHeader.NodeId.u4SubNode = u16SubNode;
        tFrame->u16Buf[0] = tHeader.NodeId.u16NodeAll;
        tHeader.Command.u12Addr = u16Addr;
        tHeader.Command.u3Cmd = u8Cmd;
        tHeader.Command.u1Pending = u8Pending;
        tFrame->u16Buf[1] = tHeader.Command.u16All;

        /* Copy static & dynamic u16Buffer (if any) */
        if (pConfigBuf != NULL)
        {
            memcpy(&tFrame->u16Buf[IPB_FRM_HEAD_SZ], pConfigBuf, (sizeof(tFrame->u16Buf[0]) * IPB_FRM_CONFIG_SZ));
        }
        else
        {
            memset(&tFrame->u16Buf[IPB_FRM_HEAD_SZ], 0, (sizeof(tFrame->u16Buf[0]) * IPB_FRM_CONFIG_SZ));
        }

        memcpy(&tFrame->u16Buf[(IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ)], pConfigBuf,
                (sizeof(tFrame->u16Buf[0]) * u16CycliSz));

        tFrame->u16Sz = IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ + u16CycliSz;

        if (calcCRC != false)
        {
            /* Compute CRC and add it to u16Buffer */
            tFrame->u16Buf[(IPB_FRM_HEAD_SZ + IPB_FRM_CONFIG_SZ) + u16CycliSz] =
                        Ipb_FrameCRC(tFrame);
            tFrame->u16Sz += IPB_FRM_CRC_SZ;
        }
        break;
    }

    return err;
}

uint16_t Ipb_FrameGetNode(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.NodeId.u16NodeAll = tFrame->u16Buf[IPB_FRM_NODE_IDX];

    return (uint16_t) tHeader.NodeId.u12Node;
}

uint16_t Ipb_FrameGetSubNode(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.NodeId.u16NodeAll = tFrame->u16Buf[IPB_FRM_NODE_IDX];

    return (uint16_t) tHeader.NodeId.u4SubNode;
}

bool Ipb_FrameGetSegmented(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.Command.u16All = tFrame->u16Buf[IPB_FRM_CMD_IDX];
    return (bool) tHeader.Command.u1Pending;
}

uint16_t Ipb_FrameGetAddr(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.Command.u16All = tFrame->u16Buf[IPB_FRM_CMD_IDX];

    return (uint16_t) tHeader.Command.u12Addr;
}

uint8_t Ipb_FrameGetCmd(const Ipb_TFrame* tFrame)
{
    THeader tHeader;

    tHeader.Command.u16All = tFrame->u16Buf[IPB_FRM_CMD_IDX];

    return (uint8_t) tHeader.Command.u3Cmd;
}

uint16_t Ipb_FrameGetConfigData(const Ipb_TFrame* tFrame, uint16_t* u16Buf)
{
    memcpy(u16Buf, &tFrame->u16Buf[IPB_FRM_HEAD_SZ], (sizeof(tFrame->u16Buf[0]) * IPB_FRM_CONFIG_SZ));
    return IPB_FRM_CONFIG_SZ;
}

bool Ipb_FrameCheckCRC(const Ipb_TFrame* tFrame)
{
    bool bCRC = true;

    if (Ipb_FrameCRC(tFrame) != 0)
    {
        bCRC = false;
    }

    return bCRC;
}

uint16_t Ipb_FrameCRC(const Ipb_TFrame* tFrame)
{
    uint16_t crc = CRC_START_XMODEM;
    uint8_t* pu8In = (uint8_t*) tFrame->u16Buf;

    for (uint16_t i = 0; i < (tFrame->u16Sz * 2); i++)
    {
        crc = update_crc_ccitt(crc, pu8In[i]);
    }

    return crc;
}
