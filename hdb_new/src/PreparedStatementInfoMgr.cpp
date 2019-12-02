#include "PreparedStatementInfoMgr.h"
//#include "MySqldbLib.h"
#include <thread>

CPreparedStatementInfoMgr* g_thePreparedStatementMgr = NULL;

CPreparedStatementInfoMgr::CPreparedStatementInfoMgr()
{
}


CPreparedStatementInfoMgr::~CPreparedStatementInfoMgr()
{
}


void CPreparedStatementInfoMgr::AddedStatementSQL(void* pStatement, const char* szSQL,
	SQLDBC_StringEncodingType::Encoding encoding)
{
	StatementSQL* pStateSQL = new StatementSQL(szSQL, encoding);

	{
		std::lock_guard<std::mutex> lockStatementSQL(_mutexStatementSQL);
		_StatementSQL[pStatement] = pStateSQL;
	}
	
}

const StatementSQL* CPreparedStatementInfoMgr::FindStatementSQL(void* pStatement)
{
	{
		std::lock_guard<std::mutex> lockStatementSQL(_mutexStatementSQL);
		auto itStateSQL = _StatementSQL.find(pStatement);
		if (itStateSQL != _StatementSQL.end())
		{
			return itStateSQL->second;
		}
	}

	return NULL;
}

void CPreparedStatementInfoMgr::AddedStatementParam(void* pStatement, SQLDBC_UInt2 Index, SQLDBC_HostType Type,
	void * paramAddr, SQLDBC_Length* LengthIndicator, SQLDBC_Length Size, SQLDBC_Bool Terminate)
{
	StatementParam* pStateParam = new StatementParam(Index, Type, paramAddr, LengthIndicator, Size, Terminate);

	{
		std::lock_guard<std::mutex> lockStatementParam(_mutextStatementParam);
		
		//find exist
		auto itStatementParam = _StatementParam.find(pStatement);
		if (itStatementParam != _StatementParam.end())
		{
			//delete if exist by Index
			auto itParam = itStatementParam->second.begin();
			while (itParam != itStatementParam->second.end())
			{
				if ((*itParam)->_Index == Index)
				{
					delete *itParam;
					itStatementParam->second.remove(*itParam);
				}
				itParam++;
			}

			//added
			itStatementParam->second.push_back(pStateParam);
		}
		else
		{
			std::list<StatementParam*> lstStatementParam;
			lstStatementParam.push_back(pStateParam);
			_StatementParam[pStatement] = lstStatementParam;
		}
	}
}

const LIST_STATEMENT_PARAM* CPreparedStatementInfoMgr::FindStatementParam(void* pStatement)
{
	{
		std::lock_guard<std::mutex> lockStatementParam(_mutextStatementParam);
		auto itStatementParam = _StatementParam.find(pStatement);
		if (itStatementParam != _StatementParam.end())
		{
			return &(itStatementParam->second);
		}
	}
	return NULL;
}

void CPreparedStatementInfoMgr::AddedEnforceStatement(void* pOriginalStatement, void* pEnforceStatement)
{
	{
		std::lock_guard<std::mutex> lockEnforceState(_mutextEnforceStatement);
		_OriginalStatementToEnforceStatement[pOriginalStatement] = pEnforceStatement;
	}
}

void* CPreparedStatementInfoMgr::FindEnforcerStatement(void* pOriginalStatement)
{
	{
		std::lock_guard<std::mutex> lockEnforceState(_mutextEnforceStatement);
		auto itEnforceState = _OriginalStatementToEnforceStatement.find(pOriginalStatement);
		if (itEnforceState != _OriginalStatementToEnforceStatement.end())
		{
			return itEnforceState->second;
		}
	}

	return NULL;
}

void CPreparedStatementInfoMgr::RemoveEnforceStatement(void* pOriginalStatement)
{
	{
		std::lock_guard<std::mutex> lockEnforceState(_mutextEnforceStatement);
		auto itEnforceState = _OriginalStatementToEnforceStatement.find(pOriginalStatement);
		if (itEnforceState != _OriginalStatementToEnforceStatement.end())
		{
			void* pEnforceStatement = itEnforceState->second;
			_OriginalStatementToEnforceStatement.erase(itEnforceState);

			//release
			//CMySqldbLib::ReleaseEnforceStatement(pEnforceStatement);
		}
	}

}

void CPreparedStatementInfoMgr::ReleaseStatement(void* pOriginalStatement)
{
//	theLog->WriteLog(0, "CPreparedStatementInfoMgr::ReleaseStatement pOriginalStatement. state:%p", pOriginalStatement);

	//release sql info
	{
		std::lock_guard<std::mutex> lockStatementSQL(_mutexStatementSQL);
		auto itStateSQL = _StatementSQL.find(pOriginalStatement);
		if (itStateSQL!=_StatementSQL.end())
		{
			delete itStateSQL->second;
			_StatementSQL.erase(itStateSQL);
		}

	}

	//release paramaters
	{
		std::lock_guard<std::mutex> lockStatementParm(_mutextStatementParam);
		auto itStateParams = _StatementParam.find(pOriginalStatement);
		if (itStateParams!=_StatementParam.end())
		{
			LIST_STATEMENT_PARAM& lstParams = itStateParams->second;
			auto itParam = lstParams.begin();
			while (itParam!=lstParams.end())
			{
				delete *itParam;
				itParam = lstParams.erase(itParam);
			}

			_StatementParam.erase(itStateParams);
		}
	}

	//release enforcer statement
	{
		std::lock_guard<std::mutex> lockEnforceState(_mutextEnforceStatement);
		auto itEnforcerState = _OriginalStatementToEnforceStatement.find(pOriginalStatement);
		if (itEnforcerState!=_OriginalStatementToEnforceStatement.end())
		{
			void* pEnforceState = itEnforcerState->second;
			_OriginalStatementToEnforceStatement.erase(itEnforcerState);

		    //release 
			//CMySqldbLib::ReleaseEnforceStatement(pEnforceState);
		}
	}
}