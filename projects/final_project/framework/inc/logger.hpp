/*****************************************************************************
 * Exercise:    Logger
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
             const std::string file_name = __FILE__, 
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
    const char* LevelToColor(Logger::LogLevel level);
    std::string LevelToString(LogLevel level) const;
    
    std::atomic<bool> m_is_alive;
    WaitableQueue<LogArgs> m_queue;
    LogLevel m_curr_level;
    std::ofstream m_file;
    std::thread m_writer;

}; // class Logger

static const std::string g_log_path = "./log_file";

} // namespace ilrd


#define LOG(level, msg) \
    ilrd::Handleton<ilrd::Logger>::GetInstance()->Log(msg, level, __FILE__, __LINE__)

// Basic logging macros
#define LOG_ERROR(msg)   LOG(ilrd::Logger::ERROR, msg)
#define LOG_WARNING(msg) LOG(ilrd::Logger::WARNING, msg)
#define LOG_DEBUG(msg)   LOG(ilrd::Logger::DEBUGING, msg)
#define LOG_INFO(msg)    LOG(ilrd::Logger::DEBUGING, msg)

// Component-aware logging macro
#define COMPONENT_LOG(component, level, msg) \
    ilrd::Handleton<ilrd::Logger>::GetInstance()->Log( \
        std::string("[" component "] ") + msg, level, __FILE__, __LINE__)

// Framework component macros
#define FACTORY_LOG(level,msg)        COMPONENT_LOG("Factory", level, msg)
#define REACTOR_LOG(level, msg)       COMPONENT_LOG("Reactor", level, msg)
#define THREADPOOL_LOG(level, msg)    COMPONENT_LOG("ThreadPool", level, msg)
#define SCHEDULER_LOG(level, msg)     COMPONENT_LOG("Scheduler", level, msg)
#define DISPATCHER_LOG(level, msg)    COMPONENT_LOG("Dispatcher", level, msg)
#define DIRMONITOR_LOG(level, msg)    COMPONENT_LOG("DirMonitor", level, msg)
#define DLLLOADER_LOG(level, msg)     COMPONENT_LOG("DllLoader", level, msg)
#define MEDIATOR_LOG(level, msg)      COMPONENT_LOG("InputMediator", level, msg)
#define FRAMEWORK_LOG(level, msg)     COMPONENT_LOG("Framework", level, msg)

// Concrete component macros (for future use)


// Disable debug logs in release build for performance
#ifdef NDEBUG
    #undef LOG_DEBUG
    #define LOG_DEBUG(msg) ((void)0)
#endif

#endif // __ILRD_LOGGER__