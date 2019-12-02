#include "MySqldbLib.h"
#include "log.h"
#include "SqlDbcUserInfoMgr.h"
//#include "QueryPolicy.h"
#include "PreparedStatementInfoMgr.h"
#include "MyDisp_Wrok.h"

HookManager* CMySqldbLib::_hookMgr = NULL;

SQLDBC_PreparedStatement_prepare_1_Fun   CMySqldbLib::_SQLDBC_PreparedStatement_prepare_1_Old=NULL;
SQLDBC_PreparedStatement_prepare_2_Fun   CMySqldbLib::_SQLDBC_PreparedStatement_prepare_2_Old=NULL;
SQLDBC_PreparedStatement_prepare_3_Fun   CMySqldbLib::_SQLDBC_PreparedStatement_prepare_3_Old=NULL;
SQLDBC_Statement_execute_1_Fun   CMySqldbLib::_SQLDBC_Statement_execute_1_Old = NULL;
SQLDBC_PreparedStatement_execute_2_Fun CMySqldbLib::_SQLDBC_PreparedStatement_execute_2_Old = NULL;
SQLDBC_Statement_getResultSet_1_Fun CMySqldbLib::_SQLDBC_Statement_getResultSet_1_Old = NULL;
SQLDBC_ResultSet_next_Fun CMySqldbLib::_SQLDBC_ResultSet_next_Old = NULL;
SQLDBC_ResultSet_getObject_1_Fun CMySqldbLib::_SQLDBC_ResultSet_getObject_1_Old = NULL;
SQLDBC_Statement_getConnection_Fun CMySqldbLib::_SQLDBC_Statement_getConnection_Old = NULL;
SQLDBC_Connection_connect_2_Fun CMySqldbLib::_SQLDBC_Connection_connect_2_Old = NULL;
SQLDBC_Connection_connect_3_Fun CMySqldbLib::_SQLDBC_Connection_connect_3_Old = NULL;
SQLDBC_Connection_connect_4_Fun CMySqldbLib::_SQLDBC_Connection_connect_4_Old = NULL;

SQLDBC_Connection_createStatement_Fun CMySqldbLib::_SQLDBC_Connection_createStatement_Old = NULL;
SQLDBC_Connection_createPreparedStatement_Fun CMySqldbLib::_SQLDBC_Connection_createPreparedStatement_Old = NULL;
SQLDBC_Connection_releaseStatement_1_Fun CMySqldbLib::_SQLDBC_Connection_releaseStatement_1_Old = NULL;
SQLDBC_Statement_executeBatch_Fun  CMySqldbLib::_SQLDBC_Statement_executeBatch_Old = NULL;

SQLDBC_PreparedStatement_executeItab_1_Fun CMySqldbLib::_SQLDBC_PreparedStatement_executeItab_1_Old = NULL;
SQLDBC_PreparedStatement_executeItab_2_Fun CMySqldbLib::_SQLDBC_PreparedStatement_executeItab_2_Old = NULL;
SQLDBC_PreparedStatement_executeItab_3_Fun CMySqldbLib::_SQLDBC_PreparedStatement_executeItab_3_Old = NULL;

SQLDBC_PreparedStatement_getItabReader_Fun CMySqldbLib::_SQLDBC_PreparedStatement_getItabReader_Old = NULL;
SQLDBC_PreparedStatement_bindParameter_1_Fun CMySqldbLib::_SQLDBC_PreparedStatement_bindParameter_1_Old = NULL;
SQLDBC_PreparedStatement_bindParameter_2_Fun CMySqldbLib::_SQLDBC_PreparedStatement_bindParameter_2_Old = NULL;


extern CLog* g_theLog;

CMySqldbLib::CMySqldbLib()
{
}


CMySqldbLib::~CMySqldbLib()
{
}

