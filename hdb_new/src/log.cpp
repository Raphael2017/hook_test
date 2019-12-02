#include "stdafx.h"
#include "log.h"
//#include "commfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <boost/program_options.hpp>
#ifdef WIN32
#include <windows.h>
#else
#include <stdarg.h>
#include <sys/stat.h>
#endif
#include <locale>
#include <codecvt>

#define MAX_MESSAGE_SIZE_CHARS 1024*10

namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;


// Declare attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

CLog::CLog()
{
  m_logLevel =   boost::log::trivial::trace;
}


std::string CLog::GetProgramDataFolder()
{
	std::string strFolder;


	//create sub folder
	strFolder += "c:\\ProgramData\\Nextlabs\\HANADB_Enforcer";

	int nRes = SHCreateDirectoryExA(NULL, strFolder.c_str(), NULL);
	if ((nRes != ERROR_SUCCESS) &&
		(nRes != ERROR_ALREADY_EXISTS) &&
		(nRes != ERROR_FILE_EXISTS))
	{
		strFolder = "";
	}


	return strFolder;
}

std::string CLog::ToUTF8(const std::u16string& src, std::string& dst) 
{
#if (defined WIN32) && (_MSC_VER >= 1900)
	auto p = reinterpret_cast<const int16_t *>(src.data());
	dst = std::wstring_convert< std::codecvt_utf8_utf16<int16_t>, int16_t >{}.to_bytes(p, p + src.size());
#else
	dst = std::wstring_convert< std::codecvt_utf8_utf16<char16_t>, char16_t >{}.to_bytes(src);
#endif
	return dst;
}

std::string CLog::ToUTF8(const std::wstring& src, std::string& dst) {
	dst = std::wstring_convert< std::codecvt_utf8<wchar_t >, wchar_t >{}.to_bytes(src);
	return dst;
}

std::string CLog::GetLogPath()
{

    std::string strAppDataFolder = GetProgramDataFolder();
	if (!strAppDataFolder.empty())
	{
		std::string strLogFile = strAppDataFolder + "/log";

		CreateDirectoryA(strLogFile.c_str(), NULL);

		strLogFile += "/";

		return strLogFile;
	}


	return "";
}


bool CLog::InitLog(const std::string& strModuleName)
{
    boost::shared_ptr< boost::log::core > core = boost::log::core::get();

	core->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);
	boost::log::add_common_attributes(); // Add TimeStamp, ThreadID so that we can use those attributes in Format.

	/* log formatter: [TimeStamp] [ThreadId] [Severity Level] [Scope] Log message */
	auto fmtTimeStamp = boost::log::expressions::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S.%f");
	//auto fmtThreadId = boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
	auto fmtSeverity = boost::log::expressions::attr<boost::log::trivial::severity_level>(severity.get_name());

	boost::log::formatter logFmt = boost::log::expressions::format("%1%|%2%|%3%")
		% fmtTimeStamp %  fmtSeverity % boost::log::expressions::smessage;
	

#if 1
	{
		typedef sinks::synchronous_sink < sinks::text_file_backend > sink_t;

		pid_t dwProcID = getpid();
        std::string strLogPath = GetLogPath()  + strModuleName + std::to_string(dwProcID) + "_%Y-%m-%d_%H-%M-%S.%N.log";
		typedef sinks::text_file_backend backend_t;
		boost::shared_ptr< sinks::text_file_backend > backend =
			boost::make_shared< sinks::text_file_backend >(
				boost::log::keywords::file_name =  strLogPath,
				boost::log::keywords::rotation_size = 10 * 1024 * 1024,
				boost::log::keywords::min_free_space = 30 * 1024 * 1024,
				boost::log::keywords::open_mode = std::ios_base::app);
		boost::shared_ptr< sink_t > fsSink(new sink_t(backend));
		fsSink->set_formatter(logFmt);
		fsSink->locked_backend()->auto_flush(true);
		core->add_sink(fsSink);
	}
#endif

#if 0
	/* Windows debugger output backend OutputDebugString */
	typedef sinks::asynchronous_sink< sinks::debug_output_backend > debug_sink_t;
	// Create the sink. The backend requires synchronization in the frontend.
	boost::shared_ptr< debug_sink_t > dbgSink = boost::make_shared<debug_sink_t>();
	// Set the special filter to the frontend in order to skip the sink when no debugger is available
//	dbgSink->set_filter(boost::log::expressions::is_debugger_present());
	dbgSink->set_formatter(logFmt);
	core->add_sink(dbgSink);
#endif 


    return true;
}


void CLog::WriteLogInternal(int nlevel, const char* szLog)
{
	if(nlevel>=m_logLevel)
	{
	 switch(nlevel)
	 {
		 case boost::log::trivial::trace:
		      BOOST_LOG_TRIVIAL(trace)<<szLog;
		      break;
		 case boost::log::trivial::debug:
		      BOOST_LOG_TRIVIAL(debug)<<szLog;
		       break;
		case boost::log::trivial::info:
		      BOOST_LOG_TRIVIAL(info)<<szLog;
		       break;
		case boost::log::trivial::warning:
		      BOOST_LOG_TRIVIAL(warning)<<szLog;
		       break;
		case boost::log::trivial::error:
		      BOOST_LOG_TRIVIAL(error)<<szLog;
		       break;
		 case boost::log::trivial::fatal:
		      BOOST_LOG_TRIVIAL(fatal)<<szLog;
		       break;
		default:
		 break;
	 }

	}
}

int CLog::WriteLog(int nlevel, const char* szFmt, ... )
{
	if(nlevel >= m_logLevel)
	{
		char szLog[MAX_MESSAGE_SIZE_CHARS] = {0};

	//format log content
	va_list args;
	va_start(args, szFmt);
	int nLog = vsnprintf(szLog, MAX_MESSAGE_SIZE_CHARS-1, szFmt, args);
	va_end(args);

    WriteLogInternal(nlevel, szLog);

	 return nLog;
	}
	
	return 0;
}

int CLog::WriteLog(int nlevel, const wchar_t* wszFmt, ... )
{
	if(nlevel >= m_logLevel)
	{
	 wchar_t wszLog[MAX_MESSAGE_SIZE_CHARS] = {0};

	//format log content
	va_list args;
	va_start(args, wszFmt);
	int nLog = vswprintf(wszLog, MAX_MESSAGE_SIZE_CHARS-1, wszFmt, args);
	va_end(args);

    //convert from wchar to char
    std::string u8_cvt_str;
	 ToUTF8(wszLog, u8_cvt_str);
	  WriteLogInternal(nlevel, u8_cvt_str.c_str() );
	  return nLog;
	}

	return 0;
}


void CLog::UpdateLogLevel(const std::string& strlevel)
{
	int nLevel = atoi(strlevel.c_str());
	if(nLevel>=boost::log::trivial::trace && nLevel<=boost::log::trivial::fatal)
	{
       boost::log::core::get()->set_filter(boost::log::trivial::severity >= nLevel);
	   m_logLevel = nLevel;
	}
}