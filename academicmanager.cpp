/**
 * @file academicmanager.cpp
 * @brief Core logic controller for the Acadence application.
 * Handles File I/O, Authentication, and Data Management.
 */
#include "academicmanager.hpp"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>

// Helper functions for CSV handling
/**
 * @brief Reads a CSV file and parses it into a vector of string lists.
 * Handles quoted fields containing commas or newlines.
 * @param filename The path to the CSV file.
 * @return A QVector of QStringList, where each inner list is a row.
 */
QVector<QStringList> AcadenceManager::readCsv(const QString &filename)
{
    QVector<QStringList> data;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw Acadence::FileException("Failed to open file for reading: " + filename);
    }
    else
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            if (line.trimmed().isEmpty())
                continue;

            QStringList row;
            QString currentField;
            bool inQuotes = false;
            for (int i = 0; i < line.length(); ++i)
            {
                QChar c = line[i];
                if (c == '"')
                {
                    // Handle escaped quotes ("") inside quoted fields
                    if (inQuotes && i + 1 < line.length() && line[i + 1] == '"')
                    {
                        currentField += '"'; // Escaped quote
                        i++;
                    }
                    else
                    {
                        // Toggle quote state
                        inQuotes = !inQuotes;
                    }
                }
                else if (c == ',' && !inQuotes)
                {
                    // Field separator found outside of quotes
                    row.append(currentField.trimmed());
                    currentField.clear();
                }
                else
                {
                    currentField += c;
                }
            }
            row.append(currentField.trimmed());
            data.append(row);
        }
        file.close();
    }
    return data;
}

/**
 * @brief Escapes a string for CSV format.
 * Wraps text in quotes if it contains commas, quotes, or newlines.
 */
static QString escapeCsv(const QString &val)
{
    if (val.contains(',') || val.contains('"') || val.contains('\n'))
    {
        QString temp = val;
        temp.replace("\"", "\"\"");
        return "\"" + temp + "\"";
    }
    return val;
}

static void appendCsv(const QString &filename, const QStringList &fields)
{
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        throw Acadence::FileException("Failed to open file for appending: " + filename);
    }
    else
    {
        QTextStream out(&file);
        QStringList escaped;
        for (const QString &f : fields)
            escaped << escapeCsv(f);
        out << escaped.join(",") << "\n";
        file.close();
    }
}

void AcadenceManager::writeCsv(const QString &filename, const QVector<QStringList> &data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        throw Acadence::FileException("Failed to open file for writing: " + filename);
    }
    else
    {
        QTextStream out(&file);
        for (const auto &row : data)
        {
            QStringList escapedRow;
            for (const QString &field : row)
                escapedRow << escapeCsv(field);
            out << escapedRow.join(",") << "\n";
        }
        file.close();
    }
}

AcadenceManager::AcadenceManager()
{
    // Constructor is intentionally empty.
}

QString AcadenceManager::login(const QString &username, const QString &password, int &userId)
{
    // 1. Check Admins
    QVector<QStringList> admins = readCsv("admins.csv");
    // Format: ID,Username,Password,Name,Email
    for (const auto &row : admins)
    {
        if (row.size() >= 3 && row[1] == username && row[2] == password)
        {
            userId = row[0].toInt();
            return "Admin";
        }
    }

    // 2. Check Students
    QVector<QStringList> students = readCsv("students.csv");
    // Format: ID,Name,Email,Username,Password,...
    for (const auto &row : students)
    {
        if (row.size() >= 5 && row[3] == username && row[4] == password)
        {
            userId = row[0].toInt();
            return "Student";
        }
    }

    // 3. Check Teachers
    QVector<QStringList> teachers = readCsv("teachers.csv");
    // Format: ID,Name,Email,Username,Password,...
    for (const auto &row : teachers)
    {
        if (row.size() >= 5 && row[3] == username && row[4] == password)
        {
            userId = row[0].toInt();
            return "Teacher";
        }
    }
    return ""; // Not found
}