bool CMySqldbLib::AddedHook()
{
	const char* szLibDbcDllName = "libSQLDBCHDB.dll";

	//load libSQLDBCHDB.dll
	HANDLE hSQLDBCModule = LoadLibraryA(szLibDbcDllName);
	if (hSQLDBCModule==NULL)
	{
		g_theLog->WriteLog(0, "CMySqldbLib::AddedHook load libSQLDBCHDB failed.");
		return false;
	}
	g_theLog->WriteLog(0, "CMySqldbLib::AddedHook load libSQLDBCHDB Success.");

	//hook api
	_hookMgr = new HookManager();
	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_prepare_1_Old,
		                 My_SQLDBC_PreparedStatement_prepare_1,
		                  szLibDbcDllName,
		                 "?prepare@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD@Z");

	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_prepare_2_Old,
		My_SQLDBC_PreparedStatement_prepare_2,
		szLibDbcDllName,
		"?prepare@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBDW4Encoding@SQLDBC_StringEncodingType@@@Z");

	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_prepare_3_Old,
		My_SQLDBC_PreparedStatement_prepare_3,
		szLibDbcDllName,
		"?prepare@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD_JW4Encoding@SQLDBC_StringEncodingType@@@Z");

	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_bindParameter_1_Old,
		My_SQLDBC_PreparedStatement_bindParameter_1,
		szLibDbcDllName,
		"?bindParameter@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@IW4SQLDBC_HostType@@PEAXPEA_J_J_N@Z");
	
	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_bindParameter_2_Old,
		My_SQLDBC_PreparedStatement_bindParameter_2,
		szLibDbcDllName,
		"?bindParameter@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@IW4SQLDBC_HostType@@PEAXPEA_J_J33_N@Z");

	_hookMgr->AddHookItem((void**)&_SQLDBC_Statement_execute_1_Old,
		My_SQLDBC_Statement_execute_1,
		szLibDbcDllName,
		"?execute@SQLDBC_Statement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD@Z");

	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_execute_2_Old,
		My_SQLDBC_PreparedStatement_execute_2,
		szLibDbcDllName,
		"?execute@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");

	_hookMgr->AddHookItem((void**)&_SQLDBC_Statement_executeBatch_Old,
		My_SQLDBC_Statement_executeBatch,
		szLibDbcDllName,
		"?executeBatch@SQLDBC_Statement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");

	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_executeItab_1_Old,
		My_SQLDBC_PreparedStatement_executeItab_1,
		szLibDbcDllName,
		"?executeItab@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@AEAUSQLDBC_ItabDescriptor@@_N@Z");

	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_executeItab_2_Old,
		My_SQLDBC_PreparedStatement_executeItab_2,
		szLibDbcDllName,
		"?executeItab@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@AEAUSQLDBC_ShmDescriptor@@AEAUSQLDBC_ItabDescriptor@@I_N@Z");

	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_executeItab_3_Old,
		My_SQLDBC_PreparedStatement_executeItab_3,
		szLibDbcDllName,
		"?executeItab@SQLDBC_PreparedStatement@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEAX_N@Z");


	_hookMgr->AddHookItem((void**)&_SQLDBC_PreparedStatement_getItabReader_Old,
		My_SQLDBC_PreparedStatement_getItabReader,
		szLibDbcDllName,
		"?getItabReader@SQLDBC_PreparedStatement@SQLDBC@@QEAAPEAVSQLDBC_ItabReader@2@XZ");

	_hookMgr->AddHookItem((void**)&_SQLDBC_Statement_getResultSet_1_Old,
		My_SQLDBC_Statement_getResultSet_1,
		szLibDbcDllName,
		"?getResultSet@SQLDBC_Statement@SQLDBC@@QEAAPEAVSQLDBC_ResultSet@2@XZ");

	_hookMgr->AddHookItem((void**)&_SQLDBC_ResultSet_next_Old,
		My_SQLDBC_ResultSet_next,
		szLibDbcDllName,
		"?next@SQLDBC_ResultSet@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@XZ");

	_hookMgr->AddHookItem((void**)&_SQLDBC_ResultSet_getObject_1_Old,
		My_SQLDBC_ResultSet_getObject_1,
		szLibDbcDllName,
		"?getObject@SQLDBC_ResultSet@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@HW4SQLDBC_HostType@@PEAXPEA_J_J_N@Z");
		
	_hookMgr->AddHookItem((void**)&_SQLDBC_Statement_getConnection_Old,
		My_SQLDBC_Statement_getConnection,
		szLibDbcDllName,
		"?getConnection@SQLDBC_Statement@SQLDBC@@QEAAPEAVSQLDBC_Connection@2@XZ");

#if 0
	hookMgr->AddHookItem((void**)&_SQLDBC_Connection_connect_2_Old,
		My_SQLDBC_Connection_connect_2,
		szLibDbcDllName,
		"?connect@SQLDBC_Connection@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD_J010101W4Encoding@SQLDBC_StringEncodingType@@@Z");

	hookMgr->AddHookItem((void**)&_SQLDBC_Connection_connect_3_Old,
		My_SQLDBC_Connection_connect_3,
		szLibDbcDllName,
		"?connect@SQLDBC_Connection@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD000W4Encoding@SQLDBC_StringEncodingType@@@Z");

	hookMgr->AddHookItem((void**)&_SQLDBC_Connection_connect_4_Old,
		My_SQLDBC_Connection_connect_4,
		szLibDbcDllName,
		"?connect@SQLDBC_Connection@SQLDBC@@QEAA?AW4SQLDBC_Retcode@@PEBD000@Z");
#endif 

	_hookMgr->AddHookItem((void**)&_SQLDBC_Connection_createStatement_Old,
		My_SQLDBC_Connection_createStatement,
		szLibDbcDllName,
		"?createStatement@SQLDBC_Connection@SQLDBC@@QEAAPEAVSQLDBC_Statement@2@XZ");

	_hookMgr->AddHookItem((void**)&_SQLDBC_Connection_createPreparedStatement_Old,
		My_SQLDBC_Connection_createPreparedStatement,
		szLibDbcDllName,
		"?createPreparedStatement@SQLDBC_Connection@SQLDBC@@QEAAPEAVSQLDBC_PreparedStatement@2@XZ");

	_hookMgr->AddHookItem((void**)&_SQLDBC_Connection_releaseStatement_1_Old,
		My_SQLDBC_Connection_releaseStatement_1,
		szLibDbcDllName,
		"?releaseStatement@SQLDBC_Connection@SQLDBC@@QEAAXPEAVSQLDBC_Statement@2@@Z");


	_hookMgr->hook();


	//output log
	g_theLog->WriteLog(0, "CMySqldbLib::_SQLDBC_PreparedStatement_prepare_1_Old=%p", _SQLDBC_PreparedStatement_prepare_1_Old);
	g_theLog->WriteLog(0, "CMySqldbLib::_SQLDBC_PreparedStatement_prepare_2_Old=%p", _SQLDBC_PreparedStatement_prepare_2_Old);
	g_theLog->WriteLog(0, "CMySqldbLib::_SQLDBC_PreparedStatement_prepare_3_Old=%p", _SQLDBC_PreparedStatement_prepare_3_Old);
	g_theLog->WriteLog(0, "CMySqldbLib::_SQLDBC_Statement_execute_1_Old=%p", _SQLDBC_Statement_execute_1_Old);


	return true;

}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_prepare_1(void* pThis, char const * szSql)
{
	theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_prepare_1, sql=%s", (szSql==NULL ? "NULL" : szSql) );

	if (_SQLDBC_PreparedStatement_prepare_1_Old)
	{
		return (_SQLDBC_PreparedStatement_prepare_1_Old)(pThis, szSql);
	}

	return SQLDBC_NOT_OK;
}

