/**
 * @file dictionary.c
 * @brief This file contains the COCO dictionary management.
 *
 * @author  Firmware department
 * @copyright Ingenia Motion Control (c) 2018. All rights reserved.
 */

#include "ipb_dict.h"
#include "ipb_dict_usr.h"

uint8_t Ipb_DictRead(Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NO_ERROR;

    switch (pIpbMsg->u16Addr)
    {
        case MONITOR_STATUS_KEY:
            u8Ret = McbDict_ReadReg0F0(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_ENABLE_KEY:
            u8Ret = McbDict_ReadReg0F1(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_CYCLE_PERIOD_KEY:
            u8Ret = McbDict_ReadReg0F2(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_NUMBER_CYCLES_KEY:
            u8Ret = McbDict_ReadReg0F3(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_DATA_KEY:
            u8Ret = McbDict_ReadReg0F4(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAP_REG_KEY:
            u8Ret = McbDict_ReadReg0E0(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_NUMBER_MAP_REGISTER_KEY:
            u8Ret = McbDict_ReadReg0E3(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_BLOCK_SIZE_BY_KEY:
            u8Ret = McbDict_ReadReg0E4(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAP_REG_KEY:
            u8Ret = McbDict_ReadReg0B0(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_NUMBER_DATA_SIZE_KEY:
            u8Ret = McbDict_ReadReg0B2(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_NUMBER_MAP_REGISTER_KEY:
            u8Ret = McbDict_ReadReg0B3(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_BLOCK_SIZE_BY_KEY:
            u8Ret = McbDict_ReadReg0B4(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_0_KEY:
            u8Ret = McbDict_ReadReg0D0(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_1_KEY:
            u8Ret = McbDict_ReadReg0D1(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_2_KEY:
            u8Ret = McbDict_ReadReg0D2(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_3_KEY:
            u8Ret = McbDict_ReadReg0D3(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_4_KEY:
            u8Ret = McbDict_ReadReg0D4(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_5_KEY:
            u8Ret = McbDict_ReadReg0D5(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_6_KEY:
            u8Ret = McbDict_ReadReg0D6(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_7_KEY:
            u8Ret = McbDict_ReadReg0D7(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_8_KEY:
            u8Ret = McbDict_ReadReg0D8(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_9_KEY:
            u8Ret = McbDict_ReadReg0D9(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_10_KEY:
            u8Ret = McbDict_ReadReg0DA(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_11_KEY:
            u8Ret = McbDict_ReadReg0DB(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_12_KEY:
            u8Ret = McbDict_ReadReg0DC(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_13_KEY:
            u8Ret = McbDict_ReadReg0DD(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_14_KEY:
            u8Ret = McbDict_ReadReg0DE(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONITOR_MAPPED_REGISTER_15_KEY:
            u8Ret = McbDict_ReadReg0DF(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_0_KEY:
           u8Ret = McbDict_ReadReg0A0(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
           break;
        case DIST_MAPPED_REGISTER_1_KEY:
            u8Ret = McbDict_ReadReg0A1(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_2_KEY:
            u8Ret = McbDict_ReadReg0A2(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_3_KEY:
            u8Ret = McbDict_ReadReg0A3(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_4_KEY:
            u8Ret = McbDict_ReadReg0A4(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_5_KEY:
            u8Ret = McbDict_ReadReg0A5(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_6_KEY:
            u8Ret = McbDict_ReadReg0A6(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_7_KEY:
            u8Ret = McbDict_ReadReg0A7(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_8_KEY:
            u8Ret = McbDict_ReadReg0A8(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_9_KEY:
            u8Ret = McbDict_ReadReg0A9(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_10_KEY:
            u8Ret = McbDict_ReadReg0AA(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_11_KEY:
            u8Ret = McbDict_ReadReg0AB(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_12_KEY:
            u8Ret = McbDict_ReadReg0AC(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_13_KEY:
            u8Ret = McbDict_ReadReg0AD(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_14_KEY:
            u8Ret = McbDict_ReadReg0AE(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case DIST_MAPPED_REGISTER_15_KEY:
            u8Ret = McbDict_ReadReg0AF(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONI_SOC_TRIGGER_TYPE_KEY:
            u8Ret = McbDict_ReadReg09E(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONI_TG_DELAY_SAMPLES_KEY:
            u8Ret = McbDict_ReadReg09D(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONI_WINDOW_SAMPLES_KEY:
            u8Ret = McbDict_ReadReg09C(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONI_RISING_EDGE_INDEX_KEY:
            u8Ret = McbDict_ReadReg09B(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONI_RISING_EDGE_CONDITION_KEY:
            u8Ret = McbDict_ReadReg09A(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONI_NMB_TG_REPETITIONS_KEY:
            u8Ret = McbDict_ReadReg099(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        case MONI_EOC_TRIGGER_TYPE_KEY:
            u8Ret = McbDict_ReadReg098(pIpbMsg->pu16Data, &pIpbMsg->u16Size);
            break;
        default:
            u8Ret = NOT_SUPPORTED;
            break;
    }

    return u8Ret;
}

uint8_t Ipb_DictWrite(Ipb_TMsg* pIpbMsg)
{
    uint8_t u8Ret = NO_ERROR;

    switch (pIpbMsg->u16Addr)
    {
       case MONITOR_ENABLE_KEY:
           u8Ret = McbDict_WriteReg0F1(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONITOR_CYCLE_PERIOD_KEY:
           u8Ret = McbDict_WriteReg0F2(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONITOR_MAP_REG_KEY:
           u8Ret = McbDict_WriteReg0E0(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONITOR_REMOVE_LAST_REG_KEY:
           u8Ret = McbDict_WriteReg0E1(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONITOR_REMOVE_REGS_KEY:
           u8Ret = McbDict_WriteReg0E2(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;

       case DIST_DATA_KEY:
           u8Ret = McbDict_WriteReg0C0(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case DIST_MAP_REG_KEY:
           u8Ret = McbDict_WriteReg0B0(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case DIST_REMOVE_LAST_REG_KEY:
           u8Ret = McbDict_WriteReg0B1(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONI_FORCE_SOC_KEY:
           u8Ret = McbDict_WriteReg09F(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONI_SOC_TRIGGER_TYPE_KEY:
           u8Ret = McbDict_WriteReg09E(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONI_TG_DELAY_SAMPLES_KEY:
           u8Ret = McbDict_WriteReg09D(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONI_WINDOW_SAMPLES_KEY:
           u8Ret = McbDict_WriteReg09C(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONI_RISING_EDGE_INDEX_KEY:
           u8Ret = McbDict_WriteReg09B(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONI_RISING_EDGE_CONDITION_KEY:
           u8Ret = McbDict_WriteReg09A(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONI_NMB_TG_REPETITIONS_KEY:
           u8Ret = McbDict_WriteReg099(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       case MONI_EOC_TRIGGER_TYPE_KEY:
           u8Ret = McbDict_WriteReg098(pIpbMsg->pu16Data, pIpbMsg->u16Size);
           break;
       default:
           u8Ret = NOT_SUPPORTED;
           break;
    }

    return u8Ret;
}
