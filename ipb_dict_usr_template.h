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

#ifdef IPB_DICT_DEF
uint8_t 
IpbReadReg1(uint16_t* pu16Data, uint16_t* pu16DataSz);

TIpbDictEntry ptIpbNode0Dict[] =
{
    { (uint16_t)0x0001U, &IpbReadReg1, NULL },
};

uint16_t u16IpbNode0Size = sizeof(ptIpbNode0Dict);

TIpbDictInst ptIpbDict[] =
{
     { (uint16_t)0U, ptIpbNode0Dict, &u16IpbNode0Size },
};
#endif

#endif /* IPB_DICT_USR_TEMPLATE_H */
