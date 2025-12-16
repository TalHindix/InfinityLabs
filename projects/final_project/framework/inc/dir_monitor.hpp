/*****************************************************************************
 * Exercise:    DirMonitor
 * Date:        11/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Avi Tobar
 * Status:      Approved
 *****************************************************************************/

#ifndef __ILRD_DIR_MONITOR__
#define __ILRD_DIR_MONITOR__

#include <string>               // std::string
#include <thread>               // std::thread
#include <atomic>               // std::atomic

#include "dispatcher.hpp"       // ilrd::Dispatcher

namespace ilrd
{

class DirMonitor
{
public:
    explicit DirMonitor(const std::string& dir);
    ~DirMonitor() noexcept;
    
    void Run();
    void Register(BaseCallback<const std::string&>* cb);
    void Unregister(BaseCallback<const std::string&>* cb);

    const std::string& GetDirPath() const // jUST FOR DEBUGGING
    {
        return m_dirPath;
    }

    DirMonitor(const DirMonitor&) = delete;
    DirMonitor& operator=(const DirMonitor&) = delete;
    
private:
    static void ListeningLoop(DirMonitor* dir_monitor);
    
    Dispatcher<const std::string&> m_dispatcher;
    std::thread m_listener;
    std::string m_dirPath;
    int m_inotifyFd;
    int m_watchFd;
    std::atomic<bool> m_isRunning;

}; // class DirMonitor

} // namespace ilrd

#endif // __ILRD_DIR_MONITOR__