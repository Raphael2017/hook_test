#ifndef LOG_H_2019_01_28
#define LOG_H_2019_01_28
#include <stdio.h>
#include <string>

#include <boost/log/core.hpp>
#include <boost/log/attributes.hpp> 
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sinks/debug_output_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/console.hpp> // boost::log::add_console_log
#include <boost/log/utility/setup/file.hpp> // boost::log::add_file_log
#include <boost/filesystem.hpp>
#include <boost/log/sinks/async_frontend.hpp>

enum emLogLevel
{
    log_trace = boost::log::trivial::trace,
    log_debug = boost::log::trivial::debug,
    log_info = boost::log::trivial::info,
    log_warning = boost::log::trivial::warning,
    log_error = boost::log::trivial::error,
    log_fatal = boost::log::trivial::fatal
};

class CLog
{
 public:
 static CLog* Instance()
 {
     static CLog* theLog = new CLog();
     return theLog;
 }

 bool InitLog(const std::string& strModuleName);
 int  WriteLog(int lvl, const wchar_t* fmt, ...);
 int  WriteLog(int lvl, const char* fmt, ...);

 void UpdateLogLevel(const std::string& strlevel);

 protected:
 CLog();
 CLog(const CLog&){}

 private:
 std::string GetLogPath();
void WriteLogInternal(int lvl, const char* msg);
std::string GetProgramDataFolder();
std::string ToUTF8(const std::u16string& src, std::string& dst);
std::string ToUTF8(const std::wstring& src, std::string& dst);

 private:
 int m_logLevel;

};

#define theLog CLog::Instance()

#endif 