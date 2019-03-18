/**
 * @file ipb.h
 * @brief This file contains API functions of the
 *        ingenia protocol bus (IPB)
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#ifndef IPB_H
#define IPB_H

#include <stdint.h>
#include "ipb_intf.h"

#define IPB_DFLT_TIMEOUT 1000

typedef enum
{
    /* Blocking mode, each request block until response */
    IPB_BLOCKING = 0,
    /* Non Blocking mode, if not ready, return state */
    IPB_NON_BLOCKING
} Ipb_EMode;

/** Motion control but instance */
typedef struct
{
    /** Specific interface */
    Ipb_EIntf eIntf;
    /** Indicates if mcb is cyclic */
    bool isCyclic;
    /** Linked interface module */
    Ipb_TIntf tIntf;
    /** Transmission mode */
    Ipb_EMode eMode;
} Ipb_TInst;

/** Frame data struct */
typedef struct
{
    /** Subnode data */
    uint16_t u16SubNode;
    /** Address data */
    uint16_t u16Addr;
    /** Command data */
    uint16_t u16Cmd;
    /** Message total size */
    uint16_t u16Size;
    /** Pointer to data */
    uint16_t pu16Data[IPB_FRM_MAX_DATA_SZ - (IPB_FRM_HEAD_SZ + IPB_FRM_CRC_SZ)];
    /** Message status */
    Ipb_EStatus eStatus;
} Ipb_TMsg;

/** Initialization functions */
void Ipb_Init(Ipb_TInst* ptInst, Ipb_EIntf eIntf, Ipb_EMode eMode);
void Ipb_Deinit(Ipb_TInst* ptInst);

/**
 * Generic write function
 *
 * @param[in] ptInst
 *  Specifies the target instance
 * @param[in/out] mcbMsg
 *  Request to be send and load with reply
 * @param[in] u32Timeout
 *  Timeout duration
 */
Ipb_EStatus
Ipb_Write(Ipb_TInst* ptInst, Ipb_TMsg* ptMsg, uint32_t u32Timeout);

/**
 * Generic read function
 *
 * @param[in] ptInst
 *  Specifies the target instance
 * @param[in/out] mcbMsg
 *  Request to be send and load with reply
 * @param[in] u32Timeout
 *  Timeout duration
 */
Ipb_EStatus
Ipb_Read(Ipb_TInst* ptInst, Ipb_TMsg* ptMsg, uint32_t u32Timeout);

#endif /* IPB_H */
