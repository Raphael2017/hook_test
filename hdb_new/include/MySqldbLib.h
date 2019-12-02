#pragma once
#include "SQLDBC_Types.h"
#include <map>

#define WINAPI

struct NxlDBCUserInfo;
class StatementSQL;
class HookManager;
typedef long long __int64;

typedef  SQLDBC_Retcode(WINAPI *SQLDBC_PreparedStatement_prepare_1_Fun)(void* pThis, char const * szSql);
typedef  SQLDBC_Retcode(WINAPI *SQLDBC_PreparedStatement_prepare_2_Fun)(void* pThis, char const * szSql,  SQLDBC_StringEncodingType::Encoding encoding);
typedef  SQLDBC_Retcode(WINAPI *SQLDBC_PreparedStatement_prepare_3_Fun)(void* pThis, char const * szSql, __int64 sqlLength,  SQLDBC_StringEncodingType::Encoding encoding);
typedef  SQLDBC_Retcode(WINAPI *SQLDBC_Statement_execute_1_Fun)(void* pThis, char const * szSql);
typedef  SQLDBC_Retcode(WINAPI *SQLDBC_PreparedStatement_execute_2_Fun)(void* pThis);
typedef  void*         (WINAPI *SQLDBC_Statement_getResultSet_1_Fun)(void* pThis);
typedef  SQLDBC_Retcode(WINAPI *SQLDBC_ResultSet_next_Fun)(void* pThis);
typedef  SQLDBC_Retcode(WINAPI *SQLDBC_ResultSet_getObject_1_Fun)(void* pThis, int Index,
	                            SQLDBC_HostType Type, void * paramAddr, __int64 * LengthIndicator,
	                             __int64 Size, bool Terminate);
typedef void*          (WINAPI *SQLDBC_Statement_getConnection_Fun)(void* pThis);

typedef SQLDBC_Retcode (WINAPI *SQLDBC_Connection_connect_2_Fun)(const char *servernode, SQLDBC_Length servernodeLength, const char *serverdb, SQLDBC_Length serverdbLength, const char *username, SQLDBC_Length usernameLength, const char *password, SQLDBC_Length passwordLength, const SQLDBC_StringEncoding userpwdEncoding);
typedef SQLDBC_Retcode (WINAPI *SQLDBC_Connection_connect_3_Fun)(const char *servernode, const char *serverdb, const char *username, const char *password, const SQLDBC_StringEncoding userpwdEncoding);
typedef SQLDBC_Retcode (WINAPI *SQLDBC_Connection_connect_4_Fun)(const char *servernode, const char *serverdb, const char *username, const char *password);
typedef void*          (WINAPI *SQLDBC_Connection_createStatement_Fun)(void* pThis);
typedef void*          (WINAPI *SQLDBC_Connection_createPreparedStatement_Fun)(void* pThis);
typedef void           (WINAPI *SQLDBC_Connection_releaseStatement_1_Fun)(void* pThis, void* pStatement);
typedef SQLDBC_Retcode (WINAPI *SQLDBC_Statement_executeBatch_Fun)(void* pThis);

typedef SQLDBC_Retcode (WINAPI *SQLDBC_PreparedStatement_executeItab_1_Fun)(void* pThis,/*struct SQLDBC_ItabDescriptor &*/void* pItabDesc, bool b);
typedef SQLDBC_Retcode (WINAPI *SQLDBC_PreparedStatement_executeItab_2_Fun)(void* pThis, /*struct SQLDBC_ShmDescriptor &*/void* pShmDesc, /*struct SQLDBC_ItabDescriptor &*/ void* pItabDesc, unsigned int nParam, bool bParam);
typedef SQLDBC_Retcode (WINAPI *SQLDBC_PreparedStatement_executeItab_3_Fun)(void* pThis, void * pVoid, bool bValue);
typedef void*          (WINAPI *SQLDBC_PreparedStatement_getItabReader_Fun)(void* pThis);

