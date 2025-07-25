#include <stdio.h>
#include <stdlib.h>

int g_var = 1;
const int g_cvar = 2;
static int sg_var = 3;
static const int sg_cvar = 4;

static void static_fn(void);
void extern_fn(void);

static void static_fn(void)
{
}

void extern_fn(void)
{
    static int ext_static = 55;
    printf("[extern_fn]   static local  ext_static       @ %p\n", (void *)&ext_static);
}

static void print_fn_addr(const char *label, void (*fn)(void))
{
    union { void (*fp)(void); void *vp; } conv;
    conv.fp = fn;
    printf("%-40s @ %p\n", label, conv.vp);
}

void recurse(int depth)
{
    int l_var = depth;
    const int l_cvar = depth + 1;
    int *h_var = (int *)malloc(sizeof *h_var);
    (void)l_cvar;
    printf("[recurse] depth %d | stack l_var @ %p | heap h_var @ %p\n", depth, (void *)&l_var, (void *)h_var);
    if (depth < 5)
    {
        recurse(depth + 1);
    }
    free(h_var);
}

int main(int argc, char **argv, char **envp)
{
    size_t big_sz = (size_t)64 * 1024 * 1024; /* 64 MiB; large enough that glibc malloc typically uses mmap */
    void *big = malloc(big_sz);
    (void)argc;
    printf("--- large malloc (possible mmap) size=%lu bytes @ %p\n", (unsigned long)big_sz, big);
    if (big != NULL)
    {
        ((char *)big)[0] = 0;
        ((char *)big)[big_sz - 1] = 0;
    }

    puts("=== memory-layout address report ===");
    puts("--- globals & static globals ---");
    printf("Global  (non-const)  g_var            @ %p\n", (void *)&g_var);
    printf("Global  (const)      g_cvar           @ %p\n", (void *)&g_cvar);
    printf("Static  (non-const)  sg_var           @ %p\n", (void *)&sg_var);
    printf("Static  (const)      sg_cvar          @ %p\n", (void *)&sg_cvar);

    puts("--- code (text) section addresses ---");
    print_fn_addr("static function static_fn", static_fn);
    print_fn_addr("extern  function extern_fn", extern_fn);

    puts("--- runtime strings / pointers ---");
    printf("argv[0]   string literal             @ %p\n", (void *)argv[0]);
    if (envp != NULL && envp[0] != NULL)
    {
        printf("envp[0]   first environment string   @ %p\n", (void *)envp[0]);
    }

    puts("--- recursion: watch stack \342\226\266 / heap \342\226\265 ---");
    recurse(0);
    extern_fn();

    free(big);
    return 0;
}

