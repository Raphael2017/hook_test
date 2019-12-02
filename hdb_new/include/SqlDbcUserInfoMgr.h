#ifndef SQL_DBC_USER_INFO_H
#define SQL_DBC_USER_INFO_H


#include <map>
#include <string>
#include <shared_mutex>

struct NxlDBCUserInfo
{
	std::wstring _strName;
	std::wstring _Terminal; //192.168.187.21-sap-ts03
};

class CDbcUserInfoMgr
{
public:
	CDbcUserInfoMgr();
	~CDbcUserInfoMgr();

public:
	static CDbcUserInfoMgr* Instance()
	{
		static CDbcUserInfoMgr mgr;
		return &mgr;
	}


public:
	const NxlDBCUserInfo* GetUserInfoByLogonHdl(void* pConn, int logonHdl);


private:
	NxlDBCUserInfo* GetUserInfoFromUsr41ByTermID(void* pConn, int logonHdl);
	void AddedUserInfoToCache(const std::wstring& wstrUserName, NxlDBCUserInfo* pUserInfo);
	NxlDBCUserInfo* GetUserInfoFromCache(const std::wstring& wstrUserName);

private:
	std::map<std::wstring, NxlDBCUserInfo*> _mapUserInfo;
	//std::shared_mutex  _mutexUserInfo; //now _mapUserInfo only be accessed by one thread.

};

#endif //SQL_DBC_USER_INFO_H

