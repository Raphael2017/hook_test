#ifndef HOOK_IMPL_H
#define HOOK_IMPL_H

#include "interface.h"

class SubHookImpl : public IHook {
public:
    virtual void *Install(void *old_addr, void *new_addr) override;
};

class FuncHookImpl : public IHook {
public:
    virtual void *Install(void *old_addr, void *new_addr) override;
};


#endif