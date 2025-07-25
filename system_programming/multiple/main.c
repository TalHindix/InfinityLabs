#include <stdio.h>
#include <dlfcn.h>
#include "globals.h"

int main(void)
{
    void *h1 = NULL;
    void *h2 = NULL;
    void (*Foo)(void)  = NULL;
    void (*Foo2)(void) = NULL;
    char *err;

    puts("=== main: calling PrintGlobalVariableAddress() ===");
    PrintGlobalVariableAddress();

    puts("=== dlopen libfoo.so ===");
    h1 = dlopen("./libfoo.so", RTLD_NOW);
    if (!h1) { fprintf(stderr, "%s\n", dlerror()); return 1; }

    dlerror();  /* clear */
    *(void **)(&Foo) = dlsym(h1, "Foo");   /* POSIX-safe assignment */
    err = dlerror();
    if (err) { fprintf(stderr, "%s\n", err); return 1; }

    puts("=== calling Foo() ===");
    Foo();

    puts("=== dlopen libfoo_2.so ===");
    h2 = dlopen("./libfoo_2.so", RTLD_NOW);
    if (!h2) { fprintf(stderr, "%s\n", dlerror()); return 1; }

    dlerror();  /* clear */
    *(void **)(&Foo2) = dlsym(h2, "Foo2");
    err = dlerror();
    if (err) { fprintf(stderr, "%s\n", err); return 1; }

    puts("=== calling Foo2() ===");
    Foo2();

    dlclose(h2);
    dlclose(h1);
    return 0;
}

