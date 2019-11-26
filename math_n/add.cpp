#include "stdio.h"
#define SUBHOOK_X86_64
#include "subhook.h"
#include <dlfcn.h>


subhook::Hook *hk = nullptr;

class Lesson;
class Student;


typedef void(*SelectLessonFun)(void *self, void *lesson);

void SelectLessonNew(Student *self, Lesson *lesson) {
    printf("Call New SelectLesson, Student: %p, Lesson: %p, GetTrampoline: %p\n", self, lesson, hk->GetTrampoline());
    ((SelectLessonFun)hk->GetTrampoline())(self, lesson);
}

extern "C" {


__attribute__((constructor))
void loadMsg() {
    void *h = dlopen("libmath_add.so", RTLD_LAZY);
    if (h == NULL)
        return;
    // _ZN6SQLDBC24SQLDBC_PreparedStatement13bindParameterEj15SQLDBC_HostTypePvPxxb
    void *p = dlsym(h, "_ZN7Student12SelectLessonEP6Lesson");
    if (p != NULL) {
        hk = new subhook::Hook(p, (void*)&SelectLessonNew, subhook::HookFlag64BitOffset);
        if (hk->Install()) {
            printf("hook install success\n");
        } else {
            printf("hook install fail\n");
        }
    }
}

}