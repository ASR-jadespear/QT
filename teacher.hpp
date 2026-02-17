#ifndef TEACHER_H
#define TEACHER_H

#include "person.hpp"

/**
 * @brief Represents a Teacher user in the system.
 *
 * Inherits from Person and includes teacher-specific attributes like
 * department and designation.
 */
class Teacher : public Person
{
private:
    QString department;  ///< Academic department.
    QString designation; ///< Job title (e.g., "Lecturer", "Professor").
    double salary;

public:
    /**
     * @brief Constructs a Teacher object.
     * @param id Unique Teacher ID.
     * @param name Full Name.
     * @param email Email Address.
     * @param dept Department.
     * @param desig Designation.
     */
    Teacher(int id, QString name, QString email, QString dept = "", QString desig = "");

    QString getRole() const override { return "Teacher"; }

    QString getDepartment() const { return department; }
    QString getDesignation() const { return designation; }
    double getSalary() const { return salary; }

    void setSalary(double s) { salary = s; }
};

#endif // TEACHER_H