bool AcadenceManager::changePassword(int userId, const QString &role, const QString &oldPass, const QString &newPass)
{
    QString filename;
    int passIndex = 4; // Default for Student/Teacher (ID, Name, Email, Username, Password)

    if (role == "Student")
    {
        filename = "students.csv";
    }
    else if (role == "Teacher")
    {
        filename = "teachers.csv";
    }
    else if (role == "Admin")
    {
        filename = "admins.csv";
        passIndex = 2; // Admin: ID, Username, Password
    }
    else
    {
        return false;
    }

    QVector<QStringList> data = readCsv(filename);
    bool found = false;

    for (auto &row : data)
    {
        if (row.size() > passIndex && row[0].toInt() == userId)
        {
            if (row[passIndex] == oldPass)
            {
                row[passIndex] = newPass;
                found = true;
            }
            else
            {
                throw Acadence::Exception("Old password does not match.");
            }
            break;
        }
    }

    if (found)
    {
        writeCsv(filename, data);
        return true;
    }
    throw Acadence::Exception("User not found.");
}

// Dashboard
QVector<Notice> AcadenceManager::getNotices()
{
    QVector<Notice> notices;
    QVector<QStringList> data = readCsv("notices.csv");
    // Format: Date,Author,Content
    for (const auto &row : data)
    {
        if (row.size() >= 3)
        {
            notices.append(Notice(row[0], row[1], row[2]));
        }
    }
    return notices;
}

void AcadenceManager::addNotice(const QString &content, const QString &author)
{
    QString date = QDate::currentDate().toString("yyyy-MM-dd");
    appendCsv("notices.csv", {date, author, content});
}

QString AcadenceManager::getNextClass(int userId)
{
    // Simple logic: Get routine for today, find first class after current time
    Student *s = getStudent(userId);
    if (!s)
        return "No Data";

    int semester = s->getSemester();
    delete s;

    QString day = QDate::currentDate().toString("dddd");
    QVector<RoutineSession> routine = getRoutineForDay(day, semester);
    QTime now = QTime::currentTime();

    for (const auto &item : routine)
    {
        QTime start = QTime::fromString(item.getStartTime(), "HH:mm");
        if (start > now)
        {
            return item.getCourseCode() + " (" + item.getStartTime() + ")";
        }
    }
    return "No more classes";
}

QString AcadenceManager::getDashboardStats(int userId, QString role)
{
    if (role == "Student")
    {
        // Calculate GPA or Attendance
        return "GPA: " + QString::number(3.5); // Placeholder calculation
    }
    else if (role == "Teacher")
    {
        return "Active Courses: " + QString::number(getTeacherCourses(userId).size());
    }
    return "System Active";
}

// Users
Student *AcadenceManager::getStudent(int id)
{
    QVector<QStringList> data = readCsv("students.csv");
    // Format: ID,Name,Email,Username,Password,Dept,Batch,Sem,DateAdmission,CGPA
    for (const auto &row : data)
    {
        if (row.size() >= 8 && row[0].toInt() == id) // Basic fields check
        {
            Student *s = new Student(row[0].toInt(), row[1], row[2], row[5], row[6], row[7].toInt());
            s->setUsername(row[3]);
            s->setPassword(row[4]);

            // Populate new fields if available in CSV
            if (row.size() >= 9)
                s->setDateAdmission(QDate::fromString(row[8], Qt::ISODate));
            if (row.size() >= 10)
                s->setGpa(row[9].toDouble());

            return s;
        }
    }
    return nullptr;
}

Teacher *AcadenceManager::getTeacher(int id)
{
    QVector<QStringList> data = readCsv("teachers.csv");
    // Format: ID,Name,Email,Username,Password,Dept,Designation,Salary
    for (const auto &row : data)
    {
        if (row.size() >= 7 && row[0].toInt() == id) // Basic fields check
        {
            Teacher *t = new Teacher(row[0].toInt(), row[1], row[2], row[5], row[6]);
            t->setUsername(row[3]);
            t->setPassword(row[4]);

            if (row.size() >= 8)
                t->setSalary(row[7].toDouble());

            return t;
        }
    }
    return nullptr;
}

