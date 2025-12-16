/*****************************************************************************
 * Exercise:    DirMonitor
 * Date:        11/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Avi Tobar
 * Status:      Approved
 *****************************************************************************/

#include <sys/inotify.h>        // inotify_init
#include <unistd.h>             // read
#include <stdexcept>            // std::runtime_error
#include <filesystem>

#include "dir_monitor.hpp"

namespace ilrd
{

static const size_t EVENT_BUFFER_SIZE = 4096;
static const int INVALID_FD = -1;

DirMonitor::DirMonitor(const std::string& dir)
    : m_dispatcher()
    , m_listener()
    , m_dirPath(std::filesystem::absolute(dir).string())
    , m_inotifyFd(INVALID_FD)
    , m_watchFd(INVALID_FD)
    , m_isRunning(false)
{
    m_inotifyFd = inotify_init();
    
    if (INVALID_FD == m_inotifyFd)
    {
        throw std::runtime_error("DirMonitor: Failed to init inotify");
    }
    
    m_watchFd = inotify_add_watch(m_inotifyFd, m_dirPath.c_str(), IN_CREATE | IN_MOVED_TO);
    
    if (INVALID_FD == m_watchFd)
    {
        close(m_inotifyFd);
        throw std::runtime_error("DirMonitor: Failed to watch directory: " + dir);
    }
}

DirMonitor::~DirMonitor() noexcept
{
    m_isRunning.store(false);
    
    if (INVALID_FD != m_watchFd)
    {
        inotify_rm_watch(m_inotifyFd, m_watchFd);
    }
    
    if (m_listener.joinable())
    {
        m_listener.join();
    }
    
    if (INVALID_FD != m_inotifyFd)
    {
        close(m_inotifyFd);
    }
}

void DirMonitor::Run()
{
    if (m_isRunning.load())
    {
        return;
    }
    
    m_isRunning.store(true);
    m_listener = std::thread(ListeningLoop, this);
}

void DirMonitor::Register(BaseCallback<const std::string&>* cb)
{
    m_dispatcher.Subscribe(cb);
}

void DirMonitor::Unregister(BaseCallback<const std::string&>* cb)
{
    m_dispatcher.UnSubscribe(cb);
}

void DirMonitor::ListeningLoop(DirMonitor* dir_monitor)
{
    char buffer[EVENT_BUFFER_SIZE];
    
    while (dir_monitor->m_isRunning.load())
    {
        ssize_t bytesRead = read(dir_monitor->m_inotifyFd, buffer, EVENT_BUFFER_SIZE);
        
        if (bytesRead <= 0)
        {
            continue;
        }
        
        ssize_t offset = 0;
        while (offset < bytesRead)
        {
            struct inotify_event* event = 
                reinterpret_cast<struct inotify_event*>(buffer + offset);
            
            if (event->len > 0)
            {
                std::filesystem::path fullPath =
                    std::filesystem::path(dir_monitor->m_dirPath) / event->name;

                dir_monitor->m_dispatcher.Broadcast(fullPath.string());
            }
            
            offset += sizeof(struct inotify_event) + event->len;
        }
    }
}

} // namespace ilrd