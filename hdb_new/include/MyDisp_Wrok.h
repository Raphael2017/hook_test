#ifndef MY_DISP_WORK_H
#define MY_DISP_WORK_H
#include "MyDisp_Work_Type.h"

#define OFFSET_CurrentRequest  0x00000000044853f8
#define OFFSET_ThRqSetCurrentRequest  0x15CFA4

typedef void (WINAPI *disp_work_ThRqSetCurrentRequest_Fun)(MY_REQUEST_BUF *, unsigned int);

class HookManager;

class MyDisp_Wrok
{
public:
	~MyDisp_Wrok();

private:
	MyDisp_Wrok();
	MyDisp_Wrok(const MyDisp_Wrok&) {}

public:
	static MyDisp_Wrok* Instance() {
		static MyDisp_Wrok mydw;
		return &mydw;
	}

	bool Init();

	const MY_REQUEST_BUF*  GetCurrentRequest();

	//hook api
private:
	static void WINAPI My_disp_work_ThRqSetCurrentRequest(MY_REQUEST_BUF *, unsigned int);



private:
	unsigned char* _pBaseAddrDW;
	HookManager* _hookMgr;

	MY_REQUEST_BUF  _currentRequest;

	static disp_work_ThRqSetCurrentRequest_Fun  _disp_work_ThRqSetCurrentRequest_Old;
};


extern MyDisp_Wrok* g_theMyDW;

#endif

