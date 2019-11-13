#include "add.h"
#include "stdio.h"
#include "gothook.h"
#include <dlfcn.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif




extern "C" {

typedef int(*add_fun)(int, int);

add_fun add_ori = nullptr;

__attribute__((constructor))
void loadMsg() {
    auto h1 = dlopen("libmath_add_n.so", RTLD_LAZY);
    auto h2 = dlopen("libmath_add.so", RTLD_LAZY);
    void *p1 = nullptr, *p2 = nullptr;
    if (h1 && h2 && (p1 = dlsym(h2, "add")) && (p2 = dlsym(h1, "add"))) {
        printf("load new add success, ori: %p, new: %p\n", p1, p2);
        hookFunc("hook", p1, p2, (void**)&add_ori);
    }
}

int add(int m, int n) {
    auto r = add_ori(m, n);
    printf("add is hooked, the result is %d, but it will be replaced with 0\n", r);
    r = 0;
    return r;
}

}