struct PolicyMask {
	std::wstring _user_name;
	std::wstring _table_name;
	std::wstring _action;
	bool _has_mask;
	PolicyMask * next;
};


std::wstring GetTable(const std::wstring src)
{
	if (src.find(L"SELECT") == std::string::npos && src.find(L"select") == std::string::npos) {
		return L"";
	}

	const std::wstring BUT0BK = L"BUT0BK";
	const std::wstring BUT0BK_FIND = L"FROM \"BUT0BK\"";
	
	const std::wstring BNKA = L"BNKA";
	const std::wstring BNKA_FIND = L"FROM \"BNKA\"";

	const std::wstring T012 = L"T012";
	const std::wstring T012_FIND = L"FROM \"T012\"";

	const std::wstring BUT000 = L"BUT000";
	const std::wstring BUT000_FIND = L"FROM \"BUT000\"";


	std::wstring table_name;
	if (src.find(BUT0BK_FIND) != std::string::npos) {
		table_name = BUT0BK;
	}
	else if (src.find(BNKA_FIND) != std::string::npos) {
		table_name = BNKA;
	}
	else if (src.find(T012_FIND) != std::string::npos) {
		table_name = T012;
	}
	else if (src.find(BUT000_FIND) != std::string::npos) {
		table_name = BUT000;
	}


	return table_name;
}

std::wstring for_demo(const std::wstring& src, const std::wstring& user, PolicyMask *policys) {
	const std::wstring BUT0BK = L"BUT0BK", DFKKBPTAXNUM = L"DFKKBPTAXNUM", BANKL = L"\"BANKL\"", BANKL_MASK = L" '***' BANKL ", TAXTYPE = L"TAXTYPE", TAXTYPE_MASK = L" '***' TAXTYPE ", TAXNUM = L"TAXNUM", TAXNUM_MASK = L" '***' TAXNUM ";
	const std::wstring BUT0BK_WILD = L" MANDT ,PARTNER ,BKVID, BANKS, '***' BANKL, BANKN ,BKONT ,BKREF ,KOINH ,BKEXT ,XEZER ,ACCNAME ,MOVE_BKVID ,BK_VALID_FROM ,BK_VALID_TO,BK_MOVE_DATE ,IBAN ,BANK_GUID ,ACCOUNT_ID ,CHECK_DIGIT,ACCOUNT_TYPE ,BP_EEW_BUT0BK ";
	const std::wstring DFKKBPTAXNUM_WILD = L" CLIENT ,PARTNER , '***' TAXTYPE , '***' TAXNUM ,TAXNUMXL ";
	for (PolicyMask *it = policys; it != nullptr; it = it->next) {
		assert(it->_table_name == BUT0BK || it->_table_name == DFKKBPTAXNUM);
		assert(it->_has_mask);
	}

	if (src.find(L"SELECT") == std::string::npos && src.find(L"select") == std::string::npos) {
		return src;
	}
	std::wstring table_name;
	if (src.find(BUT0BK) != std::string::npos) {
		table_name = BUT0BK;
	}
	else if (src.find(DFKKBPTAXNUM) != std::string::npos) {
		table_name = DFKKBPTAXNUM;
	}
	else {
		return src;
	}

	int i = -1;
	for (PolicyMask *it = policys; it != nullptr; it = it->next) {
		if (it->_user_name == user && it->_table_name == table_name) {
			i = table_name == BUT0BK ? 0 : 1;
			break;
		}
	}

	if (i == -1) return src;

	std::wstring r = src;
	if (i == 0) {       // BUT0BK
		auto find_wild = r.find(L"*");
// 		if (find_wild != std::string::npos) {
// 			r.replace(find_wild, 1, BUT0BK_WILD);
// 		}
// 		else 
		if ((find_wild = r.find(BANKL)) != std::string::npos) {
			r.replace(find_wild, BANKL.length(), BANKL_MASK);
		}
	}
	else {            // DFKKBPTAXNUM
		assert(i == 1);
		auto find_wild = r.find(L"*");
		if (find_wild != std::string::npos) {
			r.replace(find_wild, 1, DFKKBPTAXNUM_WILD);
		}
		else {
			if ((find_wild = r.find(TAXTYPE)) != std::string::npos) {
				r.replace(find_wild, TAXTYPE.length(), TAXTYPE_MASK);
			}
			if ((find_wild = r.find(TAXNUM)) != std::string::npos && r[find_wild - 1] != L'P') {
				r.replace(find_wild, TAXNUM.length(), TAXNUM_MASK);
			}
		}
	}
	return r;
}

std::wstring MaskSQLField(const std::wstring& src, const MaskFields& mf)
{
	std::wstring r = src;
	size_t find_wild;

	std::wstring strMaksValueFmt =L"'" + mf._c2character + L"' ";

	auto fields = mf._fields;
	auto itField = fields.begin();
	while (itField != fields.end())
	{
		std::wstring fname = L"\"" + *itField + L"\"";

		if ((find_wild = r.find(fname)) != std::string::npos) {

			std::wstring fnameMask = strMaksValueFmt + fname;
			r.replace(find_wild, fname.length(), fnameMask.c_str());
		}
		itField++;
	}

	return r;
}


