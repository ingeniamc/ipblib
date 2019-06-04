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

uint8_t 
IpbReadReg1(uint16_t* pu16Data, uint16_t* pu16DataSz);

uint8_t
IpbReadReg5(uint16_t* pu16Data, uint16_t* pu16DataSz);

uint8_t
IpbWriteReg5(uint16_t* pu16Data, uint16_t* pu16DataSz);

/** Dictionaries definitions example */
extern TIpbDictEntry ptIpbNode0Dict[];
extern uint16_t u16IpbNode0Size;
extern TIpbDictEntry ptIpbNode5Dict[];
extern uint16_t u16IpbNode5Size;

/** Mandatory defines link from ipblib to example dictionary */
#define DICT_IDX_0_NODE         (int16_t)0
#define DICT_IDX_1_NODE         (int16_t)-1 /** Not used */
#define DICT_IDX_2_NODE         (int16_t)5
#define DICT_IDX_3_NODE         (int16_t)-1 /** Not used */

#define DICT_IDX_0_DO_POINTER   (TIpbDictEntry*)ptIpbNode0Dict
#define DICT_IDX_1_DO_POINTER   (TIpbDictEntry*)NULL
#define DICT_IDX_2_DO_POINTER   (TIpbDictEntry*)ptIpbNode5Dict
#define DICT_IDX_3_DO_POINTER   (TIpbDictEntry*)NULL

#define DICT_IDX_0_SIZE_POINTER          &u16IpbNode0Size
#define DICT_IDX_1_SIZE_POINTER          NULL
#define DICT_IDX_2_SIZE_POINTER          &u16IpbNode5Size
#define DICT_IDX_3_SIZE_POINTER          NULL

#endif /* IPB_DICT_USR_TEMPLATE_H */
