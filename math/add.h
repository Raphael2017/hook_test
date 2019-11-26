#ifndef ADD_H
#define ADD_H

#include <string>
#include <set>

class Lesson {
public:
    Lesson(const std::string& name, unsigned int class_hour);
    std::string GetName();
    unsigned int GetClassHour();
private:
    std::string     _name;
    unsigned int    _class_hour;
};

class Student {
public:
    Student(const std::string& name, unsigned int age);
    std::string GetName();
    unsigned int GetAge();
    void SelectLesson(Lesson *lesson);
    void CancelLesson(Lesson *lesson);
    const std::set<Lesson*>& GetSelectedLessons();
private:
    std::string             _name;
    unsigned int            _age;
    std::set<Lesson*>       _selected_lessons;
};

#endif