#include <iostream>
#include <thread>
#include <chrono>

#include "dir_monitor.hpp"
#include "dll_loader.hpp"
#include "dispatcher.hpp"

using namespace ilrd;

 struct Event
 {
    std::string data;
    int id;
 };
 
int main()
{
    std::cout << "=== Test Start ===" << std::endl;
    
    DirMonitor monitor("plugins");
    DllLoader loader;
    
    Callback<const std::string&,DllLoader> cb(loader,&DllLoader::LoadCallback);
    
    monitor.Register(&cb);
    
    std::cout << "Watching " << monitor.GetDirPath() << "..." << std::endl;
    
    monitor.Run();
    
    std::this_thread::sleep_for(std::chrono::seconds(30));
    
    std::cout << "=== Test End ===" << std::endl;
    
    return 0;
}