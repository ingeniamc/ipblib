/**
 * @file ipb_frame.h
 * @brief This file contains useful functions for framing purpose
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#ifndef IPB_FRAME_H
#define IPB_FRAME_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/** Ingenia protocol frame maximum buffer size */
#define IPB_FRM_MAX_DATA_SZ     512U

/** Ingenia protocol frame static buffer header size */
#define IPB_FRM_HEAD_SZ			2U
/** Ingenia protocol frame static buffer size */
#define IPB_FRM_CONFIG_SZ       4U
/** Ingenia protocol frame CRC size */
#define IPB_FRM_CRC_SZ          1U

/** Position of Node into frame */
#define IPB_FRM_NODE_IDX        0U
/** Position of commands into frame */
#define IPB_FRM_CMD_IDX         1U
/** Position of configdata into frame */
#define IPB_FRM_CFG_IDX         2U

/** Total size of a config frame */
#define IPB_FRAME_TOTAL_CFG_SIZE 7U

/** Ingenia protocol frame max util data size */
#define IPB_MAX_DATA_SZ         (IPB_FRM_MAX_DATA_SZ - IPB_FRM_HEAD_SZ - IPB_FRM_CRC_SZ)

/** Ingenia protocol static function requests/replies */
/** Read request */
#define IPB_REQ_READ            1U
/** Write request */
#define IPB_REQ_WRITE           2U
/** Idle request  */
#define IPB_REQ_IDLE            7U

/** Acknowledge */
#define IPB_REP_ACK             3U
/** Error detected during read */
#define IPB_REP_READ_ERROR      5U
/** Error detected during write */
#define IPB_REP_WRITE_ERROR     6U
/** General error */
#define IPB_REP_ERROR           4U

/** Ingenia protocol extended flag definitions */
#define IPB_FRM_NOTEXT          0U
#define IPB_FRM_EXT             1U

/** Ingenia protocol bus frame */
typedef struct {
	/** Data buffer */
    uint16_t u16Buf[IPB_FRM_MAX_DATA_SZ];
    /** Frame size */
    uint16_t u16Sz;
} Ipb_TFrame;

/**
 * Initialises an Ingenia High Speed Protocol frame.
 *
 * @param [out] tFrame
 *      Destination frame
 * @param [in] u16SubNode
 *      Destination internal network node.
 * @param [in] u16Addr
 *      Destination address.
 * @param [in] u16Cmd
 *      Frame command (request or reply)
 * @param [in] pu16Buf
 *      Buffer with data.
 * @param [in] u16Sz
 *      Size of data.
 * @param [in] calcCRC
 * 		Indicates if CRC field will be calculated.
 * @return 0 success, error code otherwise
 */
int32_t
Ipb_FrameCreate(Ipb_TFrame* tFrame, uint16_t u16SubNode, uint16_t u16Addr, uint8_t u8Cmd,
                const uint16_t* pu16Buf, uint16_t u16Sz, bool calcCRC);

/**
 * Returns the SubNode of the header.
 *
 * @param [in] tFrame
 *      Input frame.
 * @return SubNode.
 */
uint16_t
Ipb_FrameGetSubNode(const Ipb_TFrame *tFrame);

/**
 * Returns the address of the header.
 *
 * @param [in] tFrame
 *      Input frame.
 * @return Address.
 */
uint16_t
Ipb_FrameGetAddr(const Ipb_TFrame* tFrame);

/**
 * Returns the command (request or reply) of the static data.
 *
 * @param [in] tFrame
 *      Input frame.
 * @return Command.
 */
uint8_t
Ipb_FrameGetCmd(const Ipb_TFrame* tFrame);

/**
 * Checks if the frame is extended.
 *
 * @param [in] tFrame
 *      Input frame.
 * @return true if static data is extended.
 */
bool
Ipb_FrameGetExtended(const Ipb_TFrame* tFrame);

/**
 * Returns the static data of a frame.
 *
 * @param [in] tFrame
 *      Input frame.
 * @return Configtic data
 */
uint16_t
Ipb_FrameGetConfigData(const Ipb_TFrame* tFrame, uint16_t* buf);

/**
 * Indicates if the crc for the input frame is correct
 *
 * @param[in] tFrame
 *  Input frame
 *
 * @return true if crc is correct
 *         false if crc is wrong
 */
bool
Ipb_FrameCheckCRC(const Ipb_TFrame* tFrame);

#endif /* IPB_FRAME_H */