// Planner
QVector<Task> AcadenceManager::getTasks(int userId)
{
    QVector<Task> tasks;
    QVector<QStringList> data = readCsv("tasks.csv");
    // Format: ID,UserID,Desc,IsCompleted
    for (const auto &row : data)
    {
        if (row.size() >= 4 && row[1].toInt() == userId)
        {
            tasks.append(Task(row[0].toInt(), row[2], (row[3] == "1")));
        }
    }
    return tasks;
}

void AcadenceManager::addTask(int userId, const QString &description)
{
    // Generate ID
    QVector<QStringList> data = readCsv("tasks.csv");
    int maxId = 0;
    for (const auto &row : data)
        if (row.size() > 0)
            maxId = std::max(maxId, row[0].toInt());

    appendCsv("tasks.csv", {QString::number(maxId + 1), QString::number(userId), description, "0"});
}

void AcadenceManager::completeTask(int taskId, bool status)
{
    QVector<QStringList> data = readCsv("tasks.csv");
    for (auto &row : data)
    {
        if (row.size() >= 4 && row[0].toInt() == taskId)
        {
            row[3] = status ? "1" : "0";
        }
    }
    writeCsv("tasks.csv", data);
}

// Habits
DailyPrayerStatus AcadenceManager::getDailyPrayers(int userId, QString date)
{
    QVector<QStringList> data = readCsv("prayers.csv");
    // Format: UserID,Date,Fajr,Dhuhr,Asr,Maghrib,Isha
    for (const auto &row : data)
    {
        if (row.size() >= 7 && row[0].toInt() == userId && row[1] == date)
        {
            return DailyPrayerStatus(row[2] == "1", row[3] == "1", row[4] == "1", row[5] == "1", row[6] == "1");
        }
    }
    return DailyPrayerStatus(false, false, false, false, false);
}

void AcadenceManager::updateDailyPrayer(int userId, QString date, QString prayer, bool status)
{
    QVector<QStringList> data = readCsv("prayers.csv");
    bool found = false;

    int prayerIdx = -1;
    if (prayer == "fajr")
        prayerIdx = 2;
    else if (prayer == "dhuhr")
        prayerIdx = 3;
    else if (prayer == "asr")
        prayerIdx = 4;
    else if (prayer == "maghrib")
        prayerIdx = 5;
    else if (prayer == "isha")
        prayerIdx = 6;

    for (auto &row : data)
    {
        if (row.size() >= 7 && row[0].toInt() == userId && row[1] == date)
        {
            if (prayerIdx != -1)
                row[prayerIdx] = status ? "1" : "0";
            found = true;
        }
    }

    if (!found)
    {
        QStringList newRow = {QString::number(userId), date, "0", "0", "0", "0", "0"};
        if (prayerIdx != -1)
            newRow[prayerIdx] = status ? "1" : "0";
        data.append(newRow);
    }
    writeCsv("prayers.csv", data);
}

QVector<Habit *> AcadenceManager::getHabits(int userId)
{
    QVector<Habit *> habits;
    QVector<QStringList> data = readCsv("habits.csv");
    // Format: ID,UserID,Name,Type,Freq,Target,Current,Streak,LastDate,IsCompleted,Unit
    for (const auto &row : data)
    {
        if (row.size() >= 11 && row[1].toInt() == userId)
        {
            int id = row[0].toInt();
            QString name = row[2];
            HabitType type = (row[3] == "Duration") ? HabitType::DURATION : HabitType::COUNT;
            Frequency freq = (row[4] == "Daily") ? Frequency::DAILY : Frequency::WEEKLY;
            int target = row[5].toInt();
            int current = row[6].toInt();
            int streak = row[7].toInt();
            QDate lastDate = QDate::fromString(row[8], Qt::ISODate);
            bool isComp = (row[9] == "1");
            QString unit = row[10];

            Habit *h = nullptr;
            if (type == HabitType::DURATION)
            {
                auto *dh = new DurationHabit(id, userId, name, freq, target);
                dh->currentMinutes = current;
                h = dh;
            }
            else
            {
                auto *ch = new CountHabit(id, userId, name, freq, target, unit);
                ch->currentCount = current;
                h = ch;
            }

            h->streak = streak;
            h->lastUpdated = lastDate;
            h->isCompleted = isComp;

            // Check reset logic immediately on load
            if (h->checkReset())
            {
                // If reset happened, we should technically save it back, but for now just display
            }
            habits.append(h);
        }
    }
    return habits;
}

