// Log.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#ifndef LOG
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif // !BLOCKQUEUE

#include <memory>
#include <string>
#include <atomic>
#include <thread>
#include <assert.h>
#include <functional>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
//#include <BlockQueue.h>
// TODO: 在此处引用程序需要的其他标头。

class API Log
{
public:

    using Level = spdlog::level::level_enum;

    using FileEven = spdlog::file_event_handlers;

	static Log* instance();

public:

    bool async() const;

    bool open() const;

    Level getLevel() const;

    void initOut(Level level = Level::info);

    void initBasic(std::string name = "log.txt", Level level = Level::info);

    void initRotating(std::string name = "log.txt", Level level = Level::info
        , size_t memory_size = 5 * 1024 * 1024, size_t file_cnt = 10);

    void initDaily(std::string name = "log.txt" , Level level = Level::info
        , int hour = 0, int min = 0, size_t file_cnt = 10);

    void initMaxSize(int max_size);

    void setAsync();

    void close();

    void setLevel(Level level);

    void setFileEvent(FileEven event);

    bool write(Level level, const std::string& s);

    template <class ...Args>
    bool write(Level level, const std::string& s, Args&& ...args)
    {
        std::string str = fmt::format(s, std::forward<Args>(args)...);
        return this->write(level, str);
    }


private:

	Log();

	~Log();

    void _switchWrite(Level level, const std::string& str);
	

private:

    int m_MaxSize;

    Level m_MinLevel;

    FileEven m_Event;

	std::atomic<bool> is_Open;

    std::atomic<bool> is_Async;

    //std::unique_ptr<std::thread> m_Thread;

	std::shared_ptr<spdlog::logger> m_Logger;

    //std::unique_ptr<BlockQueue<std::pair<Level, std::string>>> m_Pool;
};


#define LOG_DEBUG(...) Log::instance()->write(Log::Level::debug, ##__VA_ARGS__)
#define LOG_INFO(...) Log::instance()->write(Log::Level::info, ##__VA_ARGS__)
#define LOG_WARN(...) Log::instance()->write(Log::Level::warn, ##__VA_ARGS__)
#define LOG_ERR(...) Log::instance()->write(Log::Level::err, ##__VA_ARGS__)