typedef SQLDBC_Retcode (WINAPI *SQLDBC_PreparedStatement_bindParameter_1_Fun)(void* pThis,const SQLDBC_UInt2 Index,
	const SQLDBC_HostType 	Type,
	void * 	paramAddr,
	SQLDBC_Length * 	LengthIndicator,
	const SQLDBC_Length 	Size,
	const SQLDBC_Bool 	Terminate
);
typedef SQLDBC_Retcode (WINAPI *SQLDBC_PreparedStatement_bindParameter_2_Fun)(void* pThis,const SQLDBC_UInt2 Index,
	const SQLDBC_HostType 	Type,
	void * 	paramAddr,
	SQLDBC_Length * 	LengthIndicator,
	const SQLDBC_Length 	Size,
	SQLDBC_Length 	para6,
	SQLDBC_Length 	Para7,
	const SQLDBC_Bool 	Terminate
);

class CMySqldbLib
{
public:
	CMySqldbLib();
	~CMySqldbLib();

	static bool AddedHook();

public:
	static void ReleaseEnforceStatement(void* pStatement);


	//hook function
private:
	static  SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_prepare_1(void* pThis, char const * szSql);
	static  SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_prepare_2(void* pThis, char const * szSql,  SQLDBC_StringEncodingType::Encoding encoding);
	static  SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_prepare_3(void* pThis, char const * szSql, __int64 sqlLength,  SQLDBC_StringEncodingType::Encoding encoding);
	
	
	static  SQLDBC_Retcode WINAPI My_SQLDBC_Statement_execute_1(void* pThis, char const * szSql);
	static  SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_execute_2(void* pThis);
	static  SQLDBC_Retcode WINAPI My_SQLDBC_Statement_executeBatch(void* pThis);

	static SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_executeItab_1(void* pThis,/*struct SQLDBC_ItabDescriptor &*/void* pItabDesc, bool b);
	static SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_executeItab_2(void* pThis, /*struct SQLDBC_ShmDescriptor &*/void* pShmDesc, /*struct SQLDBC_ItabDescriptor &*/ void* pItabDesc, unsigned int nParam, bool bParam);
	static SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_executeItab_3(void* pThis, void * pVoid, bool bValue);
	static void*          WINAPI My_SQLDBC_PreparedStatement_getItabReader(void* pThis);


	static SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_bindParameter_1(void* pThis,const SQLDBC_UInt2 Index,
		const SQLDBC_HostType 	Type,
		void * 	paramAddr,
		SQLDBC_Length * 	LengthIndicator,
		const SQLDBC_Length 	Size,
		const SQLDBC_Bool 	Terminate = SQLDBC_TRUE
	);
	static SQLDBC_Retcode WINAPI My_SQLDBC_PreparedStatement_bindParameter_2(void* pThis,const SQLDBC_UInt2 Index,
		const SQLDBC_HostType 	Type,
		void * 	paramAddr,
		SQLDBC_Length * 	LengthIndicator,
		const SQLDBC_Length 	Size,
		SQLDBC_Length 	para6,
		SQLDBC_Length 	Para7,
		const SQLDBC_Bool 	Terminate = SQLDBC_TRUE
	);

	static  void*          WINAPI My_SQLDBC_Statement_getResultSet_1(void* pThis);
	static  void*          WINAPI My_SQLDBC_Statement_getConnection(void* pThis);

	static  SQLDBC_Retcode WINAPI My_SQLDBC_ResultSet_next(void* pThis);
	static  SQLDBC_Retcode WINAPI My_SQLDBC_ResultSet_getObject_1(void* pThis, int Index,
			                          SQLDBC_HostType Type, void * paramAddr, __int64 * LengthIndicator, 
			                          __int64 Size, bool Terminate = SQLDBC_TRUE);

