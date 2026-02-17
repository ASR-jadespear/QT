#ifndef STUDENT_H
#define STUDENT_H

#include "person.hpp"
#include <QDate>
#include <QMap>
#include <QList>

/**
 * @brief Represents a Student user in the system.
 *
 * Inherits from Person and includes student-specific attributes like
 * department, batch, semester, and GPA.
 */
class Student : public Person
{
private:
    QString department; ///< Academic department (e.g., "CSE").
    QString batch;      ///< Batch year or code.
    int semester;       ///< Current semester number.
    double m_gpa;       ///< Current Grade Point Average.
    QDate dateAdmission;

public:
    /**
     * @brief Constructs a Student object.
     * @param id Unique Student ID.
     * @param name Full Name.
     * @param email Email Address.
     * @param dept Department Name.
     * @param batch Batch Code.
     * @param sem Current Semester.
     */
    Student(int id, QString name, QString email, QString dept = "", QString batch = "", int sem = 1);

    QString getRole() const override { return "Student"; }

    QString getDepartment() const { return department; }
    QString getBatch() const { return batch; }
    int getSemester() const { return semester; }
    QDate getDateAdmission() const { return dateAdmission; }
    double getCGPA() const { return m_gpa; }

    void setGpa(double gpa) { m_gpa = gpa; }
    void setDateAdmission(const QDate &date) { dateAdmission = date; }

    double calculateGPA() const;
};

#endif // STUDENT_H