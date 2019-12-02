#include "stdafx.h"
#include "SqlDbcUserInfoMgr.h"
#include "MySqldbLib.h"
#include "log.h"


CDbcUserInfoMgr::CDbcUserInfoMgr()
{
}


CDbcUserInfoMgr::~CDbcUserInfoMgr()
{
}

const NxlDBCUserInfo* CDbcUserInfoMgr::GetUserInfoByLogonHdl(void* pConn, int nLogonHdl)
{
	//get info from usr41 by nLogonHDl
	std::unique_ptr<NxlDBCUserInfo> usr41Info(GetUserInfoFromUsr41ByTermID(pConn,nLogonHdl));
	if (NULL==usr41Info)
	{
		return NULL;
	}

	//get user info by userName
	const NxlDBCUserInfo* pUserInfo = GetUserInfoFromCache(usr41Info->_strName);

	//
	if (NULL==pUserInfo)
	{
		//create user Info, added attribute from usr41 and others where
		NxlDBCUserInfo* pNewInfo = new NxlDBCUserInfo();
		pNewInfo->_strName = usr41Info->_strName;
		pNewInfo->_Terminal = usr41Info->_Terminal;

		//added UserInfo to cache
		AddedUserInfoToCache(usr41Info->_strName, pNewInfo);

		pUserInfo = pNewInfo;
	}

	return pUserInfo;
}


NxlDBCUserInfo* CDbcUserInfoMgr::GetUserInfoFromCache(const std::wstring& wstrUserName)
{
	auto itUserInfo = _mapUserInfo.find(wstrUserName);
	if (itUserInfo != _mapUserInfo.end())
	{
		return itUserInfo->second;
	}
	return NULL;
}

void CDbcUserInfoMgr::AddedUserInfoToCache(const std::wstring& wstrUserName, NxlDBCUserInfo* pUserInfo) {
	_mapUserInfo[wstrUserName] = pUserInfo;
}

NxlDBCUserInfo* CDbcUserInfoMgr::GetUserInfoFromUsr41ByTermID(void* pConn, int logonHdl)
{
	NxlDBCUserInfo* pUserInfo = NULL;
	//create sql
	const char sqlFmt[] = "SELECT BNAME,TERMID,TERMINAL FROM USR41 WHERE TERMID=%d LIMIT 1;";
	char szSql[sizeof(sqlFmt) + 10];
	sprintf_s(szSql, sqlFmt, logonHdl);

	theLog->WriteLog(0, "GetUserInfoFromUsr41ByTermID, sql=%s", szSql);

	//create statement
	if (CMySqldbLib::_SQLDBC_Connection_createStatement_Old)
	{
		void* pStatement = CMySqldbLib::_SQLDBC_Connection_createStatement_Old(pConn);
		if (pStatement)
		{
			SQLDBC_Retcode rc = CMySqldbLib::_SQLDBC_Statement_execute_1_Old(pStatement, szSql);
			if (rc==SQLDBC_OK)
			{
				void* pResultSet = CMySqldbLib::_SQLDBC_Statement_getResultSet_1_Old(pStatement);

				rc = CMySqldbLib::_SQLDBC_ResultSet_next_Old(pResultSet);
				if (rc == SQLDBC_OK)
				{
					pUserInfo = new NxlDBCUserInfo();

					//get user name
					{
						wchar_t szUserName[255] = { 0 };
						SQLDBC_Length ind;
						CMySqldbLib::_SQLDBC_ResultSet_getObject_1_Old(pResultSet, 1, SQLDBC_HOSTTYPE_UCS2_SWAPPED, szUserName,
							&ind, sizeof(szUserName), SQLDBC_TRUE);
						pUserInfo->_strName = szUserName;
					}

					//get terminal
					{
						wchar_t szTerminal[255] = { 0 };
						SQLDBC_Length ind;
						CMySqldbLib::_SQLDBC_ResultSet_getObject_1_Old(pResultSet, 1, SQLDBC_HOSTTYPE_UCS2_SWAPPED, szTerminal,
							&ind, sizeof(szTerminal), SQLDBC_TRUE);
						pUserInfo->_Terminal = szTerminal;
					}
				}
			}

			CMySqldbLib::_SQLDBC_Connection_releaseStatement_1_Old(pConn, pStatement);
			pStatement = NULL;
		}
	}

	return pUserInfo;
}
