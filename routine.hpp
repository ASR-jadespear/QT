#ifndef ROUTINE_HPP
#define ROUTINE_HPP

#include <QString>
#include <QVector>

class RoutineSession
{
private:
    QString day;
    QString startTime;
    QString endTime;
    QString courseCode;
    QString courseName;
    QString room;
    QString instructor;
    int semester;

public:
    RoutineSession(QString day, QString start, QString end, QString code, QString name, QString room, QString instructor, int sem);

    QString getDay() const;
    QString getStartTime() const;
    QString getEndTime() const;
    QString getCourseCode() const;
    QString getCourseName() const;
    QString getRoom() const;
    QString getInstructor() const;
    int getSemester() const;
};

class WeeklyRoutine
{
private:
    QVector<RoutineSession> sessions;

public:
    WeeklyRoutine();
    void addSession(const RoutineSession &session);
    QVector<RoutineSession> getSessionsForDay(const QString &day) const;
    QVector<RoutineSession> getAllSessions() const;
};

#endif // ROUTINE_HPP