#ifndef PERSON_H
#define PERSON_H

#include <QString>

/**
 * @brief Abstract base class representing a generic person in the system.
 *
 * This class serves as the foundation for specific roles like Student and Teacher,
 * holding common attributes such as ID, name, and email.
 */
class Person
{
protected:
    int id;        ///< Unique identifier for the person.
    QString name;  ///< Full name of the person.
    QString email; ///< Contact email address.

public:
    /**
     * @brief Constructs a Person object.
     * @param id Unique ID.
     * @param name Full name.
     * @param email Email address.
     */
    Person(int id, QString name, QString email)
        : id(id), name(name), email(email) {}

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~Person() {}

    /**
     * @brief Pure virtual function to get the role of the person.
     * @return A string representing the role (e.g., "Student", "Teacher").
     */
    virtual QString getRole() const = 0;

    int getId() const { return id; }
    QString getName() const { return name; }
    QString getEmail() const { return email; }
};

#endif // PERSON_H