std::wstring MaskSQLFieldWithCondition(const std::wstring& src, const MaskFields& mf)
{
	std::wstring r = src;
	size_t find_wild;

	std::wstring strMaksValueFmt = L"CASE WHEN %s THEN '***' ELSE %s END AS %s ";// L"'" + mf._c2character + L"' ";

	auto fields = mf._fields;
	auto itField = fields.begin();
	while (itField != fields.end() )
	{
		std::wstring fname = L"\"" + *itField + L"\"";

		if ((find_wild = r.find(fname)) != std::string::npos) {
			
			wchar_t* pszFnameMask = new wchar_t[src.length() + 1024];

			//construct conditions
			std::wstring strConditions;
			if (mf._condi.empty())
			{
				strConditions = L"1=1";
			}
			else
			{
				auto itCon = mf._condi.begin();
				while (itCon != mf._condi.end())
				{
					if (!strConditions.empty())
					{
						strConditions += L" OR ";
					}
					strConditions += itCon->_field + itCon->_op + L"'" + itCon->_value + L"'";
					itCon++;
				}
			}
			

			wsprintfW(pszFnameMask, strMaksValueFmt.c_str(), strConditions.c_str(), fname.c_str(), fname.c_str());
			std::wstring fnameMask = pszFnameMask;
			delete[] pszFnameMask;
			pszFnameMask = NULL;

			r.replace(find_wild, fname.length() , fnameMask.c_str() );
		}


		itField++;
	}

	return r;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_prepare_2(void* pThis, char const * szSql,  SQLDBC_StringEncodingType::Encoding encoding)
{
	SQLDBC_Retcode ret = SQLDBC_NOT_OK;
	theLog->WriteLog(0, L"My_SQLDBC_PreparedStatement_prepare_2, pThis=%p, sql=%s, encod=%d",
		             pThis, (szSql == NULL ? L"NULL" : (const wchar_t*)szSql), encoding);

	g_thePreparedStatementMgr->AddedStatementSQL(pThis, szSql, encoding);

	//Init pc
	static long lPolicyInit = 0;
	if (!InterlockedCompareExchange(&lPolicyInit, 1, 0))
	{
		std::string strJPCHost = "https://edrm-jpc.azure.cloudaz.net";
		std::string strJPCPort = "443";
		std::string strOAuthHost = "https://edrm-cc.azure.cloudaz.net";
		std::string strOAuthPort = "443";
		std::string strClientId = "apiclient";
		std::string strClientSecure = "ItisBlue888!";
		bool bPCInit = QueryCloudAZInit(strJPCHost.c_str(), strJPCPort.c_str(), strOAuthHost.c_str(), strOAuthPort.c_str(), strClientId.c_str(), strClientSecure.c_str(), 1);
		g_theLog->WriteLog(0, "QueryCloudAZInit result:%d", bPCInit);
	}
	
	ret = (_SQLDBC_PreparedStatement_prepare_2_Old)(pThis,  szSql, encoding);

	return ret;
}


SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_prepare_3(void* pThis, char const * szSql, __int64 sqlLength,  SQLDBC_StringEncodingType::Encoding encoding)
{
	theLog->WriteLog(0, L"My_SQLDBC_PreparedStatement_prepare_3, this=%p, sql=%s",pThis,  (szSql == NULL ? L"NULL" : (const wchar_t*)szSql) );
	if (_SQLDBC_PreparedStatement_prepare_3_Old)
	{
		return (_SQLDBC_PreparedStatement_prepare_3_Old)(pThis, szSql, sqlLength, encoding);
	}

	return SQLDBC_NOT_OK;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_Statement_execute_1(void* pThis, char const * szSql)
{
	theLog->WriteLog(0, "My_SQLDBC_Statement_execute_1, this=%p, sql:%s", pThis, szSql);
	if (_SQLDBC_Statement_execute_1_Old)
	{
		return _SQLDBC_Statement_execute_1_Old(pThis, szSql);
	}

	return SQLDBC_NOT_OK;
}


SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_execute_2(void* pThis)
{
	theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_execute_2, this=%p", pThis);

	//enforcer, find sql and param
	const StatementSQL* pStateSQL = g_thePreparedStatementMgr->FindStatementSQL(pThis);
	if (NULL != pStateSQL)
	{
		std::wstring strNewSql = DoenforcerForStatementOnExecute(pThis, pStateSQL);

		//do enforcer
		if ((!strNewSql.empty()) && (strNewSql.compare(pStateSQL->_wstrSQL) != 0))
		{
			theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_execute_2 do enforcer.");

			//create new object of PreparedStatement
			//get connection
			void* pConn = _SQLDBC_Statement_getConnection_Old(pThis);
			void* pEnforceStatement = _SQLDBC_Connection_createPreparedStatement_Old(pConn);
			if (pEnforceStatement)
			{
				theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_execute_2 create enforce statement success");

				//prepare sql
				SQLDBC_Retcode enforceRet = _SQLDBC_PreparedStatement_prepare_2_Old(pEnforceStatement, (const char*)strNewSql.c_str(), pStateSQL->_encoding);
				theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_execute_2 prepare new sql result:%d", enforceRet);
				if (enforceRet == SQLDBC_OK)
				{
					//bind parameters
					const LIST_STATEMENT_PARAM* pStateParams = g_thePreparedStatementMgr->FindStatementParam(pThis);
					if (pStateParams != NULL)
					{
						auto itParam = pStateParams->begin();
						while (itParam != pStateParams->end())
						{
							auto pParam = *itParam;
							enforceRet = _SQLDBC_PreparedStatement_bindParameter_1_Old(pEnforceStatement, pParam->_Index,
								pParam->_Type, pParam->_paramAddr, pParam->_LengthIndicator, pParam->_Size, pParam->_Terminate);

							theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_execute_2 bind param, index:%d, result:%d", pParam->_Index, enforceRet);
							itParam++;
						}
					}

					//execute
					enforceRet = _SQLDBC_PreparedStatement_execute_2_Old(pEnforceStatement);
					theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_execute_2 execute enforce sql result:%d", enforceRet);
					if (enforceRet == SQLDBC_OK)
					{
						//
						g_thePreparedStatementMgr->AddedEnforceStatement(pThis, pEnforceStatement);
						theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_execute_2 added enforcer statement");
					}

				}
			}
		}
		else
		{
			//remove enforcer statement
			theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_execute_2 no need do enforcer.");

			g_thePreparedStatementMgr->RemoveEnforceStatement(pThis);
		}

	}

	if (_SQLDBC_PreparedStatement_execute_2_Old)
	{	
		return _SQLDBC_PreparedStatement_execute_2_Old(pThis);
	}
	return SQLDBC_NOT_OK;
}

void* WINAPI CMySqldbLib::My_SQLDBC_Statement_getResultSet_1(void* pThis)
{
	theLog->WriteLog(0, "My_SQLDBC_Statement_getResultSet_1, this=%p", pThis);

	if (_SQLDBC_Statement_getResultSet_1_Old)
	{
		void* pEnforceStatement = g_thePreparedStatementMgr->FindEnforcerStatement(pThis);
		if (pEnforceStatement)
		{
			void* pRet = _SQLDBC_Statement_getResultSet_1_Old(pEnforceStatement);
			g_theLog->WriteLog(0, "My_SQLDBC_Statement_getResultSet_1 pthis=%p, return enforcre void*=%p", pThis, pRet);
			return pRet;
		}
		else
		{
			return _SQLDBC_Statement_getResultSet_1_Old(pThis);
		}
		
	}
	return NULL;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_ResultSet_next(void* pThis)
{
	if (_SQLDBC_ResultSet_next_Old)
	{
		return _SQLDBC_ResultSet_next_Old(pThis);
	}
	return SQLDBC_NOT_OK;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_ResultSet_getObject_1(void* pThis, int Index, 
	                               SQLDBC_HostType Type, void * paramAddr, __int64 * LengthIndicator,
	                               __int64 Size, bool Terminate /* = SQLDBC_TRUE */)
{
	if (_SQLDBC_ResultSet_getObject_1_Old)
	{
		return _SQLDBC_ResultSet_getObject_1_Old(pThis, Index, Type, paramAddr, LengthIndicator, Size, Terminate);
	}

	return SQLDBC_NOT_OK;
}

void* WINAPI CMySqldbLib::My_SQLDBC_Statement_getConnection(void* pThis)
{
	if (_SQLDBC_Statement_getConnection_Old)
	{
		return _SQLDBC_Statement_getConnection_Old(pThis);
	}
	return NULL;
}


SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_Connection_connect_2(const char *servernode,
	                  SQLDBC_Length servernodeLength, const char *serverdb,
	                  SQLDBC_Length serverdbLength, const char *username, 
	                  SQLDBC_Length usernameLength, const char *password,
	                  SQLDBC_Length passwordLength, const SQLDBC_StringEncoding userpwdEncoding)
{
	g_theLog->WriteLog(0, "My_SQLDBC_Connection_connect_2");
	if (_SQLDBC_Connection_connect_2_Old)
	{
		return _SQLDBC_Connection_connect_2_Old(servernode, servernodeLength, serverdb, serverdbLength,
			username, usernameLength, password, passwordLength, userpwdEncoding);
	}

	return SQLDBC_NOT_OK;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_Connection_connect_3(const char *servernode,
	                  const char *serverdb, const char *username, const char *password,
	                  const SQLDBC_StringEncoding userpwdEncoding)
{
	g_theLog->WriteLog(0, "My_SQLDBC_Connection_connect_3");
	if (_SQLDBC_Connection_connect_3_Old)
	{
		return _SQLDBC_Connection_connect_3_Old(servernode, serverdb, username, password, userpwdEncoding);
	}

	return SQLDBC_NOT_OK;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_Connection_connect_4(const char *servernode,
	                  const char *serverdb, const char *username, const char *password)
{
	g_theLog->WriteLog(0, "My_SQLDBC_Connection_connect_4");
	if (_SQLDBC_Connection_connect_4_Old)
	{
		return _SQLDBC_Connection_connect_4_Old(servernode, serverdb, username, password);
	}

	return SQLDBC_NOT_OK;
}

void* WINAPI CMySqldbLib::My_SQLDBC_Connection_createStatement(void* pThis)
{
	if (_SQLDBC_Connection_createStatement_Old)
	{
		return _SQLDBC_Connection_createStatement_Old(pThis);
	}
	return NULL;
}
void  WINAPI CMySqldbLib::My_SQLDBC_Connection_releaseStatement_1(void* pThis, void* pStatement)
{
	g_theLog->WriteLog(0, "My_SQLDBC_Connection_releaseStatement_1:pConn:%p, pState:%p", pThis, pStatement);

	//remove information
	g_thePreparedStatementMgr->ReleaseStatement(pStatement);
	
	if (_SQLDBC_Connection_releaseStatement_1_Old)
	{
		return _SQLDBC_Connection_releaseStatement_1_Old(pThis, pStatement);
	}

}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_Statement_executeBatch(void* pThis)
{
	g_theLog->WriteLog(0, "My_SQLDBC_Statement_executeBatch,pState:%p", pThis);
	if (_SQLDBC_Statement_executeBatch_Old)
	{
		return _SQLDBC_Statement_executeBatch_Old(pThis);
	}

	return SQLDBC_NOT_OK;
}


SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_executeItab_1(void* pThis, void* pItabDesc, bool b)
{
	g_theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_1,pState:%p", pThis);

	if (_SQLDBC_PreparedStatement_executeItab_1_Old)
	{
		return _SQLDBC_PreparedStatement_executeItab_1_Old(pThis, pItabDesc, b);
	}

	return SQLDBC_NOT_OK;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_executeItab_2(void* pThis, void* pShmDesc, void* pItabDesc, unsigned int nParam, bool bParam)
{
	g_theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_2,pState:%p", pThis);

	if (_SQLDBC_PreparedStatement_executeItab_2_Old)
	{
		return _SQLDBC_PreparedStatement_executeItab_2_Old(pThis, pShmDesc, pItabDesc, nParam, bParam);
	}

	return SQLDBC_NOT_OK;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_executeItab_3(void* pThis, void * pVoid, bool bValue)
{
	g_theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3,pState:%p, 2p:%p, 3p:%d", pThis, pVoid, bValue);

	//enforcer, find sql and param
	const StatementSQL* pStateSQL = g_thePreparedStatementMgr->FindStatementSQL(pThis);
	if (NULL != pStateSQL)
	{
		std::wstring strNewSql = DoenforcerForStatementOnExecute(pThis, pStateSQL);

		//do enforcer
		if (strNewSql.compare(pStateSQL->_wstrSQL) != 0)
		{
			theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 do enforcer.");

			//create new object of PreparedStatement
			//get connection
			void* pConn = _SQLDBC_Statement_getConnection_Old(pThis);
			void* pEnforceStatement = _SQLDBC_Connection_createPreparedStatement_Old(pConn);
			if (pEnforceStatement)
			{
				theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 create enforce statement success");

				//prepare sql
				SQLDBC_Retcode enforceRet = _SQLDBC_PreparedStatement_prepare_2_Old(pEnforceStatement, (const char*)strNewSql.c_str(), pStateSQL->_encoding);
				theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 prepare new sql result:%d", enforceRet);
				if (enforceRet == SQLDBC_OK)
				{
					//bind parameters
					const LIST_STATEMENT_PARAM* pStateParams = g_thePreparedStatementMgr->FindStatementParam(pThis);
					if (pStateParams != NULL)
					{
						auto itParam = pStateParams->begin();
						while (itParam != pStateParams->end())
						{
							auto pParam = *itParam;
							enforceRet = _SQLDBC_PreparedStatement_bindParameter_1_Old(pEnforceStatement, pParam->_Index,
								pParam->_Type, pParam->_paramAddr, pParam->_LengthIndicator, pParam->_Size, pParam->_Terminate);

							theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 bind param, index:%d, result:%d", pParam->_Index, enforceRet);
							itParam++;
						}
					}

					//execute
					enforceRet = _SQLDBC_PreparedStatement_executeItab_3_Old(pEnforceStatement, pVoid, bValue);
					theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 bind param, execute enforce sql result:%d", enforceRet);
					if (enforceRet == SQLDBC_OK)
					{
						//
						g_thePreparedStatementMgr->AddedEnforceStatement(pThis, pEnforceStatement);
						theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 added enforcer statement");
					}

				}
			}
		}
		else
		{
			//remove enforcer statement
			theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_executeItab_3 no need do enforcer.");

			g_thePreparedStatementMgr->RemoveEnforceStatement(pThis);
		}

	}
	

	


	//call next
	if (_SQLDBC_PreparedStatement_executeItab_3_Old)
	{
		return  _SQLDBC_PreparedStatement_executeItab_3_Old(pThis, pVoid, bValue);
	}

	return SQLDBC_NOT_OK;
}

void* CMySqldbLib::My_SQLDBC_PreparedStatement_getItabReader(void* pThis)
{
	//g_theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_getItabReader pthis=%p", pThis);

	if (_SQLDBC_PreparedStatement_getItabReader_Old)
	{
		void* pEnforceStatement = g_thePreparedStatementMgr->FindEnforcerStatement(pThis);
		if (pEnforceStatement)
		{
			void* pRet =  _SQLDBC_PreparedStatement_getItabReader_Old(pEnforceStatement);
			g_theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_getItabReader pthis=%p, return enforcre void*=%p", pThis, pRet);
			return pRet;
		}
		else
		{
			return _SQLDBC_PreparedStatement_getItabReader_Old(pThis);
		}
		
	}

	return NULL;
}


SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_bindParameter_1(void* pThis,
	const SQLDBC_UInt2 Index,
	const SQLDBC_HostType Type, void * paramAddr, SQLDBC_Length * LengthIndicator,
	const SQLDBC_Length Size, const SQLDBC_Bool Terminate /* = SQLDBC_TRUE */)
{
	//theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_bindParameter_1, pThis:%p, Index=%d, type=%d, leng=%d, lengPtr=%d, size=%d",
	//	pThis, Index, Type, *LengthIndicator, LengthIndicator, Size );

	g_thePreparedStatementMgr->AddedStatementParam(pThis, Index, Type, paramAddr, LengthIndicator, Size, Terminate);

	if (_SQLDBC_PreparedStatement_bindParameter_1_Old)
	{
		return _SQLDBC_PreparedStatement_bindParameter_1_Old(pThis, Index, Type, paramAddr, LengthIndicator, Size, Terminate);
	}
	return SQLDBC_NOT_OK;
}

SQLDBC_Retcode WINAPI CMySqldbLib::My_SQLDBC_PreparedStatement_bindParameter_2(void* pThis,
	const SQLDBC_UInt2 Index, const SQLDBC_HostType Type, void * paramAddr,
	SQLDBC_Length * LengthIndicator, const SQLDBC_Length Size, SQLDBC_Length para6,
	SQLDBC_Length Para7, const SQLDBC_Bool Terminate /* = SQLDBC_TRUE */)
{
	theLog->WriteLog(0, "My_SQLDBC_PreparedStatement_bindParameter_2, pThis:%p", pThis);
	if (_SQLDBC_PreparedStatement_bindParameter_2_Old)
	{
		return _SQLDBC_PreparedStatement_bindParameter_2_Old(pThis, Index, Type, paramAddr, LengthIndicator,
			Size, para6, Para7, Terminate);
	}
	return SQLDBC_NOT_OK;
}

void* WINAPI CMySqldbLib::My_SQLDBC_Connection_createPreparedStatement(void* pThis)
{
	if (_SQLDBC_Connection_createPreparedStatement_Old)
	{
		return _SQLDBC_Connection_createPreparedStatement_Old(pThis);
	}
	return NULL;
}


void CMySqldbLib::ReleaseEnforceStatement(void* pStatement)
{
	//get connection
	void* pConnection = NULL;
	if (_SQLDBC_Statement_getConnection_Old)
	{
		pConnection = _SQLDBC_Statement_getConnection_Old(pStatement);
	}

	//release
	if (pConnection && _SQLDBC_Connection_releaseStatement_1_Old)
	{
		_SQLDBC_Connection_releaseStatement_1_Old(pConnection, pStatement);
		theLog->WriteLog(0, "Release enforce statement");
	}
}

std::wstring CMySqldbLib::DoenforcerForStatementOnExecute(void* pStatement, const StatementSQL* pStateSQL)
{
	std::wstring strNewSql;

	if (pStateSQL != NULL)
	{
		//get from table
		const std::wstring& strSql = pStateSQL->_wstrSQL;
		std::wstring strTableName = GetTable(strSql);
		theLog->WriteLog(0, L"DoenforcerForStatementOnExecute table name:%s, threadID:%d", strTableName.c_str(), GetCurrentThreadId());

		if (!strTableName.empty())
		{
			//get connection
			void* pConn = _SQLDBC_Statement_getConnection_Old(pStatement);

			//get request buf
			const MY_REQUEST_BUF* pCurrentRequest = g_theMyDW->GetCurrentRequest();
			const MY_DP_SESSION_INFO& sessionInfo = pCurrentRequest->_sender._sessionInfo;
			theLog->WriteLog(0, "DoenforcerForStatementOnExecute sessionInfo._logonHdl=%d, sessionInfo._logonId=%d, sessionInfo._sessionHdl=%d",
				sessionInfo._logonHdl, sessionInfo._logonId, sessionInfo._sessionHdl);

			//get dbcContext
			const NxlDBCUserInfo* pUserInfo = CDbcUserInfoMgr::Instance()->GetUserInfoByLogonHdl(pConn, pCurrentRequest->_sender._sessionInfo._logonHdl);

			//get user name
			std::string strUserName;
			if (pUserInfo)
			{
				strUserName = CommonFun::ConvertUtf16ToUtf8(pUserInfo->_strName.c_str());
			}
			theLog->WriteLog(0, "DoenforcerForStatementOnExecute user name:%s", strUserName.c_str());


			
			MaskFields mf;
			std::map<std::string, std::string> resAttr;
			std::list<std::string> lstColumns;
			if (_wcsicmp(strTableName.c_str(), L"but0bk") == 0) {
				lstColumns = { "MANDT" , "PARTNER" , "BKVID" , "BANKS" , "BANKL" , "BANKN" , "BKONT" , "BKREF" , "KOINH" , "BKEXT" , "XEZER" , "ACCNAME" , "MOVE_BKVID" , "PROTECT" , "BK_VALID_FROM" , "BK_VALID_TO" , "BK_MOVE_DATE" , "IBAN" , "BANK_GUID" , "ACCOUNT_ID" , "CHECK_DIGIT" , "ACCOUNT_TYPE" , "BP_EEW_BUT0BK" };
			}
			else if (_wcsicmp(strTableName.c_str(), L"but000") == 0)
			{
				lstColumns = { "CLIENT" , "PARTNER" , "TYPE" , "BPKIND" , "BU_GROUP" , "BPEXT" , "BU_SORT1" , "BU_SORT2" , "SOURCE" , "TITLE" , "XDELE" , "XBLCK" , "AUGRP" , "TITLE_LET" , "BU_LOGSYS" , "CONTACT" , "NOT_RELEASED" , "NOT_LG_COMPETENT" , "PRINT_MODE" , "BP_EEW_DUMMY" , "RATE" , "NAME_ORG1" , "NAME_ORG2" , "NAME_ORG3" , "NAME_ORG4" , "LEGAL_ENTY" , "IND_SECTOR" , "LEGAL_ORG" , "FOUND_DAT" , "LIQUID_DAT" , "LOCATION_1" , "LOCATION_2" , "LOCATION_3" , "NAME_LAST" , "NAME_FIRST" , "NAME_LST2" , "NAME_LAST2" , "NAMEMIDDLE" , "TITLE_ACA1" , "TITLE_ACA2" , "TITLE_ROYL" , "PREFIX1" , "PREFIX2" , "NAME1_TEXT" , "NICKNAME" , "INITIALS" , "NAMEFORMAT" , "NAMCOUNTRY" , "LANGU_CORR" , "XSEXM" , "XSEXF" , "BIRTHPL" , "MARST" , "EMPLO" , "JOBGR" , "NATIO" , "CNTAX" , "CNDSC" , "PERSNUMBER" , "XSEXU" , "XUBNAME" , "BU_LANGU" , "GENDER" , "BIRTHDT" , "DEATHDT" , "PERNO" , "CHILDREN" , "MEM_HOUSE" , "BIRTHDT_STATUS" , "PARTGRPTYP" , "NAME_GRP1" , "NAME_GRP2" , "MC_NAME1" , "MC_NAME2" , "CRUSR" , "CRDAT" , "CRTIM" , "CHUSR" , "CHDAT" , "CHTIM" , "PARTNER_GUID" , "ADDRCOMM" , "TD_SWITCH" , "IS_ORG_CENTRE" , "DB_KEY" , "VALID_FROM" , "VALID_TO" , "XPCPT" , "NATPERS" , "MILVE" , "NUC_SEC" , "PAR_REL" , "BP_SORT" , "KBANKS" , "KBANKL" };
			}
			else if (_wcsicmp(strTableName.c_str(), L"BNKA") == 0)
			{
				//lstColumns = { "CLIENT" , "PARTNER" , "TYPE" , "BPKIND" , "BU_GROUP" , "BPEXT" , "BU_SORT1" , "BU_SORT2" , "SOURCE" , "TITLE" , "XDELE" , "XBLCK" , "AUGRP" , "TITLE_LET" , "BU_LOGSYS" , "CONTACT" , "NOT_RELEASED" , "NOT_LG_COMPETENT" , "PRINT_MODE" , "BP_EEW_DUMMY" , "RATE" , "NAME_ORG1" , "NAME_ORG2" , "NAME_ORG3" , "NAME_ORG4" , "LEGAL_ENTY" , "IND_SECTOR" , "LEGAL_ORG" , "FOUND_DAT" , "LIQUID_DAT" , "LOCATION_1" , "LOCATION_2" , "LOCATION_3" , "NAME_LAST" , "NAME_FIRST" , "NAME_LST2" , "NAME_LAST2" , "NAMEMIDDLE" , "TITLE_ACA1" , "TITLE_ACA2" , "TITLE_ROYL" , "PREFIX1" , "PREFIX2" , "NAME1_TEXT" , "NICKNAME" , "INITIALS" , "NAMEFORMAT" , "NAMCOUNTRY" , "LANGU_CORR" , "XSEXM" , "XSEXF" , "BIRTHPL" , "MARST" , "EMPLO" , "JOBGR" , "NATIO" , "CNTAX" , "CNDSC" , "PERSNUMBER" , "XSEXU" , "XUBNAME" , "BU_LANGU" , "GENDER" , "BIRTHDT" , "DEATHDT" , "PERNO" , "CHILDREN" , "MEM_HOUSE" , "BIRTHDT_STATUS" , "PARTGRPTYP" , "NAME_GRP1" , "NAME_GRP2" , "MC_NAME1" , "MC_NAME2" , "CRUSR" , "CRDAT" , "CRTIM" , "CHUSR" , "CHDAT" , "CHTIM" , "PARTNER_GUID" , "ADDRCOMM" , "TD_SWITCH" , "IS_ORG_CENTRE" , "DB_KEY" , "VALID_FROM" , "VALID_TO" , "XPCPT" , "NATPERS" , "MILVE" , "NUC_SEC" , "PAR_REL" , "BP_SORT" , "KBANKS" , "KBANKL" };
			}
			else if (_wcsicmp(strTableName.c_str(), L"T012") == 0)
			{
				//lstColumns = { "CLIENT" , "PARTNER" , "TYPE" , "BPKIND" , "BU_GROUP" , "BPEXT" , "BU_SORT1" , "BU_SORT2" , "SOURCE" , "TITLE" , "XDELE" , "XBLCK" , "AUGRP" , "TITLE_LET" , "BU_LOGSYS" , "CONTACT" , "NOT_RELEASED" , "NOT_LG_COMPETENT" , "PRINT_MODE" , "BP_EEW_DUMMY" , "RATE" , "NAME_ORG1" , "NAME_ORG2" , "NAME_ORG3" , "NAME_ORG4" , "LEGAL_ENTY" , "IND_SECTOR" , "LEGAL_ORG" , "FOUND_DAT" , "LIQUID_DAT" , "LOCATION_1" , "LOCATION_2" , "LOCATION_3" , "NAME_LAST" , "NAME_FIRST" , "NAME_LST2" , "NAME_LAST2" , "NAMEMIDDLE" , "TITLE_ACA1" , "TITLE_ACA2" , "TITLE_ROYL" , "PREFIX1" , "PREFIX2" , "NAME1_TEXT" , "NICKNAME" , "INITIALS" , "NAMEFORMAT" , "NAMCOUNTRY" , "LANGU_CORR" , "XSEXM" , "XSEXF" , "BIRTHPL" , "MARST" , "EMPLO" , "JOBGR" , "NATIO" , "CNTAX" , "CNDSC" , "PERSNUMBER" , "XSEXU" , "XUBNAME" , "BU_LANGU" , "GENDER" , "BIRTHDT" , "DEATHDT" , "PERNO" , "CHILDREN" , "MEM_HOUSE" , "BIRTHDT_STATUS" , "PARTGRPTYP" , "NAME_GRP1" , "NAME_GRP2" , "MC_NAME1" , "MC_NAME2" , "CRUSR" , "CRDAT" , "CRTIM" , "CHUSR" , "CHDAT" , "CHTIM" , "PARTNER_GUID" , "ADDRCOMM" , "TD_SWITCH" , "IS_ORG_CENTRE" , "DB_KEY" , "VALID_FROM" , "VALID_TO" , "XPCPT" , "NATPERS" , "MILVE" , "NUC_SEC" , "PAR_REL" , "BP_SORT" , "KBANKS" , "KBANKL" };
			}


			resAttr["table"] = CommonFun::ConvertUtf16ToUtf8(strTableName.c_str());
			QueryAZ(strUserName.c_str(), resAttr, lstColumns, mf);

			//do mask
			strNewSql = MaskSQLFieldWithCondition(strSql, mf);

			std::wstring wstrField;
			std::vector<std::wstring>::iterator itField = mf._fields.begin();
			while (itField != mf._fields.end())
			{
				wstrField += itField->c_str();
				wstrField += L",";
				itField++;
			}

			std::wstring wstrCondi;
			auto itCondi = mf._condi.begin();
			while (itCondi != mf._condi.end())
			{
				wstrCondi += itCondi->_field + itCondi->_op + itCondi->_value + L",";
				itCondi++;
			}
			theLog->WriteLog(0, L"DoenforcerForStatementOnExecute mask field:%s, condi:%s, new sql:%s", wstrField.c_str(), wstrCondi.c_str(), strNewSql.c_str());		
		}
	}

	return strNewSql;
}