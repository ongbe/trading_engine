//**************************************************
//  Author:      Sunny Yan
//  Created On:  Fri Apr 4 14:20:42 HKT 2014
//  Description:
//
//**************************************************

#include "Logger.h"

boost::weak_ptr<Logger> Logger::m_pInstance;

// --------------------------------------------------
// Pantheois - Begin
// --------------------------------------------------
extern const PAN_CHAR_T PANTHEIOS_FE_PROCESS_IDENTITY[] = "DefaultLogger";

PANTHEIOS_CALL(PAN_CHAR_T const*) pantheios_fe_getProcessIdentity(void*)
{ return PANTHEIOS_LITERAL_STRING("DefaultLogger"); }

PANTHEIOS_CALL(int) pantheios_fe_init(void *, void **ptoken)
{ *ptoken = NULL; return 0; }

PANTHEIOS_CALL(void) pantheios_fe_uninit(void *) {}

PANTHEIOS_CALL(int) pantheios_fe_isSeverityLogged(void*,int severity,int)
{ return severity <= s_severityCeiling; }
// --------------------------------------------------
// Pantheois - End
// --------------------------------------------------

boost::shared_ptr<Logger> Logger::Instance()
{
  boost::shared_ptr<Logger> instance = m_pInstance.lock();

  if (!instance) {
    instance.reset(new Logger());
    m_pInstance = instance;
  }
  return instance;
}


Logger::~Logger()
{
  pantheios_be_file_setFilePath(NULL, PANTHEIOS_BEID_ALL);
}

void Logger::SetPath(const char * newPath)
{
  pantheios_be_file_setFilePath(PANTHEIOS_LITERAL_STRING(newPath),  PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BEID_ALL);
}

void Logger::SetLogLevel(LogLevel lev)
{
  switch (lev)
  {
    case EMERGENCY:
      s_severityCeiling = pantheios::emergency;
      break;
    case ALERT:
      s_severityCeiling = pantheios::alert;
      break;
    case CRITICAL:
      s_severityCeiling = pantheios::critical;
      break;
    case ERROR:
      s_severityCeiling = pantheios::error;
      break;
    case WARNING:
      s_severityCeiling = pantheios::warning;
      break;
    case NOTICE:
      s_severityCeiling = pantheios::notice;
      break;
    case INFO:
      s_severityCeiling = pantheios::informational;
      break;
    case DEBUG:
      s_severityCeiling = pantheios::debug;
      break;

    default:
      s_severityCeiling = pantheios::debug;
      break;
  }
  m_LogLevel = lev;
}

Logger::LogLevel Logger::GetLogLevel() const
{
  return m_LogLevel;
}

void Logger::Write(const LogLevel lev, const char *fmt, ...)
{
  char buffer[20480];
  va_list args;
  va_start (args, fmt);
  vsprintf (buffer, fmt, args);

  try
  {
    switch (lev)
    {
      case EMERGENCY:
        pantheios::log_EMERGENCY(buffer);
        break;
      case ALERT:
        pantheios::log_ALERT(buffer);
        break;
      case CRITICAL:
        pantheios::log_CRITICAL(buffer);
        break;
      case ERROR:
        pantheios::log_ERROR(buffer);
        break;
      case WARNING:
        pantheios::log_WARNING(buffer);
        break;
      case NOTICE:
        pantheios::log_NOTICE(buffer);
        break;
      case INFO:
        pantheios::log_INFORMATIONAL(buffer);
        break;
      case DEBUG:
        pantheios::log_DEBUG(buffer);
        break;

      default:
        pantheios::log_DEBUG(buffer);
        break;
    }
  }
  catch(std::bad_alloc&){
    pantheios::log_ALERT("Out of memory");
  }
  catch(std::exception& x){
    pantheios::log_CRITICAL("Exception: ", x);
  }
  catch(...){
    pantheios::puts(pantheios::emergency, "Unknown error");
  }

  va_end (args);
}

bool Logger::NeedToPrint(const LogLevel lev) const
{
  if (lev <= m_LogLevel) return true;
  else return false;
}