void AcadenceManager::addHabit(Habit *h)
{
    QVector<QStringList> data = readCsv("habits.csv");
    int maxId = 0;
    for (const auto &row : data)
        if (row.size() > 0)
            maxId = std::max(maxId, row[0].toInt());
    h->id = maxId + 1;

    QString typeStr = (h->type == HabitType::DURATION) ? "Duration" : "Count";
    QString freqStr = (h->frequency == Frequency::DAILY) ? "Daily" : "Weekly";
    QString unit = "";
    int target = 0;

    if (auto *dh = dynamic_cast<DurationHabit *>(h))
        target = dh->targetMinutes;
    else if (auto *ch = dynamic_cast<CountHabit *>(h))
    {
        target = ch->targetCount;
        unit = ch->unit;
    }

    appendCsv("habits.csv", {QString::number(h->id), QString::number(h->studentId), h->name, typeStr, freqStr,
                             QString::number(target), "0", "0", QDate::currentDate().toString(Qt::ISODate), "0", unit});
}

void AcadenceManager::updateHabit(Habit *h)
{
    QVector<QStringList> data = readCsv("habits.csv");
    for (auto &row : data)
    {
        if (row.size() >= 11 && row[0].toInt() == h->id)
        {
            int current = 0;
            if (auto *dh = dynamic_cast<DurationHabit *>(h))
                current = dh->currentMinutes;
            else if (auto *ch = dynamic_cast<CountHabit *>(h))
                current = ch->currentCount;

            row[6] = QString::number(current);
            row[7] = QString::number(h->streak);
            row[8] = h->lastUpdated.toString(Qt::ISODate);
            row[9] = h->isCompleted ? "1" : "0";
        }
    }
    writeCsv("habits.csv", data);
}

void AcadenceManager::deleteHabit(int id)
{
    QVector<QStringList> data = readCsv("habits.csv");
    QVector<QStringList> newData;
    for (const auto &row : data)
    {
        if (row.size() > 0 && row[0].toInt() != id)
        {
            newData.append(row);
        }
    }
    writeCsv("habits.csv", newData);
}

// Routine
QVector<RoutineSession> AcadenceManager::getRoutineForDay(QString day, int semester)
{
    WeeklyRoutine weeklyRoutine;
    QVector<QStringList> data = readCsv("routine.csv");
    // Format: Day,Start,End,Code,Name,Room,Instructor,Semester
    for (const auto &row : data)
    {
        if (row.size() >= 8)
        {
            if (semester == -1 || row[7].toInt() == semester)
            {
                weeklyRoutine.addSession(RoutineSession(row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7].toInt()));
            }
        }
    }
    return weeklyRoutine.getSessionsForDay(day);
}

void AcadenceManager::addRoutineItem(QString day, QString start, QString end, QString code, QString name, QString room, QString instructor, int semester)
{
    appendCsv("routine.csv", {day, start, end, code, name, room, instructor, QString::number(semester)});
}

// Academics / Teacher Tools
QVector<Course *> AcadenceManager::getTeacherCourses(int teacherId)
{
    QVector<Course *> courses;
    QVector<QStringList> data = readCsv("courses.csv");
    // Format: ID,Code,Name,TeacherID,Semester,Credits
    for (const auto &row : data)
    {
        if (row.size() >= 6 && row[3].toInt() == teacherId)
        {
            courses.append(new Course(row[0].toInt(), row[1], row[2], row[3].toInt(), row[4].toInt(), row[5].toInt()));
        }
    }
    return courses;
}

