#ifndef MY_DISP_WORK_TYPE_H
#define MY_DISP_WORK_TYPE_H
/*
0:004 > dt disp_work!DP_SESSION_INFO
+ 0x000 logonHdl         : Int4B
+ 0x004 logonId : Uint2B
+ 0x006 sessionHdl : UChar
*/

#define WINAPI

typedef struct  
{
	int _logonHdl;
	unsigned short _logonId;
	unsigned char  _sessionHdl;
}MY_DP_SESSION_INFO;



/*
dt disp_work!DP_SERVER_ADDRESS
+ 0x000 addressType      : DP_SERVER_ADDRESS_TYPE
+ 0x004 addressInfo : DP_SERVER_ADDRESS_INFO
+ 0x054 agentType : DP_AGENT_TYPE
+ 0x058 agentInfo : DP_AGENT_INFO
+ 0x064 sessionInfo : DP_SESSION_INFO
+ 0x06c responseInfo : DP_RESPONSE_INFO
*/
typedef struct  
{
	int   _addressType;
	char _addressInfo[80];
	int   _agentType;
	char _agentInfo[12];
	MY_DP_SESSION_INFO _sessionInfo;
	char  responseInfo[8];
}MY_DP_SERVER_ADDRESS;

/*
disp_work!REQUEST_BUF
+ 0x000 rq_id            : Uint4B
+ 0x004 version : UChar
+ 0x005 reqprio : UChar
+ 0x008 userTraceLevel : Uint4B
+ 0x00c len : Uint4B
+ 0x010 sender : DP_SERVER_ADDRESS
+ 0x084 receiver : DP_SERVER_ADDRESS
+ 0x0f8 reqHandlerId : REQ_HANDLER_ID
+ 0x0fc compositeMessageId : Int4B
+ 0x100 compositeMessageCounter : Int4B
+ 0x108 compositeMessageTotalLength : Uint8B
+ 0x110 endOfCompositeMessage : UChar
+ 0x111 compositeMessageDataInGlobalBuffer : UChar
+ 0x114 reqRc : REQ_REQ_RC
+ 0x118 still_valid : UChar
+ 0x119 sendHandShake : UChar
+ 0x120 reqCreationTime : DP_TIMEOUT_VALUE
+ 0x130 ca_blk : Int4B
+ 0x140 shortData : [128] UChar
+ 0x140 align : SAP_MAX_ALIGN_T
+ 0x1c0 shortDataLen : Uint4B
+ 0x1c4 protocol : Int4B
+ 0x1c8 mpi_read_hdl : MpiHandle
+ 0x1e0 mpi_write_hdl : MpiHandle
+ 0x1f8 icm_opcode : Int4B
+ 0x1fc dtrace_creator_id : Int2B
+ 0x1fe dtrace_root_id : [16] UChar
+ 0x20e dtrace_conn_id : [16] UChar
+ 0x220 dtrace_seq_nr : Uint4B
*/
typedef struct  
{
	unsigned int _rq_id;
	unsigned char _version;
	unsigned char _reqprio[3];
	unsigned int _userTraceLevel;
	unsigned int _len;
	MY_DP_SERVER_ADDRESS _sender;
	MY_DP_SERVER_ADDRESS _receiver;
	unsigned int _reqHandlerId;
}MY_REQUEST_BUF;

#endif 
