#include "course.hpp"

Course::Course(int id, QString code, QString name, int teacherId, int semester, int credits)
    : id(id), code(code), name(name), teacherId(teacherId), semester(semester), credits(credits)
{
}