Course *AcadenceManager::getCourse(int id)
{
    QVector<QStringList> data = readCsv("courses.csv");
    for (const auto &row : data)
    {
        if (row.size() >= 6 && row[0].toInt() == id)
        {
            return new Course(row[0].toInt(), row[1], row[2], row[3].toInt(), row[4].toInt(), row[5].toInt());
        }
    }
    return nullptr;
}

QVector<Assessment> AcadenceManager::getAssessments()
{
    QVector<Assessment> list;
    QVector<QStringList> data = readCsv("assessments.csv");
    // Format: ID,CourseID,Title,Type,Date,MaxMarks
    for (const auto &row : data)
    {
        if (row.size() >= 6)
        {
            // Need course name
            QString courseName = "Unknown";
            Course *c = getCourse(row[1].toInt());
            if (c)
            {
                courseName = c->getName();
                delete c;
            }

            list.append(Assessment(row[0].toInt(), row[1].toInt(), courseName, row[2], row[3], row[4], row[5].toInt()));
        }
    }
    return list;
}

void AcadenceManager::addAssessment(int courseId, QString title, QString type, QString date, int maxMarks)
{
    QVector<QStringList> data = readCsv("assessments.csv");
    int maxId = 0;
    for (const auto &row : data)
        if (row.size() > 0)
            maxId = std::max(maxId, row[0].toInt());

    appendCsv("assessments.csv", {QString::number(maxId + 1), QString::number(courseId), title, type, date, QString::number(maxMarks)});
}

QVector<AttendanceRecord> AcadenceManager::getStudentAttendance(int studentId)
{
    QVector<AttendanceRecord> records;
    Student *s = getStudent(studentId);
    if (!s)
        return records;
    int semester = s->getSemester();
    delete s;

    // Get all courses for this semester
    QVector<QStringList> courseData = readCsv("courses.csv");
    QVector<int> courseIds;
    QMap<int, QString> courseNames;
    for (const auto &row : courseData)
    {
        if (row.size() >= 6 && row[4].toInt() == semester)
        {
            int cid = row[0].toInt();
            courseIds.append(cid);
            courseNames[cid] = row[2];
        }
    }

    // Process each course
    QVector<QStringList> attData = readCsv("attendance.csv");
    QVector<QStringList> gradeData = readCsv("grades.csv");
    QVector<Assessment> assessments = getAssessments();

    for (int cid : courseIds)
    {
        // Attendance
        QSet<QString> uniqueDates;
        int attended = 0;
        for (const auto &row : attData)
        {
            if (row.size() >= 4 && row[0].toInt() == cid)
            {
                uniqueDates.insert(row[2]);
                if (row[1].toInt() == studentId && row[3] == "1")
                {
                    attended++;
                }
            }
        }

        int totalClasses = uniqueDates.size();
        int attendedClasses = attended;

        // Grades
        double totalMarksObtained = 0;
        double totalMaxMarks = 0;
        for (const auto &a : assessments)
        {
            if (a.getCourseId() == cid)
            {
                totalMaxMarks += a.getMaxMarks();
                // Find grade
                for (const auto &grow : gradeData)
                {
                    if (grow.size() >= 3 && grow[0].toInt() == studentId && grow[1].toInt() == a.getId())
                    {
                        totalMarksObtained += grow[2].toDouble();
                        break;
                    }
                }
            }
        }
        records.append(AttendanceRecord(courseNames[cid], totalClasses, attendedClasses, totalMarksObtained, totalMaxMarks));
    }
    return records;
}

