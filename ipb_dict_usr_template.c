/**
 * @file ipb_dict_usr_template.c
 * @brief This file contains a dictionary template.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2019. All rights reserved.
 */

#include "ipb_dict_usr_template.h"

TIpbDictEntry ptIpbNode0Dict[] =
{
    { (uint16_t)0x0001U, &IpbReadReg1, NULL }
};

uint16_t u16IpbNode0Size = sizeof(ptIpbNode0Dict);

TIpbDictEntry ptIpbNode5Dict[] =
{
    { (uint16_t)0x0005U, &IpbReadReg5, &IpbWriteReg5 }
};
a
uint16_t u16IpbNode5Size = sizeof(ptIpbNode5Dict);

uint8_t IpbReadReg1(uint16_t* pu16Data, uint16_t* pu16DataSz)
{
    uint8_t u8Ret = NO_ERROR;

    return u8Ret;
}

uint8_t IpbReadReg5(uint16_t* pu16Data, uint16_t* pu16DataSz)
{
    uint8_t u8Ret = NO_ERROR;

    return u8Ret;
}

uint8_t IpbWriteReg5(uint16_t* pu16Data, uint16_t* pu16DataSz)
{
    uint8_t u8Ret = NO_ERROR;

    return u8Ret;
}
