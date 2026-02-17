#ifndef ACADENCEMANAGER_HPP
#define ACADENCEMANAGER_HPP

#include <QString>
#include <QVector>
#include <QDate>
#include "student.hpp"
#include "teacher.hpp"
#include "course.hpp"
#include "habit.hpp"
#include "routine.hpp"
#include "exceptions.hpp"

// Classes replacing structs for OOP compliance
class Notice
{
private:
    QString date;
    QString author;
    QString content;

public:
    Notice(QString d, QString a, QString c) : date(d), author(a), content(c) {}
    QString getDate() const { return date; }
    QString getAuthor() const { return author; }
    QString getContent() const { return content; }
};

class Task
{
private:
    int id;
    QString description;
    bool isCompleted;

public:
    Task(int id, QString desc, bool completed) : id(id), description(desc), isCompleted(completed) {}
    int getId() const { return id; }
    QString getDescription() const { return description; }
    bool getIsCompleted() const { return isCompleted; }
    void setCompleted(bool val) { isCompleted = val; }

    // Overloading Binary Operator (Comparison)
    bool operator==(const Task &other) const
    {
        return this->id == other.id && this->description == other.description &&
               this->isCompleted == other.isCompleted;
    }
};

class Assessment
{
private:
    int id;
    int courseId;
    QString courseName;
    QString title;
    QString type;
    QString date;
    int maxMarks;

public:
    Assessment(int id, int cid, QString cname, QString t, QString type, QString d, int max)
        : id(id), courseId(cid), courseName(cname), title(t), type(type), date(d), maxMarks(max) {}

    int getId() const { return id; }
    int getCourseId() const { return courseId; }
    QString getCourseName() const { return courseName; }
    QString getTitle() const { return title; }
    QString getType() const { return type; }
    QString getDate() const { return date; }
    int getMaxMarks() const { return maxMarks; }
};

class AttendanceRecord
{
private:
    QString courseName;
    int totalClasses;
    int attendedClasses;
    double totalMarksObtained;
    double totalMaxMarks;

public:
    AttendanceRecord(QString cname, int total, int attended, double marks, double max)
        : courseName(cname), totalClasses(total), attendedClasses(attended), totalMarksObtained(marks), totalMaxMarks(max) {}

    QString getCourseName() const { return courseName; }
    int getTotalClasses() const { return totalClasses; }
    int getAttendedClasses() const { return attendedClasses; }
    double getTotalMarksObtained() const { return totalMarksObtained; }
    double getTotalMaxMarks() const { return totalMaxMarks; }
};

class Query
{
private:
    int id;
    int studentId;
    QString studentName;
    QString question;
    QString answer;

public:
    Query(int id, int sid, QString sname, QString q, QString a)
        : id(id), studentId(sid), studentName(sname), question(q), answer(a) {}

    int getId() const { return id; }
    int getStudentId() const { return studentId; }
    QString getStudentName() const { return studentName; }
    QString getQuestion() const { return question; }
    QString getAnswer() const { return answer; }
};

class DailyPrayerStatus
{
private:
    bool fajr;
    bool dhuhr;
    bool asr;
    bool maghrib;
    bool isha;

public:
    DailyPrayerStatus(bool f, bool d, bool a, bool m, bool i)
        : fajr(f), dhuhr(d), asr(a), maghrib(m), isha(i) {}

    bool getFajr() const { return fajr; }
    bool getDhuhr() const { return dhuhr; }
    bool getAsr() const { return asr; }
    bool getMaghrib() const { return maghrib; }
    bool getIsha() const { return isha; }
};

class AcadenceManager
{
public:
    AcadenceManager();

    static QVector<QStringList> readCsv(const QString &filename);
    static void writeCsv(const QString &filename, const QVector<QStringList> &data);

    QString login(const QString &username, const QString &password, int &userId);
    bool changePassword(int userId, const QString &role, const QString &oldPass, const QString &newPass);

    // Dashboard
    QVector<Notice> getNotices();
    void addNotice(const QString &content, const QString &author);
    QString getNextClass(int userId);
    QString getDashboardStats(int userId, QString role);

    // Users
    Student *getStudent(int id);
    Teacher *getTeacher(int id);

    // Planner
    QVector<Task> getTasks(int userId);
    void addTask(int userId, const QString &description);
    void completeTask(int taskId, bool status);

    // Habits
    DailyPrayerStatus getDailyPrayers(int userId, QString date);
    void updateDailyPrayer(int userId, QString date, QString prayer, bool status);
    QVector<Habit *> getHabits(int userId);
    void addHabit(Habit *h);
    void updateHabit(Habit *h);
    void deleteHabit(int id);

    // Routine
    QVector<RoutineSession> getRoutineForDay(QString day, int semester = -1);
    void addRoutineItem(QString day, QString start, QString end, QString code, QString name, QString room, QString instructor, int semester);

    // Academics / Teacher Tools
    QVector<Course *> getTeacherCourses(int teacherId);
    Course *getCourse(int id);
    QVector<Assessment> getAssessments();
    void addAssessment(int courseId, QString title, QString type, QString date, int maxMarks);

    QVector<AttendanceRecord> getStudentAttendance(int studentId);
    QVector<Student *> getStudentsBySemester(int semester);

    double getGrade(int studentId, int assessmentId);
    void addGrade(int studentId, int assessmentId, double marks);

    QVector<QString> getCourseDates(int courseId);
    bool isPresent(int courseId, int studentId, QString date);
    void markAttendance(int courseId, int studentId, QString date, bool present);

    // Queries
    QVector<Query> getQueries(int userId, QString role);
    void addQuery(int userId, QString question);
    void answerQuery(int queryId, QString answer);
};

// Function Template
/**
 * @brief Generic function to find an object by ID in a QVector.
 * @tparam T The type of object (must have getId() method).
 */
template <typename T>
T *findById(const QVector<T *> &list, int id)
{
    for (T *item : list)
    {
        if (item->getId() == id)
            return item;
    }
    return nullptr;
}

#endif // ACADENCEMANAGER_HPP