	//SQLDBC_Retcode connect(const char *connectURL, SQLDBC_Length connectURLLength, const char *connectCommand, SQLDBC_Length connectCommandLength, SQLDBC_StringEncoding connectCommandEncoding, SQLDBC_ConnectProperties &connectProperties);
	static SQLDBC_Retcode WINAPI My_SQLDBC_Connection_connect_2(const char *servernode, SQLDBC_Length servernodeLength, const char *serverdb, SQLDBC_Length serverdbLength, const char *username, SQLDBC_Length usernameLength, const char *password, SQLDBC_Length passwordLength, const SQLDBC_StringEncoding userpwdEncoding);
    static SQLDBC_Retcode WINAPI My_SQLDBC_Connection_connect_3(const char *servernode, const char *serverdb, const char *username, const char *password, const SQLDBC_StringEncoding userpwdEncoding);
	static SQLDBC_Retcode WINAPI My_SQLDBC_Connection_connect_4(const char *servernode, const char *serverdb, const char *username, const char *password);

	static void*          WINAPI My_SQLDBC_Connection_createStatement(void* pThis);
	static void*          WINAPI My_SQLDBC_Connection_createPreparedStatement(void* pThis);
	static void           WINAPI My_SQLDBC_Connection_releaseStatement_1(void* pThis, void* pStatement);

private:
	static std::wstring DoenforcerForStatementOnExecute(void* pStatement, const StatementSQL* pStateSQL);

private:
	//static HookManager* _hookMgr;

public:
	static SQLDBC_PreparedStatement_prepare_1_Fun   _SQLDBC_PreparedStatement_prepare_1_Old;
	static SQLDBC_PreparedStatement_prepare_2_Fun   _SQLDBC_PreparedStatement_prepare_2_Old;
	static SQLDBC_PreparedStatement_prepare_3_Fun   _SQLDBC_PreparedStatement_prepare_3_Old;
	static SQLDBC_Statement_execute_1_Fun           _SQLDBC_Statement_execute_1_Old;
	static SQLDBC_PreparedStatement_execute_2_Fun   _SQLDBC_PreparedStatement_execute_2_Old;
	static SQLDBC_Statement_getResultSet_1_Fun      _SQLDBC_Statement_getResultSet_1_Old;
	static SQLDBC_ResultSet_next_Fun                _SQLDBC_ResultSet_next_Old;
	static SQLDBC_ResultSet_getObject_1_Fun         _SQLDBC_ResultSet_getObject_1_Old;
	static SQLDBC_Statement_getConnection_Fun       _SQLDBC_Statement_getConnection_Old;
	static SQLDBC_Connection_connect_2_Fun  _SQLDBC_Connection_connect_2_Old;
	static SQLDBC_Connection_connect_3_Fun  _SQLDBC_Connection_connect_3_Old;
	static SQLDBC_Connection_connect_4_Fun  _SQLDBC_Connection_connect_4_Old;
	static SQLDBC_Connection_createStatement_Fun _SQLDBC_Connection_createStatement_Old;
	static SQLDBC_Connection_createPreparedStatement_Fun _SQLDBC_Connection_createPreparedStatement_Old;
	static SQLDBC_Connection_releaseStatement_1_Fun _SQLDBC_Connection_releaseStatement_1_Old;
	static SQLDBC_Statement_executeBatch_Fun _SQLDBC_Statement_executeBatch_Old;
	static SQLDBC_PreparedStatement_executeItab_1_Fun _SQLDBC_PreparedStatement_executeItab_1_Old;
	static SQLDBC_PreparedStatement_executeItab_2_Fun _SQLDBC_PreparedStatement_executeItab_2_Old;
	static SQLDBC_PreparedStatement_executeItab_3_Fun _SQLDBC_PreparedStatement_executeItab_3_Old;
	static SQLDBC_PreparedStatement_getItabReader_Fun _SQLDBC_PreparedStatement_getItabReader_Old;
	static SQLDBC_PreparedStatement_bindParameter_1_Fun _SQLDBC_PreparedStatement_bindParameter_1_Old;
	static SQLDBC_PreparedStatement_bindParameter_2_Fun _SQLDBC_PreparedStatement_bindParameter_2_Old;
};

