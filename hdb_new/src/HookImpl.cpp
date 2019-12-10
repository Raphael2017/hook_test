#include "HookImpl.h"
#include "subhook.h"
#include "funchook.h"
#include "assert.h"

void *SubHookImpl::Install(void *oldf, void *newf) {
    auto hk = new subhook::Hook((void*)oldf, (void*)newf, subhook::HookFlag64BitOffset);
    if (!hk->Install()) {
        assert(false);
        return nullptr;
    }
    if (hk->GetTrampoline() == nullptr) {
        assert(false);
        return nullptr;
    }
    return hk->GetTrampoline();
}


void *FuncHookImpl::Install(void *oldf, void *newf) {
    funchook_t *funchook = funchook_create();
    int rv = funchook_prepare(funchook, (void**)&oldf, newf);
    if (rv != 0) {
        assert(false);
        return nullptr;
    }
    rv = funchook_install(funchook, 0);
    if (rv != 0) {
        assert(false);
        return nullptr;
    }
    return oldf;
}

IHook *create_hook(IHook::HOOK_TYPE t) {
    switch (t) {
        case IHook::HOOK_BY_SUBHOOK: return new SubHookImpl;
        case IHook::HOOK_BY_FUNCHOOK: return new FuncHookImpl;
        default: return nullptr;
    }
}