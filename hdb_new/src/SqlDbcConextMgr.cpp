#include "stdafx.h"
#include "SqlDbcConextMgr.h"
#include "CriticalSectionLock.h"


CSqlDbcUserInfoMgr::CSqlDbcUserInfoMgr()
{
	InitializeCriticalSection(&m_csDBCContext);
}


CSqlDbcUserInfoMgr::~CSqlDbcUserInfoMgr()
{
}

const NxlDBCUserInfo* CSqlDbcUserInfoMgr::GetDBCContextByConnection(void* pConn)
{
	CriticalSectionLock csLock(&m_csDBCContext);

	//test
	return NULL;
	//

	auto itContext = m_mapConnection2DbcContext.find(pConn);
	if (itContext != m_mapConnection2DbcContext.end() )
	{
		return itContext->second;
	}
	return NULL;
}

void CSqlDbcUserInfoMgr::AddedDBCContext(void* pConn, NxlDBCUserInfo* ctxt)
{
	CriticalSectionLock csLock(&m_csDBCContext);
	m_mapConnection2DbcContext[pConn] = ctxt;
}
