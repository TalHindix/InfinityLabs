/*****************************************************************************
 * Exercise:    Logger
 * Date:        08/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#include <iostream>     // std::cerr

#include "logger.hpp"

namespace ilrd
{

static const std::chrono::milliseconds QUEUE_TIMEOUT(100);

Logger::Logger() : m_is_alive(true),m_curr_level(DEBUGING)                  
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
    m_is_alive.store(false);
    
    if (m_writer.joinable())
    {
        m_writer.join();
    }
    
    if (m_file.is_open())
    {
        m_file.close();
    }
}

void Logger::Log(const std::string& msg, LogLevel level, 
                 const std::string file_name, int line)
{
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

        m_file << LevelToColor(args.level)             
               << "[" << LevelToString(args.level) << "]" 
               << "\033[0m "                          
               << args.msg       
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

const char* Logger::LevelToColor(Logger::LogLevel level)
{
    switch (level)
    {
        case Logger::ERROR:     return "\033[31m"; // red
        case Logger::WARNING:   return "\033[33m"; // yellow
        case Logger::INFO:      return "\033[32m"; // green
        case Logger::DEBUGING:  return "\033[36m"; // cyan
        default:                      return "\033[0m";
    }


}

}// ilrd