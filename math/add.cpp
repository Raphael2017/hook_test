#include "add.h"

Lesson::Lesson(const std::string& name, unsigned int class_hour) : _name(name), _class_hour(class_hour) {}

std::string Lesson::GetName() { return _name; }

unsigned int Lesson::GetClassHour() { return _class_hour; }

Student::Student(const std::string& name, unsigned int age) : _name(name), _age(age) {}

std::string Student::GetName() { return _name; }

unsigned int Student::GetAge() { return _age; }

void Student::SelectLesson(Lesson *lesson) {
    printf("Call SelectLesson, Student: %p, Lesson: %p\n", this, lesson);
    _selected_lessons.insert(lesson);
}

void Student::CancelLesson(Lesson *lesson) { _selected_lessons.erase(lesson); }

const std::set<Lesson*>& Student::GetSelectedLessons() { return _selected_lessons; }