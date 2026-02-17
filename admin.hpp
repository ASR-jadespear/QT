#ifndef ADMIN_H
#define ADMIN_H

#include "person.hpp"

/**
 * @brief Represents an Administrator.
 * Admin is a Person only, with no extra academic attributes.
 */
class Admin : public Person
{
public:
    /**
     * @brief Constructs an Admin object.
     */
    Admin(int id, QString name, QString email, QString username, QString password);

    QString getRole() const override;
};

#endif // ADMIN_H