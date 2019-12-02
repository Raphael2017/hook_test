#pragma once
#include <map>
#include <string>

struct NxlDBCContext
{
	void* _pDbcConnection;
	std::string _strName;
};

class CSqlDbcConextMgr
{
public:
	CSqlDbcConextMgr();
	~CSqlDbcConextMgr();

public:
	static CSqlDbcConextMgr* Instance()
	{
		static CSqlDbcConextMgr mgr;
		return &mgr;
	}


public:
	const NxlDBCContext* GetDBCContextByConnection(void* pConn);
	void AddedDBCContext(void* pConn, NxlDBCContext* ctxt);


private:
	std::map<void*, NxlDBCContext*> m_mapConnection2DbcContext;
	CRITICAL_SECTION m_csDBCContext;

};

