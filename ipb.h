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

#define HSP_MAX_DATA_SZ 128

#define DFLT_TIMEOUT 100

typedef enum
{
    /* Blocking mode, each request block until response */
    IPB_BLOCKING = 0,
    /* Non Blocking mode, if not ready, return state */
    IPB_NON_BLOCKING
} EIpbMode;

typedef enum
{
    /* Message not ready */
    IPB_MESSAGE_NOT_READY = 0,
    /* Success request */
    IPB_MESSAGE_SUCCESS,
    /* Request error */
    IPB_MESSAGE_ERROR
} EIpbReqStatus;

/** Motion control but instance */
typedef struct
{
    /** Specific interface */
    EIpbIntf eIntf;
    /** Indicates if mcb is cyclic */
    bool isCyclic;
    /** Linked interface module */
    IpbIntf tIntf;
    /** Transmission mode */
    EIpbMode eMode;
} IpbInst;

/** Frame data struct */
typedef struct
{
    /* Node data */
    uint16_t u16Node;
    /* Subnode data */
    uint16_t u16SubNode;
    /* Address data */
    uint16_t u16Addr;
    /* Command data */
    uint16_t u16Cmd;
    /* Message total size (bytes) */
    uint16_t u16Size;
    /* Static data */
    uint16_t u16Data[HSP_MAX_DATA_SZ];
    /* Message status */
    EIpbReqStatus eStatus;
} IpbMsg;

/** Initialization functions */
void Ipb_Init(IpbInst* ptInst, EIpbIntf eIntf, EIpbMode eMode);
void Ipb_Deinit(IpbInst* ptInst);

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
EIpbReqStatus
Ipb_Write(IpbInst* ptInst, IpbMsg* mcbMsg, uint32_t u32Timeout);

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
EIpbReqStatus
Ipb_Read(IpbInst* ptInst, IpbMsg* mcbMsg, uint32_t u32Timeout);

#endif /* IPB_H */
