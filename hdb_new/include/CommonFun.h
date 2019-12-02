#ifndef COMMON_FUN_H
#define COMMON_FUN_H

#include <string>
class CommonFun
{
public:
	CommonFun();
	~CommonFun();

public:
	static std::wstring ConvertUtf8ToUtf16(const char* szUtf8);
	static std::string ConvertUtf16ToUtf8(const wchar_t* szUtf16);
};

#endif 

