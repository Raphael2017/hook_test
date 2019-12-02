#ifndef PREPARED_STATEMENT_INFO_MGR_H
#define PREPARED_STATEMENT_INFO_MGR_H

#include "SQLDBC_Types.h"
#include <list>
#include <map>
#include <mutex>
#include <string.h>
#include "CommonFun.h"

typedef unsigned char BYTE;

class StatementParam
{
public:
	StatementParam(SQLDBC_UInt2 Index, SQLDBC_HostType Type, void * paramAddr, SQLDBC_Length* LengthIndicator,
		SQLDBC_Length Size, SQLDBC_Bool Terminate)
	{
		_Index = Index;
		_Type = Type;
		_LengthIndicator = LengthIndicator;
		_Size = Size;
		_Terminate = Terminate;

		_paramAddr = new BYTE[Size];
		memcpy(_paramAddr, paramAddr, Size);
	}

	~StatementParam()
	{
		delete[] _paramAddr;
		_paramAddr = NULL;
	}

	SQLDBC_UInt2 _Index;
	SQLDBC_HostType _Type;
	void * _paramAddr;
	SQLDBC_Length* _LengthIndicator;
	SQLDBC_Length _Size;
	SQLDBC_Bool _Terminate;
};
typedef std::list<StatementParam*> LIST_STATEMENT_PARAM;

class StatementSQL
{
public:
	StatementSQL(char const * szSql, SQLDBC_StringEncodingType::Encoding encoding)
	{
		_encoding = SQLDBC_StringEncodingType::UCS2Swapped; //we convert all encoding to SQLDBC_StringEncodingType::UCS2
		if (encoding== SQLDBC_StringEncodingType::Ascii)
		{
			_wstrSQL = CommonFun::ConvertUtf8ToUtf16(szSql);
		}
		else if (encoding==SQLDBC_StringEncodingType::UCS2Swapped)
		{
			_wstrSQL = (const wchar_t*)szSql;
		}
		else
		{
			//theLog->WriteLog(0, "didn't support encoding:%d", encoding);
		}
	}


	std::wstring _wstrSQL;
	SQLDBC_StringEncodingType::Encoding _encoding;
};


class CPreparedStatementInfoMgr
{
public:
	CPreparedStatementInfoMgr();
	~CPreparedStatementInfoMgr();


public:
	void AddedStatementSQL(void* pStatement, const char* szSQL, SQLDBC_StringEncodingType::Encoding encoding);
	const StatementSQL* FindStatementSQL(void* pStatement);


	void AddedStatementParam(void* pStatement, SQLDBC_UInt2 Index, SQLDBC_HostType Type, void * paramAddr, SQLDBC_Length* LengthIndicator,
		SQLDBC_Length Size, SQLDBC_Bool Terminate);
	const LIST_STATEMENT_PARAM* FindStatementParam(void* pStatement);

	void AddedEnforceStatement(void* pOriginalStatement, void* pEnforceStatement);
	void* FindEnforcerStatement(void* pOriginalStatement);
	void RemoveEnforceStatement(void* pOriginalStatement);

	void ReleaseStatement(void* pOriginalStatement);



private:
	std::map<void*, StatementSQL*>  _StatementSQL;
	std::mutex  _mutexStatementSQL;

	std::map<void*, LIST_STATEMENT_PARAM> _StatementParam;
	std::mutex _mutextStatementParam;

	std::map<void*, void*>  _OriginalStatementToEnforceStatement;
	std::mutex _mutextEnforceStatement;

};

extern CPreparedStatementInfoMgr* g_thePreparedStatementMgr;


#endif
