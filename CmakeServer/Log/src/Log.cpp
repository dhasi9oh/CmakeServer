// Log.cpp: 定义应用程序的入口点。
//

#include "Log.h"

Log* Log::instance()
{
	static Log log;
	return &log;
}
Log::Log()
{
	m_MinLevel = Level::debug;
	is_Open.store(false);
	is_Async.store(false);
    m_MaxSize = 8192;
    m_Logger = nullptr;
}

Log::~Log()
{
	is_Open.store(false);
}

bool Log::async() const
{
	return is_Async.load();
}

bool Log::open() const
{
	return is_Open.load();
}

Log::Level Log::getLevel() const
{
	return m_MinLevel;
}

void Log::initOut(Level level)
{
    if (is_Async.load()) {
        spdlog::init_thread_pool(m_MaxSize, 1);
        m_Logger = spdlog::stdout_color_mt<spdlog::async_factory>("m_Out");
	}
	else {
		m_Logger = spdlog::stdout_color_mt("m_Out");
	}

	m_MinLevel = level;
	is_Open.store(true);
}

void  Log::initBasic(std::string name, Level level)
{
	if (is_Async.load()) {
        spdlog::init_thread_pool(m_MaxSize, 1);
        m_Logger = spdlog::basic_logger_mt<spdlog::async_factory>("m_AsyncBasic", name, true, m_Event);
	}
	else {
		m_Logger = spdlog::basic_logger_mt("m_Basic", name, true, m_Event);
	}

	m_MinLevel = level;
	is_Open.store(true);
}

void Log::initRotating(std::string name,Log::Level level,
	size_t memory_size, size_t file_cnt)
{
	if (is_Async.load()) {
        spdlog::init_thread_pool(m_MaxSize, 1);
        m_Logger = spdlog::rotating_logger_mt<spdlog::async_factory>("m_AsyncRotating", name, memory_size, file_cnt, true, m_Event);
	}
	else {
		m_Logger = spdlog::rotating_logger_mt("m_Rotating", name, memory_size, file_cnt, true, m_Event);
	}

	m_MinLevel = level;
	is_Open.store(true);
}

void Log::initDaily(std::string name, Log::Level level,
	int hour, int min, size_t file_cnt)
{
	if (is_Async.load()) {
        spdlog::init_thread_pool(m_MaxSize, 1);
        m_Logger = spdlog::daily_logger_mt<spdlog::async_factory>("m_AsyncDaily", name, hour, min, true, file_cnt, m_Event);
	}
	else {
		m_Logger = spdlog::daily_logger_mt("m_Daily", name, hour, min, true, file_cnt, m_Event);
	}

	m_MinLevel = level;
	is_Open.store(true);
}

void Log::initMaxSize(int max_size)
{
	m_MaxSize = max_size;
}

void Log::setAsync()
{
	is_Async.store(true);
}

void Log::close()
{
	is_Open.store(false);
}

void Log::setLevel(Log::Level level)
{
	m_MinLevel = level;
}

void Log::setFileEvent(Log::FileEven event)
{
	m_Event = event;
}

bool Log::write(Log::Level level, const std::string& s)
{
	if (is_Open.load() == false) {
		return false;
    }
    _switchWrite(level, s);
	return true;
}


void Log::_switchWrite(Level level, const std::string& str)
{
	assert(m_Logger != nullptr);

	switch (level) {
	case spdlog::level::err:
		m_Logger->error(str.c_str());
		break;
	case spdlog::level::info:
		m_Logger->info(str.c_str());
		break;
	case spdlog::level::warn:
		m_Logger->warn(str.c_str());
		break;
	case spdlog::level::debug:
		m_Logger->debug(str.c_str());
		break;
	case spdlog::level::trace:
		m_Logger->trace(str.c_str());
		break;
	case spdlog::level::critical:
		m_Logger->critical(str.c_str());
		break;
	default:
		m_Logger->debug(str.c_str());
		break;
	}
}
