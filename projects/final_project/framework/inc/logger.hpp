/*****************************************************************************
 * Exercise:    Singleton - Logger
 * Date:        08/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    
 * Status:      
 *****************************************************************************/

#ifndef __ILRD_LOGGER__ 
#define __ILRD_LOGGER__ 

#include <fstream>      // std::ofstream
#include <thread>       // std::thread
#include <atomic>       // std::atomic
#include <string>       // std::string

#include "waitablequeue.hpp"
#include "handleton.hpp"
#include "singleton.hpp"

namespace ilrd
{

class Logger
{
public:
    using index_t = std::ptrdiff_t;
    
    enum LogLevel
    {
        ERROR = 0, 
        WARNING = 1,
        DEBUGING = 2,
        INFO = 3
    };
    
    void SetLevel(LogLevel level = DEBUGING) noexcept;

    void Log(const std::string& msg, 
             LogLevel level = DEBUGING, 
             std::string file_name = __FILE__, 
             int line = __LINE__);
    
    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;
    
private:
    struct LogArgs
    {
        std::string msg;
        LogLevel level;
        std::string file_name;
        int line;
    };

    friend class Handleton<Logger>;
    friend class Singleton<Logger>;

    Logger();
    ~Logger();
    
    index_t WriteToFile(); 
    
    std::string LevelToString(LogLevel level) const;
    
    std::atomic<bool> m_is_alive;
    WaitableQueue<LogArgs> m_queue;
    LogLevel m_curr_level;
    std::ofstream m_file;
    std::thread m_writer;

}; // class Logger

static const std::string g_log_path = "./log_file";

} // namespace ilrd

#endif // __ILRD_LOGGER__