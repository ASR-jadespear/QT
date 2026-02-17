#ifndef COURSE_H
#define COURSE_H

#include <QString>
#include <iostream>

/**
 * @brief Represents an academic course.
 *
 * Stores details about a specific subject, including its code, instructor,
 * and credit value.
 */
class Course
{
private:
    int id;        ///< Database Primary Key.
    QString code;  ///< Course Code (e.g., "CSE 4502").
    QString name;  ///< Course Name.
    int teacherId; ///< ID of the assigned teacher.
    int semester;  ///< Semester this course belongs to.
    int credits;   ///< Credit hours.

public:
    /**
     * @brief Constructs a Course object.
     */
    Course(int id, QString code, QString name, int teacherId, int semester, int credits);

    int getId() const { return id; }
    QString getCode() const { return code; }
    QString getName() const { return name; }
    int getTeacherId() const { return teacherId; }
    int getSemester() const { return semester; }
    int getCredits() const { return credits; }

    /**
     * @brief Friend function to overload the output stream operator.
     * Allows usage like: std::cout << myCourse;
     */
    friend std::ostream &operator<<(std::ostream &os, const Course &c);
};

#endif // COURSE_H