/******************************************************************************
 * Exercise:    Explicit Loading Example
 * Date:        09/10/2025
 * Developer:   Tal Hindi
 * Reviewer:
 * Status:
 *****************************************************************************/

#include <dlfcn.h>  // dlopen, dlsym, dlclose, dlerror
#include <iostream> // std::cout, std::cerr

int main() {
  // Open the shared library
  void *handle = dlopen("./libmylib.so", RTLD_LAZY);

  if (handle == NULL) {
    std::cerr << "Failed to load library: " << dlerror() << std::endl;
    return 1;
  }

  // Clear any existing errors
  dlerror();

  // Get function pointer using C name (no mangling!)
  typedef int (*AddFunc)(int, int);
  AddFunc add = reinterpret_cast<AddFunc>(dlsym(handle, "AddNumbers"));

  // Check for errors
  const char *error = dlerror();
  if (error != NULL) {
    std::cerr << "Failed to load symbol: " << error << std::endl;
    dlclose(handle);
    return 1;
  }

  // Use the function
  std::cout << "5 + 3 = " << add(5, 3) << std::endl;

  // Close the library
  dlclose(handle);

  return 0;
}