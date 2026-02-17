#include "course.hpp"

Course::Course(int id, QString code, QString name, int teacherId, int semester, int credits)
    : id(id), code(code), name(name), teacherId(teacherId), semester(semester), credits(credits)
{
}

// Operator Overloading implementation
std::ostream &operator<<(std::ostream &os, const Course &c)
{
    os << "Course[" << c.code.toStdString() << "]: " << c.name.toStdString() << " (" << c.credits << " credits)";
    return os;
}