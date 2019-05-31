/**
 * @file ipb_dict_usr_template.h
 * @brief Header file for the dictionary.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2019. All rights reserved.
 */

#ifndef IPB_DICT_USR_TEMPLATE_H
#define IPB_DICT_USR_TEMPLATE_H

#include "ipb_dict.h"
#include <stdint.h>

/** Ipb node 0 dictionary definitions */
#define IPB_NODE_0      (uint16_t)0U

uint8_t 
IpbReadReg1(uint16_t* pu16Data, uint16_t* pu16DataSz);

TIpbDictEntry ptIpbNode0Dict[] =
{
    { (uint16_t)0x0001U, &IpbReadReg1, NULL },
};

uint16_t u16IpbNode0Size = sizeof(ptIpbNode0Dict);

#endif /* IPB_DICT_USR_TEMPLATE_H */
