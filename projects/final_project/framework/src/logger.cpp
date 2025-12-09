/*****************************************************************************
 * Exercise:    Singleton - Logger
 * Date:        08/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include <chrono>       // std::chrono
#include <ctime>        // std::localtime, std::time_t
#include <sstream>      // std::ostringstream
#include <iostream>     // std::cerr

#include "logger.hpp"

namespace ilrd
{

static const std::chrono::milliseconds QUEUE_TIMEOUT(100);

Logger::Logger() : m_is_alive(true),
                    m_queue(),
                    m_curr_level(DEBUGING),
                    m_file(),
                    m_writer()
{
    m_file.open(g_log_path, std::ios::app);

    if (!m_file.is_open())
    {
        std::cerr << "Logger: Failed to open log file: " << g_log_path << std::endl;
    }

    m_writer = std::thread(&Logger::WriteToFile, this);
}

Logger::~Logger()
{
    // Signal the writer thread to stop
    m_is_alive.store(false);
    
    // Wait for writer thread to finish
    if (m_writer.joinable())
    {
        m_writer.join();
    }
    
    // Close the file
    if (m_file.is_open())
    {
        m_file.close();
    }
}

void Logger::Log(const std::string& msg, LogLevel level, 
                 std::string file_name, int line)
{
    // Filter by level first (optimization)
    if (level > m_curr_level)
    {
        return;
    }

    time_t now = time(nullptr);
    struct tm time_info;
    localtime_r(&now, &time_info);

    char time_stamp[20];
    strftime(time_stamp, sizeof(time_stamp), 
             "%d%m%Y:%H:%M:%S", &time_info);

    std::string log_msg = std::string(time_stamp) + ": " + msg;

    LogArgs args = { log_msg, level, file_name, line };
    m_queue.push(args);
}

void Logger::SetLevel(LogLevel level) noexcept
{
    m_curr_level = level;
}

Logger::index_t Logger::WriteToFile()
{
    index_t entriesWritten = 0;
    LogArgs args;
    
    while (m_is_alive.load() || !m_queue.empty())
    {
        bool hasEntry = m_queue.pop(&args, std::chrono::milliseconds(100));
        
        if (!hasEntry)
        {
            continue;
        }

        m_file << "[" << LevelToString(args.level) << "] "
               << args.msg << " "
               << args.file_name << ":" << args.line
               << std::endl;
        
        ++entriesWritten;
    }

    m_file.flush();
    
    return entriesWritten;
}

std::string Logger::LevelToString(LogLevel level) const
{
    switch (level)
    {
        case ERROR:     return "ERROR";
        case WARNING:   return "WARNING";
        case DEBUGING:     return "DEBUG";
        case INFO:      return "INFO";
        default:        return "UNKNOWN";
    }
}

}// ilrd