QVector<Student *> AcadenceManager::getStudentsBySemester(int semester)
{
    QVector<Student *> list;
    QVector<QStringList> data = readCsv("students.csv");
    for (const auto &row : data)
    {
        if (row.size() >= 6 && row[5].toInt() == semester)
        {
            list.append(new Student(row[0].toInt(), row[1], row[2], row[3], row[4], row[5].toInt()));
        }
    }
    return list;
}

double AcadenceManager::getGrade(int studentId, int assessmentId)
{
    QVector<QStringList> data = readCsv("grades.csv");
    for (const auto &row : data)
    {
        if (row.size() >= 3 && row[0].toInt() == studentId && row[1].toInt() == assessmentId)
        {
            return row[2].toDouble();
        }
    }
    return -1.0;
}

void AcadenceManager::addGrade(int studentId, int assessmentId, double marks)
{
    QVector<QStringList> data = readCsv("grades.csv");
    bool found = false;
    for (auto &row : data)
    {
        if (row.size() >= 3 && row[0].toInt() == studentId && row[1].toInt() == assessmentId)
        {
            row[2] = QString::number(marks);
            found = true;
        }
    }
    if (!found)
    {
        data.append({QString::number(studentId), QString::number(assessmentId), QString::number(marks)});
    }
    writeCsv("grades.csv", data);
}

QVector<QString> AcadenceManager::getCourseDates(int courseId)
{
    QSet<QString> dates;
    QVector<QStringList> data = readCsv("attendance.csv");
    for (const auto &row : data)
    {
        if (row.size() >= 4 && row[0].toInt() == courseId)
        {
            dates.insert(row[2]);
        }
    }
    QVector<QString> list = dates.values();
    std::sort(list.begin(), list.end());
    return list;
}

bool AcadenceManager::isPresent(int courseId, int studentId, QString date)
{
    QVector<QStringList> data = readCsv("attendance.csv");
    for (const auto &row : data)
    {
        if (row.size() >= 4 && row[0].toInt() == courseId && row[1].toInt() == studentId && row[2] == date)
        {
            return row[3] == "1";
        }
    }
    return false;
}

void AcadenceManager::markAttendance(int courseId, int studentId, QString date, bool present)
{
    QVector<QStringList> data = readCsv("attendance.csv");
    bool found = false;
    for (auto &row : data)
    {
        if (row.size() >= 4 && row[0].toInt() == courseId && row[1].toInt() == studentId && row[2] == date)
        {
            row[3] = present ? "1" : "0";
            found = true;
        }
    }
    if (!found)
    {
        data.append({QString::number(courseId), QString::number(studentId), date, present ? "1" : "0"});
    }
    writeCsv("attendance.csv", data);
}

// Queries
QVector<Query> AcadenceManager::getQueries(int userId, QString role)
{
    QVector<Query> list;
    QVector<QStringList> data = readCsv("queries.csv");
    // Format: ID,StudentID,Question,Answer
    for (const auto &row : data)
    {
        if (row.size() >= 4)
        {
            if (role == "Teacher" || role == "Admin" || row[1].toInt() == userId)
            {
                QString sName = "Student";
                Student *s = getStudent(row[1].toInt());
                if (s)
                {
                    sName = s->getName();
                    delete s;
                }

                list.append(Query(row[0].toInt(), row[1].toInt(), sName, row[2], row[3]));
            }
        }
    }
    return list;
}

void AcadenceManager::addQuery(int userId, QString question)
{
    QVector<QStringList> data = readCsv("queries.csv");
    int maxId = 0;
    for (const auto &row : data)
        if (row.size() > 0)
            maxId = std::max(maxId, row[0].toInt());

    appendCsv("queries.csv", {QString::number(maxId + 1), QString::number(userId), question, ""});
}

void AcadenceManager::answerQuery(int queryId, QString answer)
{
    QVector<QStringList> data = readCsv("queries.csv");
    QVector<QString> lines;
    for (auto &row : data)
    {
        if (row.size() >= 4 && row[0].toInt() == queryId)
        {
            row[3] = answer;
        }
    }
    writeCsv("queries.csv", data);
}