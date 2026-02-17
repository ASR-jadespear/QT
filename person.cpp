#include "person.hpp"

int Person::personCount = 0;

Person::Person(int id, QString name, QString email, QString username, QString password)
    : id(id), name(name), email(email), username(username), password(password)
{
    personCount++;
}

Person::~Person()
{
    personCount--;
}

int Person::getPersonCount()
{
    return personCount;
}