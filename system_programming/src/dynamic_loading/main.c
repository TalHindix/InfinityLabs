#include <stdio.h>
#include <dlfcn.h>

int main(void)
{
    void *handle = NULL;
    void (*Foo)(int) = NULL;
    char *error = NULL;
    int result = 0;

    handle = dlopen("./libfoo.so", RTLD_LAZY);
    if (handle == NULL)
    {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return 1;
    }

    dlerror();  /* clear previous errors */

    *(void **)(&Foo) = dlsym(handle, "Foo");
    error = dlerror();
    if (error != NULL)
    {
        fprintf(stderr, "dlsym error: %s\n", error);
        dlclose(handle);
        return 1;
    }

    Foo(123);  /* call the loaded function */

    result = dlclose(handle);
    if (result != 0)
    {
        fprintf(stderr, "dlclose error: %s\n", dlerror());
        return 1;
    }

    return 0;
}
