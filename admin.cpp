#include "admin.hpp"

Admin::Admin(int id, QString name, QString email, QString username, QString password)
    : Person(id, name, email, username, password)
{
}

QString Admin::getRole() const
{
    return "Admin";
}