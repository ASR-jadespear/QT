#include "routine.hpp"

RoutineSession::RoutineSession(QString day, QString start, QString end, QString code, QString name, QString room, QString instructor, int sem)
    : day(day), startTime(start), endTime(end), courseCode(code), courseName(name), room(room), instructor(instructor), semester(sem) {}

QString RoutineSession::getDay() const { return day; }
QString RoutineSession::getStartTime() const { return startTime; }
QString RoutineSession::getEndTime() const { return endTime; }
QString RoutineSession::getCourseCode() const { return courseCode; }
QString RoutineSession::getCourseName() const { return courseName; }
QString RoutineSession::getRoom() const { return room; }
QString RoutineSession::getInstructor() const { return instructor; }
int RoutineSession::getSemester() const { return semester; }

WeeklyRoutine::WeeklyRoutine() {}

void WeeklyRoutine::addSession(const RoutineSession &session)
{
    sessions.append(session);
}

QVector<RoutineSession> WeeklyRoutine::getSessionsForDay(const QString &day) const
{
    QVector<RoutineSession> dailySessions;
    for (const auto &session : sessions)
    {
        if (session.getDay() == day)
        {
            dailySessions.append(session);
        }
    }
    return dailySessions;
}

QVector<RoutineSession> WeeklyRoutine::getAllSessions() const
{
    return sessions;
}