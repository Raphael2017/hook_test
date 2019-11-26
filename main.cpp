#include <iostream>
#include "add.h"
#include <unistd.h>
#include <dlfcn.h>
#include <assert.h>
#include <vector>

Student *random_student(std::vector<Student>& students) {
    //return &students.at(rand() % students.size());
    return &students.at(0);
}

Lesson *random_lesson(std::vector<Lesson>& lessons) {
    return &lessons.at(rand() % lessons.size());
}

extern "C" {
    int add(int i, int j) { return i+j; }
}

int main() {
    int mmm = add(1,2);
    srand(time(0));
    std::vector<Lesson> lessons {
            { "Chinese", 72 },      { "Mathematics", 72 },
            { "English", 72 },      { "Physics", 48 },
            { "Chemistry", 48 },    { "Biology", 48 }
    };

    std::vector<Student> students {
            { "ZhangSan", 19 }, { "LiSi", 20 }, { "WangEr", 19 }
    };

    while (true) {
        mmm = add(mmm, 2*mmm);
        Student *student = random_student(students);
        student->SelectLesson(random_lesson(lessons));
        student->CancelLesson(random_lesson(lessons));
        sleep(5);
    }

    return 0;
}