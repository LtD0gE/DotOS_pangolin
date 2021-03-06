/* BastetComm.c
 *
 * Bastet Modem Driver Device.
 *
 * Copyright (C) 2014 Huawei Device Co.,Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */


#include "v_typdef.h"
#include "VosPidDef.h"
#include "PsCommonDef.h"
#include "mdrv.h"
#include "TafTypeDef.h"
#include "BastetInit.h"
#include "BastetInterface.h"
#include "BastetRnicInterface.h"
#include "BastetPrivate.h"





#define THIS_FILE_ID                    PS_FILE_ID_BST_COMM_C
#define BST_MAX_WRITE_PAYLOAD           (2048)

struct bst_modem_rab_id {
    uint16_t modem_id;
    uint16_t rab_id;
};

extern char cur_netdev_name[IFNAMSIZ];
extern void ind_hisi_com(void *info, u32 len);
extern void Emcom_Ind_Modem_Support(u8 ucState);

int bastet_comm_write(u8 *msg, u32 len, u32 type)
{
    BST_ACOM_MSG_STRU *pMsg = NULL;
    VOS_UINT32 ulLength = 0;

    if (NULL == msg)
    {
        return -1;
    }

    ulLength        = sizeof(BST_ACOM_MSG_STRU) + len;
    pMsg            = (BST_ACOM_MSG_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(ACPU_PID_BASTET_COMM, ulLength);
    if (NULL == pMsg)
    {
        BST_PR_LOGE("PS_ALLOC_MSG_WITH_HEADER_LEN failed\n");
        return -1;
    }

    pMsg->ulReceiverPid = UEPS_PID_BASTET;
    TAF_MEM_CPY_S((VOS_VOID*)pMsg->aucValue, BST_MAX_WRITE_PAYLOAD, msg, (VOS_UINT32)len);

    pMsg->ulLen     = len;
    pMsg->enMsgType = type;
    if (PS_SEND_MSG(ACPU_PID_BASTET_COMM, pMsg) != 0)
    {
        BST_PR_LOGE("PS_SEND_MSG failed\n");
        return -1;
    }

    return 0;
}

int bastet_comm_keypsInfo_write(u32 ulState)
{
    BST_KEY_PSINFO_STRU *pMsg = NULL;
    VOS_UINT32 ulLength = 0;

    ulLength        = sizeof( BST_KEY_PSINFO_STRU );
    pMsg            = (BST_KEY_PSINFO_STRU *)PS_ALLOC_MSG(ACPU_PID_BASTET_COMM, ulLength);
    if (NULL == pMsg)
    {
        BST_PR_LOGE("PS_ALLOC_MSG_WITH_HEADER_LEN failed\n");
        return -1;
    }
    pMsg->enState = ulState;

    pMsg->enMsgType   = BST_ACORE_CORE_MSG_TYPE_EMCOM_KEY_PSINFO;

    pMsg->ulReceiverPid = UEPS_PID_BASTET;
    BST_PR_LOGD("state: %d", pMsg->enState);
    if (PS_SEND_MSG(ACPU_PID_BASTET_COMM, pMsg) != 0)
    {
        BST_PR_LOGE("PS_SEND_MSG failed\n");
        return -1;
    }

    return 0;
}


void bastet_comm_recv(MsgBlock *pMsg)
{
    u32                 len;
    BST_ACOM_MSG_STRU  *pTmpMsg;

    /*lint -e826*/
    pTmpMsg             = (BST_ACOM_MSG_STRU *)pMsg;
    /*lint +e826*/
    if ( NULL == pMsg )
    {
        BST_PR_LOGE("MsgBlock is empty\n");
        return;
    }

    len                 = pTmpMsg->ulLen;
    if ( len > BST_MAX_WRITE_PAYLOAD )
    {
        len             = BST_MAX_WRITE_PAYLOAD;
    }

    switch( pTmpMsg->enMsgType )
    {
        case BST_ACORE_CORE_MSG_TYPE_DSPP:
        {
            ind_hisi_com( pTmpMsg->aucValue, len );
            break;
        }
        case BST_ACORE_CORE_MSG_TYPE_EMCOM_SUPPORT:
        {
            BST_EMCOM_SUPPORT_STRU *pIndMsg = (BST_EMCOM_SUPPORT_STRU *)pMsg; //lint !e826
            Emcom_Ind_Modem_Support( pIndMsg->enState );
            break;
        }
        default:
            BST_PR_LOGE("pTmpMsg->enMsgType type error,state: %d\n", pTmpMsg->enMsgType);
            break;
    }
 }

/*
 * Get modem id and rab id.
 */
int get_modem_rab_id(struct bst_modem_rab_id *info)
{
    BST_RNIC_MODEM_INFO_STRU stModemInfo;
    VOS_INT rslt;

    if (NULL == info) {
        return -EINVAL;
    }

    rslt = RNIC_BST_GetModemInfo(cur_netdev_name,&stModemInfo);
    if (VOS_OK != rslt)
    {
        BST_PR_LOGE("get modem rab id fail\n");
        return -EPERM;
    }

    if (BST_RNIC_PDP_STATE_ACTIVE != stModemInfo.enIPv4State) {
        BST_PR_LOGE("Ipv4 pdp reg status inactive\n");
        return -EPERM;
    }

    info->modem_id = stModemInfo.usModemId;
    /* Bastet only running in IPv4 mode,
     * so, get IPv4 Pdp info */
    info->rab_id = stModemInfo.ucRabId;

    return 0;
}

void bastet_comm_init(void)
{
    BASTET_CommRegRecvCallBack((RECV_MSG_PROC)bastet_comm_recv);
}


