#include <iostream>
#include "add.h"
#include <unistd.h>
#include <dlfcn.h>
#include <assert.h>

void run() {
    while (true) {
        sleep(30);
        int m = rand()%10, n = rand()%10;
        int t = add(m, n);
        printf("%d\n", t);
        sleep(4);
    }
}

int main() {
    srand((unsigned int)time(0));
    run();
    return 0;
}