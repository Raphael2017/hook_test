#include "CommonFun.h"
#include <locale>
#include <codecvt>


CommonFun::CommonFun()
{
}


CommonFun::~CommonFun()
{
}


std::wstring CommonFun::ConvertUtf8ToUtf16(const char* szUtf8)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
	std::wstring u16_cvt_str = cvt.from_bytes(szUtf8); // utf-16 from utf-8
	return u16_cvt_str;
}

std::string CommonFun::ConvertUtf16ToUtf8(const wchar_t* szUtf16)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
	std::string u8_cvt_str = cvt.to_bytes(szUtf16); // utf-16 to utf-8
	return u8_cvt_str;
}
