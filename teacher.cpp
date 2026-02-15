#include "teacher.hpp"

Teacher::Teacher(int id, QString name, QString email, QString dept, QString desig)
    : Person(id, name, email), department(dept), designation(desig)
{
}