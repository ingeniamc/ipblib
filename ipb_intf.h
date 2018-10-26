/**
 * @file ipb_intf.h
 * @brief This file contains API for accessing to low level interface
 *        of the ingenia protocol bus (IPB)
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#ifndef IPB_INTF_H
#define IPB_INTF_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "ipb_frame.h"
#include "ipb_usr.h"

/** Ipb communication states */
typedef enum
{
    /** Transmission successful */
    IPB_SUCCESS = 0,
    /** Bus in stand by */
    IPB_STANDBY,
    /** Sending a write request */
    IPB_WRITE_REQUEST,
    /** Processing answer from write request */
    IPB_WRITE_ANSWER,
    /** Sending a read request */
    IPB_READ_REQUEST,
    /** Processing answer from read request */
    IPB_READ_ANSWER,
    /** Transaction error */
	IPB_ERROR,
	/** Transaction write error */
	IPB_WRITE_ERROR,
	/** Transaction read error */
	IPB_READ_ERROR
} Ipb_EStatus;

/** Ipb interfaces options */
typedef enum
{
    /** Uart interface */
    UART_BASED,
    /** USB interface */
    USB_BASED,
    /** Ethernet interface */
    ETHERNET_BASED
} Ipb_EIntf;

typedef struct Ipb_TIntf Ipb_TIntf;

struct Ipb_TIntf
{
    /** Identification of the uart module */
    uint16_t u16Id;
    /** Indicates the state of the communication bus */
    Ipb_EStatus eState;
    /** Indicates the interface type */
    Ipb_EIntf eIntf;
    /** Frame pool for holding tx data */
    Ipb_TFrame Txfrm;
    /** Frame pool for holding rx data */
    Ipb_TFrame Rxfrm;
    /** Pending data size to be transmitted/received */
    uint16_t u16Sz;
    /** Pending bits flag */
    bool ifPending;
    /** Write frame */
    Ipb_EStatus (*Write)(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
            uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t* pu16Sz, bool isExt, void* pExtData);
    /** Read frame */
    Ipb_EStatus (*Read)(Ipb_TIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
            uint16_t* pu16Cmd, uint16_t* pu16Data);
};

/** Initialize a High speed protocol interface */
void
Ipb_IntfInit(Ipb_TIntf* ptInst, Ipb_EIntf eIntf, uint16_t u16Id);

/** Deinitialize a high speed protocol interface */
void
Ipb_IntfDeinit(Ipb_TIntf* ptInst);
#endif /* IPB_INTF_H */
