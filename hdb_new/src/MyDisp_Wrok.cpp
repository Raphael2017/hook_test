#include "MyDisp_Wrok.h"
#include "log.h"


MyDisp_Wrok* g_theMyDW = NULL;

disp_work_ThRqSetCurrentRequest_Fun MyDisp_Wrok::_disp_work_ThRqSetCurrentRequest_Old = NULL;

MyDisp_Wrok::MyDisp_Wrok()
{
	_pBaseAddrDW = NULL;
}


MyDisp_Wrok::~MyDisp_Wrok()
{
}

bool MyDisp_Wrok::Init()
{
	const char* szDispWorkLibName = "disp+work.EXE";
	//load Disp+Work.exe
	_pBaseAddrDW =(unsigned char*)::LoadLibraryA(szDispWorkLibName);
	theLog->WriteLog(0, "MyDisp_Wrok::Init _pBaseAddrDW=%p", _pBaseAddrDW);

	if (_pBaseAddrDW)
	{

		//hook
		_hookMgr = new HookManager();

		_disp_work_ThRqSetCurrentRequest_Old =(disp_work_ThRqSetCurrentRequest_Fun)(_pBaseAddrDW + OFFSET_ThRqSetCurrentRequest);

		_hookMgr->AddHookItem((void**)&_disp_work_ThRqSetCurrentRequest_Old,
			My_disp_work_ThRqSetCurrentRequest,
			szDispWorkLibName, "ThRqSetCurrentRequest");

		_hookMgr->hook();
	}





	return _pBaseAddrDW != NULL;
}

const MY_REQUEST_BUF* MyDisp_Wrok::GetCurrentRequest()
{
#if 0
	MY_REQUEST_BUF** addr = (MY_REQUEST_BUF**)(_pBaseAddrDW + OFFSET_CurrentRequest);
	return *addr;  //the symbol 00007ff7`124e53f8 disp_work!pCurrentRequest always be NULL when execute sql
#else
	return &_currentRequest;
#endif 
}

void WINAPI MyDisp_Wrok::My_disp_work_ThRqSetCurrentRequest(MY_REQUEST_BUF* pBuf, unsigned int p1)
{
	if (pBuf)
	{
		MY_DP_SESSION_INFO& sessionInfo = pBuf->_sender._sessionInfo;
		//theLog->WriteLog(0, "My_disp_work_ThRqSetCurrentRequest  request_buf=%p,threadID=%d, sessionInfo._logonHdl=%d, sessionInfo._logonId=%d, sessionInfo._sessionHdl=%d",
		//	pBuf, GetCurrentThreadId(), sessionInfo._logonHdl, sessionInfo._logonId, sessionInfo._sessionHdl);
		
		//save buf
		memcpy(&(g_theMyDW->_currentRequest), pBuf, sizeof(MY_REQUEST_BUF));
	}
	if (_disp_work_ThRqSetCurrentRequest_Old)
	{
		_disp_work_ThRqSetCurrentRequest_Old(pBuf, p1);
	}
}
