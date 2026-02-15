#ifndef ACADEMICMANAGER_H
#define ACADEMICMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include "student.hpp"
#include "teacher.hpp"
#include "course.hpp"
#include "habit.hpp"

/**
 * @brief Represents a task in the Study Planner.
 */
struct Task
{
    int id;
    int studentId;
    QString description;
    bool isCompleted;
};

/**
 * @brief Represents a public notice posted by teachers.
 */
struct Notice
{
    int id;
    QString content;
    QString date;
    QString author;
};

/**
 * @brief Represents an academic assessment (Quiz/Assignment).
 */
struct Assessment
{
    int id;
    int courseId;
    QString courseName;
    QString title;
    QString type; // Quiz, Assignment
    QString date;
    int maxMarks;
};

/**
 * @brief Aggregated attendance and grade record for a student in a course.
 */
struct AttendanceRecord
{
    QString courseName;
    int totalClasses;
    int attendedClasses;
    double totalMarksObtained;
    int totalMaxMarks;
};

/**
 * @brief Represents a Q&A interaction between student and teacher.
 */
struct Query
{
    int id;
    int studentId;
    QString studentName;
    QString question;
    QString answer;
};

/**
 * @brief Represents a single class slot in the routine.
 */
struct RoutineItem
{
    int id;
    QString day;
    QString startTime;
    QString endTime;
    QString courseCode;
    QString courseName;
    QString room;
    QString instructor;
    int semester;
};

/**
 * @brief Tracks the daily prayer status for a student.
 */
struct DailyPrayerStatus
{
    bool fajr;
    bool dhuhr;
    bool asr;
    bool maghrib;
    bool isha;
};

/**
 * @brief The core controller class managing database interactions and business logic.
 *
 * Handles CRUD operations for all entities, authentication, and feature logic.
 */
class AcademicManager
{
private:
    QSqlDatabase db; ///< The SQLite database connection object.
    bool dbConnected;

    void createTables(); ///< Initializes database schema and migrations.

public:
    AcademicManager();
    ~AcademicManager();

    // Database Connection
    bool connectToDatabase(QString path);
    bool isOpen() const;

    /**
     * @brief Authenticates a user.
     * @param name Username.
     * @param password Password.
     * @param outId Reference to store the authenticated user ID.
     * @return The role of the user ("Student", "Teacher", "Admin") or empty string on failure.
     */
    QString login(QString name, QString password, int &outId);

    // === Student CRUD ===
    bool addStudent(const Student &s);
    bool removeStudent(int id);
    bool updateStudent(const Student &s);

    // Teacher CRUD
    bool addTeacher(const Teacher &t);
    bool removeTeacher(int id);
    bool updateTeacher(const Teacher &t);

    // Course CRUD
    bool addCourse(const Course &c);
    bool removeCourse(int id);
    bool updateCourse(const Course &c);

    // === Planner Features ===
    bool addTask(int studentId, QString desc);
    bool completeTask(int taskId, bool completed);
    QVector<Task> getTasks(int studentId);

    bool addHabit(Habit *habit);
    bool updateHabit(Habit *habit);
    bool deleteHabit(int id);
    QVector<Habit *> getHabits(int studentId);

    DailyPrayerStatus getDailyPrayers(int studentId, QString date);
    bool updateDailyPrayer(int studentId, QString date, QString prayerName, bool status);

    bool addNotice(QString content, QString author);
    QVector<Notice> getNotices();

    // === Routine Management ===
    bool addRoutineItem(QString day, QString startTime, QString endTime, QString courseCode, QString courseName, QString room, QString instructor, int semester);
    QVector<RoutineItem> getRoutineForDay(QString day, int semester = -1);

    // === Assessments & Grading ===
    bool addAssessment(int courseId, QString title, QString type, QString date, int maxMarks);
    QVector<Assessment> getAssessments(int courseId = -1);
    bool addGrade(int studentId, int assessmentId, double marks);
    double getGrade(int studentId, int assessmentId);

    // === Attendance ===
    bool markAttendance(int courseId, int studentId, QString date, bool present);
    QVector<AttendanceRecord> getStudentAttendance(int studentId);
    QVector<QString> getCourseDates(int courseId);
    bool isPresent(int courseId, int studentId, QString date);

    // === Communication ===
    bool addQuery(int studentId, QString question);
    bool answerQuery(int queryId, QString answer);
    QVector<Query> getQueries(int userId, QString role);
    QString getNextClass();
    QString getDashboardStats(int userId, QString role);

    // === Data Retrieval ===
    Student *getStudent(int id);
    Teacher *getTeacher(int id);
    Course *getCourse(int id);
    QVector<Student *> getAllStudents();
    QVector<Teacher *> getAllTeachers();
    QVector<Course *> getAllCourses();
    QVector<Course *> getTeacherCourses(int teacherId);
    QVector<Student *> getStudentsBySemester(int semester);
};

#endif // ACADEMICMANAGER_H