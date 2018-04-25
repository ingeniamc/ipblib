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
    /** Waiting for write request ack */
    IPB_WRITE_REQUEST_ACK,
    /** Processing answer from write request */
    IPB_WRITE_ANSWER,
    /** Processing write */
    IPB_WRITE_ANSWER_PENDING,
    /** Sending a read request */
    IPB_READ_REQUEST,
    /** Waiting for read request ack */
    IPB_READ_REQUEST_ACK,
    /** Processing answer from read request */
    IPB_READ_ANSWER,
    /** Processing request */
    IPB_READ_REQUEST_PENDING,
    /** Waiting and processing slave cyclic frame */
    IPB_CYCLIC_ANSWER,
    /** Cancel transaction */
    IPB_CANCEL,
    /** Transaction error */
    IPB_ERROR
} EIpbStatus;

/** Ipb interfaces options */
typedef enum
{
    /** Uart interface */
    UART_BASED,
    /** USB interface */
    USB_BASED,
    /** Ethernet interface */
    ETHERNET_BASED
} EIpbIntf;

typedef struct IpbIntf IpbIntf;

struct IpbIntf
{
    /** Identification of the uart module */
    uint16_t u16Id;
    /** Indicates the state of the communication bus */
    EIpbStatus eState;
    /** Indicates the interface type */
    EIpbIntf eIntf;
    /** Frame pool for holding tx data */
    TIpbFrame Txfrm;
    /** Frame pool for holding rx data */
    TIpbFrame Rxfrm;
    /** Pending data size to be transmitted/received */
    uint16_t u16Sz;
    /** Pending bits flag */
    bool ifPending;
    /** Write frame */
    EIpbStatus (*Write)(IpbIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
            uint16_t* pu16Cmd, uint16_t* pu16Data, uint16_t* pu16Sz);
    /** Read frame */
    EIpbStatus (*Read)(IpbIntf* ptInst, uint16_t* pu16Node, uint16_t* pu16SubNode, uint16_t* pu16Addr,
            uint16_t* pu16Cmd, uint16_t* pu16Data);
};

/** Initialize a High speed protocol interface */
void
Ipb_IntfInit(IpbIntf* ptInst, EIpbIntf eIntf, uint16_t u16Id);

/** Deinitialize a high speed protocol interface */
void
Ipb_IntfDeinit(IpbIntf* ptInst);
#endif /* IPB_INTF_H */
