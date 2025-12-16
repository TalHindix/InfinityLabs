/*****************************************************************************
 * Exercise:    DllLoader
 * Date:        11/12/2025
 * Developer:   Tal Hindi
 * Reviewer:    Avi Tobar
 * Status:      Approved
 *****************************************************************************/

#ifndef __ILRD_DLL_LOADER__
#define __ILRD_DLL_LOADER__

#include <string>               // std::string

namespace ilrd
{

class DllLoader
{
public:
    explicit DllLoader();
    ~DllLoader() noexcept;
    
    void* Load(const std::string& plugin_name);
    void LoadCallback(const std::string& plugin_name);

    DllLoader(const DllLoader&) = delete;
    DllLoader& operator=(const DllLoader&) = delete;
    
private:

}; // class DllLoader

} // namespace ilrd

#endif // __ILRD_DLL_LOADER__