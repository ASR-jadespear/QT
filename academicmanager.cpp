#include "academicmanager.hpp"
#include <QDebug>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QFile>

AcademicManager::AcademicManager()
{
    dbConnected = false;

    // Check for DB in the application directory (Portable Mode)
    QString appDir = QCoreApplication::applicationDirPath();
    QString localDbPath = QDir(appDir).filePath("school_system.db");
    QString dbPath;

    if (QFile::exists(localDbPath))
    {
        dbPath = localDbPath;
    }
    else
    {
        // Use standard writable location for cross-platform compatibility
        QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
        QDir dir(dataPath);
        if (!dir.exists())
            dir.mkpath(".");
        dbPath = dir.filePath("school_system.db");
    }

    // Automatically connect on startup
    connectToDatabase(dbPath);
}

AcademicManager::~AcademicManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool AcademicManager::connectToDatabase(QString path)
{
    // Add the SQLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);

    if (!db.open())
    {
        qDebug() << "Error: Connection failed -" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected at:" << path;
    dbConnected = true;
    createTables(); // Ensure tables exist
    return true;
}

/**
 * @brief Initializes the database schema.
 * Includes table creation and schema migrations for backward compatibility.
 */
void AcademicManager::createTables()
{
    QSqlQuery query;
    // Create Students Table
    QString createStudentTable =
        "CREATE TABLE IF NOT EXISTS Students ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "email TEXT, "
        "gpa REAL, "
        "credits INTEGER, "
        "department TEXT, "
        "batch TEXT, "
        "semester INTEGER, "
        "password TEXT DEFAULT '1234')"; // Default password for MVP

    if (!query.exec(createStudentTable))
    {
        qDebug() << "Error creating table:" << query.lastError();
    }

    // Migration: Ensure password column exists for older DB versions
    query.exec("ALTER TABLE Students ADD COLUMN password TEXT DEFAULT '1234'");
    query.exec("ALTER TABLE Students ADD COLUMN department TEXT");
    query.exec("ALTER TABLE Students ADD COLUMN batch TEXT");
    query.exec("ALTER TABLE Students ADD COLUMN semester INTEGER DEFAULT 1");

    // Create Teachers Table (Modernizing the backend)
    QString createTeacherTable =
        "CREATE TABLE IF NOT EXISTS Teachers ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "email TEXT, "
        "department TEXT, "
        "designation TEXT, "
        "password TEXT DEFAULT 'admin')";
    if (!query.exec(createTeacherTable))
    {
        qDebug() << "Error creating teacher table:" << query.lastError();
    }

    // Migration: Ensure password column exists for older DB versions
    query.exec("ALTER TABLE Teachers ADD COLUMN password TEXT DEFAULT 'admin'");
    query.exec("ALTER TABLE Teachers ADD COLUMN department TEXT");
    query.exec("ALTER TABLE Teachers ADD COLUMN designation TEXT");

    // Admins Table
    query.exec("CREATE TABLE IF NOT EXISTS Admins ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "username TEXT NOT NULL, "
               "password TEXT DEFAULT 'admin')");

    // Cleanup: Remove old 'admin' from Teachers if exists
    query.exec("DELETE FROM Teachers WHERE name = 'admin'");

    // Create Courses Table
    QString createCourseTable =
        "CREATE TABLE IF NOT EXISTS Courses ("
        "id INTEGER PRIMARY KEY, "
        "code TEXT, "
        "name TEXT NOT NULL, "
        "teacher_id INTEGER, "
        "semester INTEGER, "
        "credits INTEGER)";
    if (!query.exec(createCourseTable))
    {
        qDebug() << "Error creating course table:" << query.lastError();
    }
    query.exec("ALTER TABLE Courses ADD COLUMN code TEXT");
    query.exec("ALTER TABLE Courses ADD COLUMN teacher_id INTEGER");
    query.exec("ALTER TABLE Courses ADD COLUMN semester INTEGER");

    // Tasks (Planner)
    query.exec("CREATE TABLE IF NOT EXISTS Tasks ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "student_id INTEGER, "
               "description TEXT, "
               "completed INTEGER)");

    // Habits
    // Drop old table if it exists to ensure schema consistency for the new polymorphic habit system.
    query.exec("DROP TABLE IF EXISTS Habits");

    query.exec("CREATE TABLE IF NOT EXISTS Habits ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "student_id INTEGER, "
               "name TEXT, "
               "type INTEGER, "
               "frequency INTEGER, "
               "target INTEGER, "
               "unit TEXT, "
               "streak INTEGER, "
               "last_updated TEXT, "
               "is_completed INTEGER, "
               "current_value TEXT)");

    // Daily Prayers Table
    query.exec("CREATE TABLE IF NOT EXISTS DailyPrayers ("
               "student_id INTEGER, "
               "date TEXT, "
               "fajr INTEGER DEFAULT 0, "
               "dhuhr INTEGER DEFAULT 0, "
               "asr INTEGER DEFAULT 0, "
               "maghrib INTEGER DEFAULT 0, "
               "isha INTEGER DEFAULT 0, "
               "PRIMARY KEY (student_id, date))");

    // Notices
    query.exec("CREATE TABLE IF NOT EXISTS Notices ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "content TEXT, "
               "date TEXT, "
               "author TEXT)");

    // Routine
    query.exec("CREATE TABLE IF NOT EXISTS Routine ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "day TEXT, "
               "time TEXT, " // Acts as start_time
               "end_time TEXT, "
               "course_code TEXT, "
               "course_name TEXT, "
               "room TEXT, "
               "instructor TEXT, "
               "semester INTEGER)");

    // Migration: Add new Routine columns
    query.exec("ALTER TABLE Routine ADD COLUMN end_time TEXT");
    query.exec("ALTER TABLE Routine ADD COLUMN course_code TEXT");
    query.exec("ALTER TABLE Routine ADD COLUMN instructor TEXT");
    query.exec("ALTER TABLE Routine ADD COLUMN semester INTEGER");

    // Assessments
    query.exec("CREATE TABLE IF NOT EXISTS Assessments ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "course_id INTEGER, "
               "title TEXT, "
               "type TEXT, "
               "date TEXT, "
               "max_marks INTEGER)");

    // Grades
    query.exec("CREATE TABLE IF NOT EXISTS Grades ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "student_id INTEGER, "
               "assessment_id INTEGER, "
               "marks REAL)");

    // Attendance
    query.exec("CREATE TABLE IF NOT EXISTS Attendance ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "student_id INTEGER, "
               "course_id INTEGER, "
               "date TEXT, "
               "status INTEGER)"); // 1 = Present, 0 = Absent

    // Queries
    query.exec("CREATE TABLE IF NOT EXISTS Queries ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "student_id INTEGER, "
               "question TEXT, "
               "answer TEXT, "
               "date TEXT)");
}

QString AcademicManager::login(QString name, QString password, int &outId)
{
    QSqlQuery query;

    // Check Admins
    query.prepare("SELECT id FROM Admins WHERE username = :name AND password = :pass");
    query.bindValue(":name", name);
    query.bindValue(":pass", password);
    if (query.exec() && query.next())
    {
        outId = query.value("id").toInt();
        return "Admin";
    }

    // Check Students
    query.prepare("SELECT id FROM Students WHERE name = :name AND password = :pass");
    query.bindValue(":name", name);
    query.bindValue(":pass", password);
    if (query.exec() && query.next())
    {
        outId = query.value("id").toInt();
        return "Student";
    }

    // Check Teachers
    query.prepare("SELECT id FROM Teachers WHERE name = :name AND password = :pass");
    query.bindValue(":name", name);
    query.bindValue(":pass", password);
    if (query.exec() && query.next())
    {
        outId = query.value("id").toInt();
        return "Teacher";
    }

    return "";
}

bool AcademicManager::addStudent(const Student &s)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Students (id, name, email, gpa, credits, department, batch, semester) "
                  "VALUES (:id, :name, :email, :gpa, :credits, :dept, :batch, :sem)");

    query.bindValue(":id", s.getId());
    query.bindValue(":name", s.getName());
    query.bindValue(":email", s.getEmail()); // Assumes getter exists
    query.bindValue(":gpa", s.calculateGPA());
    query.bindValue(":credits", 0); // Default or get from object
    query.bindValue(":dept", s.getDepartment());
    query.bindValue(":batch", s.getBatch());
    query.bindValue(":sem", s.getSemester());

    if (!query.exec())
    {
        qDebug() << "Add Student Failed:" << query.lastError();
        return false;
    }
    return true;
}

bool AcademicManager::updateStudent(const Student &s)
{
    QSqlQuery query;
    query.prepare("UPDATE Students SET name = :name, email = :email, gpa = :gpa, department = :dept, batch = :batch, semester = :sem WHERE id = :id");
    query.bindValue(":name", s.getName());
    query.bindValue(":email", s.getEmail());
    query.bindValue(":gpa", s.calculateGPA());
    query.bindValue(":dept", s.getDepartment());
    query.bindValue(":batch", s.getBatch());
    query.bindValue(":sem", s.getSemester());
    query.bindValue(":id", s.getId());
    return query.exec();
}

bool AcademicManager::removeStudent(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Students WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();
}

Student *AcademicManager::getStudent(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Students WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next())
    {
        int sId = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString email = query.value("email").toString();
        double gpa = query.value("gpa").toDouble();
        QString dept = query.value("department").toString();
        QString batch = query.value("batch").toString();
        int sem = query.value("semester").toInt();
        // Construct the object
        Student *s = new Student(sId, name, email, dept, batch, sem);
        s->setGpa(gpa);
        return s;
    }
    return nullptr; // Not found
}

QVector<Student *> AcademicManager::getAllStudents()
{
    QVector<Student *> list;
    QSqlQuery query("SELECT * FROM Students"); // Select all

    while (query.next())
    {
        int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        QString email = query.value("email").toString();
        double gpa = query.value("gpa").toDouble();
        QString dept = query.value("department").toString();
        QString batch = query.value("batch").toString();
        int sem = query.value("semester").toInt();

        Student *s = new Student(id, name, email, dept, batch, sem);
        s->setGpa(gpa);
        list.append(s);
    }
    return list;
}

bool AcademicManager::addQuery(int studentId, QString question)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Queries (student_id, question, date) VALUES (:sid, :q, date('now'))");
    query.bindValue(":sid", studentId);
    query.bindValue(":q", question);
    return query.exec();
}

bool AcademicManager::answerQuery(int queryId, QString answer)
{
    QSqlQuery query;
    query.prepare("UPDATE Queries SET answer = :a WHERE id = :id");
    query.bindValue(":a", answer);
    query.bindValue(":id", queryId);
    return query.exec();
}

QVector<Query> AcademicManager::getQueries(int userId, QString role)
{
    QVector<Query> list;
    QSqlQuery query;

    if (role == "Student")
    {
        query.prepare("SELECT Q.*, S.name FROM Queries Q JOIN Students S ON Q.student_id = S.id WHERE Q.student_id = :id ORDER BY Q.id DESC");
        query.bindValue(":id", userId);
    }
    else
    {
        // Teachers see all queries (or you could filter by unanswered)
        query.prepare("SELECT Q.*, S.name FROM Queries Q JOIN Students S ON Q.student_id = S.id ORDER BY Q.id DESC");
    }

    if (query.exec())
    {
        while (query.next())
        {
            list.push_back({query.value("id").toInt(),
                            query.value("student_id").toInt(),
                            query.value("name").toString(),
                            query.value("question").toString(),
                            query.value("answer").toString()});
        }
    }
    return list;
}

QString AcademicManager::getNextClass()
{
    QString day = QDate::currentDate().toString("dddd"); // e.g., "Monday"
    QString time = QTime::currentTime().toString("HH:mm");

    QSqlQuery query;
    // Find the first class today that is later than current time
    query.prepare("SELECT * FROM Routine WHERE day = :d AND time > :t ORDER BY time ASC LIMIT 1");
    query.bindValue(":d", day);
    query.bindValue(":t", time);

    if (query.exec() && query.next())
    {
        return query.value("time").toString() + " - " + query.value("course_name").toString() +
               " (" + query.value("room").toString() + ")";
    }
    return "";
}

QString AcademicManager::getDashboardStats(int userId, QString role)
{
    QString stats = "";
    QSqlQuery query;

    if (role == "Student")
    {
        // Count completed tasks
        query.prepare("SELECT COUNT(*) FROM Tasks WHERE student_id = :id AND completed = 1");
        query.bindValue(":id", userId);
        if (query.exec() && query.next())
        {
            stats += "Tasks Completed: " + query.value(0).toString() + "\n";
        }

        // Count habits logged today
        query.prepare("SELECT COUNT(*) FROM Habits WHERE student_id = :id AND date = date('now')");
        query.bindValue(":id", userId);
        if (query.exec() && query.next())
        {
            stats += "Habits Logged Today: " + query.value(0).toString();
        }
    }
    else
    {
        // Teacher stats
        stats = "Welcome to the Teacher Dashboard.";
    }
    return stats;
}

bool AcademicManager::addRoutineItem(QString day, QString startTime, QString endTime, QString courseCode, QString courseName, QString room, QString instructor, int semester)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Routine (day, time, end_time, course_code, course_name, room, instructor, semester) VALUES (:d, :t, :et, :cc, :cn, :r, :i, :sem)");
    query.bindValue(":d", day);
    query.bindValue(":t", startTime);
    query.bindValue(":et", endTime);
    query.bindValue(":cc", courseCode);
    query.bindValue(":cn", courseName);
    query.bindValue(":r", room);
    query.bindValue(":i", instructor);
    query.bindValue(":sem", semester);
    return query.exec();
}

QVector<RoutineItem> AcademicManager::getRoutineForDay(QString day, int semester)
{
    QVector<RoutineItem> list;
    QSqlQuery query;
    QString sql = "SELECT * FROM Routine WHERE day = :day";
    if (semester != -1)
        sql += " AND semester = :sem";
    sql += " ORDER BY time";
    query.prepare(sql);
    query.bindValue(":day", day);
    if (semester != -1)
        query.bindValue(":sem", semester);
    if (query.exec())
    {
        while (query.next())
        {
            list.push_back({query.value("id").toInt(),
                            query.value("day").toString(),
                            query.value("time").toString(),
                            query.value("end_time").toString(),
                            query.value("course_code").toString(),
                            query.value("course_name").toString(),
                            query.value("room").toString(),
                            query.value("instructor").toString(),
                            query.value("semester").toInt()});
        }
    }
    return list;
}

bool AcademicManager::addAssessment(int courseId, QString title, QString type, QString date, int maxMarks)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Assessments (course_id, title, type, date, max_marks) VALUES (:cid, :t, :type, :d, :mm)");
    query.bindValue(":cid", courseId);
    query.bindValue(":t", title);
    query.bindValue(":type", type);
    query.bindValue(":d", date);
    query.bindValue(":mm", maxMarks);
    return query.exec();
}

QVector<Assessment> AcademicManager::getAssessments(int courseId)
{
    QVector<Assessment> list;
    QSqlQuery query;
    QString sql = "SELECT A.*, C.name as course_name FROM Assessments A LEFT JOIN Courses C ON A.course_id = C.id";
    if (courseId != -1)
        sql += " WHERE A.course_id = :cid";

    query.prepare(sql);
    if (courseId != -1)
        query.bindValue(":cid", courseId);

    if (query.exec())
    {
        while (query.next())
        {
            list.push_back({query.value("id").toInt(), query.value("course_id").toInt(), query.value("course_name").toString(),
                            query.value("title").toString(), query.value("type").toString(), query.value("date").toString(), query.value("max_marks").toInt()});
        }
    }
    return list;
}

bool AcademicManager::addGrade(int studentId, int assessmentId, double marks)
{
    // Check if exists
    QSqlQuery check;
    check.prepare("SELECT id FROM Grades WHERE student_id = :sid AND assessment_id = :aid");
    check.bindValue(":sid", studentId);
    check.bindValue(":aid", assessmentId);
    if (check.exec() && check.next())
    {
        // Update
        QSqlQuery update;
        update.prepare("UPDATE Grades SET marks = :m WHERE id = :id");
        update.bindValue(":m", marks);
        update.bindValue(":id", check.value("id"));
        return update.exec();
    }
    else
    {
        // Insert
        QSqlQuery insert;
        insert.prepare("INSERT INTO Grades (student_id, assessment_id, marks) VALUES (:sid, :aid, :m)");
        insert.bindValue(":sid", studentId);
        insert.bindValue(":aid", assessmentId);
        insert.bindValue(":m", marks);
        return insert.exec();
    }
}

double AcademicManager::getGrade(int studentId, int assessmentId)
{
    QSqlQuery query;
    query.prepare("SELECT marks FROM Grades WHERE student_id = :sid AND assessment_id = :aid");
    query.bindValue(":sid", studentId);
    query.bindValue(":aid", assessmentId);
    if (query.exec() && query.next())
        return query.value("marks").toDouble();
    return -1.0;
}

bool AcademicManager::markAttendance(int courseId, int studentId, QString date, bool present)
{
    // Check if record exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT id FROM Attendance WHERE student_id = :sid AND course_id = :cid AND date = :d");
    checkQuery.bindValue(":sid", studentId);
    checkQuery.bindValue(":cid", courseId);
    checkQuery.bindValue(":d", date);

    if (checkQuery.exec() && checkQuery.next())
    {
        // Update existing record
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE Attendance SET status = :s WHERE id = :id");
        updateQuery.bindValue(":s", present ? 1 : 0);
        updateQuery.bindValue(":id", checkQuery.value("id"));
        return updateQuery.exec();
    }
    else
    {
        // Insert new record
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO Attendance (student_id, course_id, date, status) VALUES (:sid, :cid, :d, :s)");
        insertQuery.bindValue(":sid", studentId);
        insertQuery.bindValue(":cid", courseId);
        insertQuery.bindValue(":d", date);
        insertQuery.bindValue(":s", present ? 1 : 0);
        return insertQuery.exec();
    }
}

QVector<AttendanceRecord> AcademicManager::getStudentAttendance(int studentId)
{
    QVector<AttendanceRecord> list;
    // Get all courses
    QVector<Course *> courses = getAllCourses();
    for (auto c : courses)
    {
        QSqlQuery q;
        // 1. Get Attendance
        q.prepare("SELECT COUNT(*) as total, SUM(status) as attended FROM Attendance WHERE student_id = :sid AND course_id = :cid");
        q.bindValue(":sid", studentId);
        q.bindValue(":cid", c->getId());

        // 2. Get Grades (Sum of marks obtained vs Max marks)
        double obtained = 0;
        int max = 0;
        QSqlQuery gradeQ;
        gradeQ.prepare("SELECT SUM(G.marks), SUM(A.max_marks) FROM Grades G "
                       "JOIN Assessments A ON G.assessment_id = A.id "
                       "WHERE G.student_id = :sid AND A.course_id = :cid");
        gradeQ.bindValue(":sid", studentId);
        gradeQ.bindValue(":cid", c->getId());
        if (gradeQ.exec() && gradeQ.next())
        {
            obtained = gradeQ.value(0).toDouble();
            max = gradeQ.value(1).toInt();
        }

        if (q.exec() && q.next())
        {
            list.push_back({c->getName(), q.value("total").toInt(), q.value("attended").toInt(), obtained, max});
        }
        delete c;
    }
    return list;
}

QVector<QString> AcademicManager::getCourseDates(int courseId)
{
    QVector<QString> list;
    QSqlQuery query;
    query.prepare("SELECT DISTINCT date FROM Attendance WHERE course_id = :cid ORDER BY date");
    query.bindValue(":cid", courseId);
    if (query.exec())
    {
        while (query.next())
            list.append(query.value("date").toString());
    }
    return list;
}

bool AcademicManager::isPresent(int courseId, int studentId, QString date)
{
    QSqlQuery query;
    query.prepare("SELECT status FROM Attendance WHERE course_id = :cid AND student_id = :sid AND date = :d");
    query.bindValue(":cid", courseId);
    query.bindValue(":sid", studentId);
    query.bindValue(":d", date);
    if (query.exec() && query.next())
    {
        return query.value("status").toBool();
    }
    return false;
}

bool AcademicManager::addCourse(const Course &c)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Courses (id, code, name, teacher_id, semester, credits) VALUES (:id, :code, :name, :tid, :sem, :credits)");
    query.bindValue(":id", c.getId());
    query.bindValue(":code", c.getCode());
    query.bindValue(":name", c.getName());
    query.bindValue(":tid", c.getTeacherId());
    query.bindValue(":sem", c.getSemester());
    query.bindValue(":credits", c.getCredits());

    if (!query.exec())
    {
        qDebug() << "Add Course Failed:" << query.lastError();
        return false;
    }
    return true;
}

bool AcademicManager::updateCourse(const Course &c)
{
    QSqlQuery query;
    query.prepare("UPDATE Courses SET code = :code, name = :name, teacher_id = :tid, semester = :sem, credits = :credits WHERE id = :id");
    query.bindValue(":code", c.getCode());
    query.bindValue(":name", c.getName());
    query.bindValue(":tid", c.getTeacherId());
    query.bindValue(":sem", c.getSemester());
    query.bindValue(":credits", c.getCredits());
    query.bindValue(":id", c.getId());
    return query.exec();
}

bool AcademicManager::removeCourse(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Courses WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

Course *AcademicManager::getCourse(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Courses WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next())
    {
        return new Course(query.value("id").toInt(),
                          query.value("code").toString(),
                          query.value("name").toString(),
                          query.value("teacher_id").toInt(),
                          query.value("semester").toInt(),
                          query.value("credits").toInt());
    }
    return nullptr;
}

QVector<Course *> AcademicManager::getAllCourses()
{
    QVector<Course *> list;
    QSqlQuery query("SELECT * FROM Courses");
    while (query.next())
    {
        list.append(new Course(query.value("id").toInt(),
                               query.value("code").toString(),
                               query.value("name").toString(),
                               query.value("teacher_id").toInt(),
                               query.value("semester").toInt(),
                               query.value("credits").toInt()));
    }
    return list;
}

QVector<Course *> AcademicManager::getTeacherCourses(int teacherId)
{
    QVector<Course *> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Courses WHERE teacher_id = :tid");
    query.bindValue(":tid", teacherId);
    if (query.exec())
    {
        while (query.next())
        {
            list.append(new Course(query.value("id").toInt(),
                                   query.value("code").toString(),
                                   query.value("name").toString(),
                                   query.value("teacher_id").toInt(),
                                   query.value("semester").toInt(),
                                   query.value("credits").toInt()));
        }
    }
    return list;
}

QVector<Student *> AcademicManager::getStudentsBySemester(int semester)
{
    QVector<Student *> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Students WHERE semester = :sem");
    query.bindValue(":sem", semester);
    if (query.exec())
    {
        while (query.next())
        {
            list.append(new Student(query.value("id").toInt(), query.value("name").toString(),
                                    query.value("email").toString(), query.value("department").toString(),
                                    query.value("batch").toString(),
                                    query.value("semester").toInt()));
        }
    }
    return list;
}

bool AcademicManager::addTask(int studentId, QString desc)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Tasks (student_id, description, completed) VALUES (:sid, :desc, 0)");
    query.bindValue(":sid", studentId);
    query.bindValue(":desc", desc);
    return query.exec();
}

bool AcademicManager::completeTask(int taskId, bool completed)
{
    QSqlQuery query;
    query.prepare("UPDATE Tasks SET completed = :comp WHERE id = :id");
    query.bindValue(":comp", completed ? 1 : 0);
    query.bindValue(":id", taskId);
    return query.exec();
}

QVector<Task> AcademicManager::getTasks(int studentId)
{
    QVector<Task> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Tasks WHERE student_id = :sid");
    query.bindValue(":sid", studentId);
    if (query.exec())
    {
        while (query.next())
        {
            list.push_back({query.value("id").toInt(), studentId, query.value("description").toString(), query.value("completed").toBool()});
        }
    }
    return list;
}

bool AcademicManager::addHabit(Habit *habit)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Habits (student_id, name, type, frequency, target, unit, streak, last_updated, is_completed, current_value) "
                  "VALUES (:sid, :name, :type, :freq, :target, :unit, 0, :last, 0, :val)");

    query.bindValue(":sid", habit->studentId);
    query.bindValue(":name", habit->name);
    query.bindValue(":type", (int)habit->type);
    query.bindValue(":freq", (int)habit->frequency);

    int target = 0;
    QString unit = "";
    if (auto h = dynamic_cast<DurationHabit *>(habit))
        target = h->targetMinutes;
    else if (auto h = dynamic_cast<CountHabit *>(habit))
    {
        target = h->targetCount;
        unit = h->unit;
    }

    query.bindValue(":target", target);
    query.bindValue(":unit", unit);
    query.bindValue(":last", habit->lastUpdated.toString(Qt::ISODate));
    query.bindValue(":val", habit->serializeValue());

    return query.exec();
}

bool AcademicManager::updateHabit(Habit *habit)
{
    QSqlQuery query;
    query.prepare("UPDATE Habits SET streak = :streak, last_updated = :last, is_completed = :comp, current_value = :val WHERE id = :id");
    query.bindValue(":streak", habit->streak);
    query.bindValue(":last", habit->lastUpdated.toString(Qt::ISODate));
    query.bindValue(":comp", habit->isCompleted ? 1 : 0);
    query.bindValue(":val", habit->serializeValue());
    query.bindValue(":id", habit->id);
    return query.exec();
}

bool AcademicManager::deleteHabit(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Habits WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QVector<Habit *> AcademicManager::getHabits(int studentId)
{
    QVector<Habit *> list;
    QSqlQuery query;
    query.prepare("SELECT * FROM Habits WHERE student_id = :sid");
    query.bindValue(":sid", studentId);
    if (query.exec())
    {
        while (query.next())
        {
            int id = query.value("id").toInt();
            QString name = query.value("name").toString();
            HabitType type = (HabitType)query.value("type").toInt();
            Frequency freq = (Frequency)query.value("frequency").toInt();
            int target = query.value("target").toInt();
            QString unit = query.value("unit").toString();

            Habit *h = nullptr;
            if (type == HabitType::DURATION)
                h = new DurationHabit(id, studentId, name, freq, target);
            else if (type == HabitType::COUNT)
                h = new CountHabit(id, studentId, name, freq, target, unit);

            if (h)
            {
                h->streak = query.value("streak").toInt();
                h->lastUpdated = QDate::fromString(query.value("last_updated").toString(), Qt::ISODate);
                h->isCompleted = query.value("is_completed").toBool();
                h->deserializeValue(query.value("current_value").toString());

                // Check for reset logic (new day/week)
                if (h->checkReset())
                {
                    // If reset happened, we should update DB immediately or wait for next action.
                    // Let's update DB to sync the reset state.
                    updateHabit(h);
                }
                list.append(h);
            }
        }
    }
    return list;
}

DailyPrayerStatus AcademicManager::getDailyPrayers(int studentId, QString date)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM DailyPrayers WHERE student_id = :sid AND date = :d");
    query.bindValue(":sid", studentId);
    query.bindValue(":d", date);

    if (query.exec() && query.next())
    {
        return {
            query.value("fajr").toBool(),
            query.value("dhuhr").toBool(),
            query.value("asr").toBool(),
            query.value("maghrib").toBool(),
            query.value("isha").toBool()};
    }
    else
    {
        // Create default entry if not exists
        QSqlQuery insert;
        insert.prepare("INSERT INTO DailyPrayers (student_id, date) VALUES (:sid, :d)");
        insert.bindValue(":sid", studentId);
        insert.bindValue(":d", date);
        insert.exec();
        return {false, false, false, false, false};
    }
}

bool AcademicManager::updateDailyPrayer(int studentId, QString date, QString prayerName, bool status)
{
    // prayerName should be "fajr", "dhuhr", etc.
    QSqlQuery query;
    QString sql = QString("UPDATE DailyPrayers SET %1 = :val WHERE student_id = :sid AND date = :d").arg(prayerName);
    query.prepare(sql);
    query.bindValue(":val", status ? 1 : 0);
    query.bindValue(":sid", studentId);
    query.bindValue(":d", date);
    return query.exec();
}

bool AcademicManager::addNotice(QString content, QString author)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Notices (content, date, author) VALUES (:c, date('now'), :a)");
    query.bindValue(":c", content);
    query.bindValue(":a", author);
    return query.exec();
}

QVector<Notice> AcademicManager::getNotices()
{
    QVector<Notice> list;
    QSqlQuery query("SELECT * FROM Notices ORDER BY id DESC");
    while (query.next())
    {
        list.push_back({query.value("id").toInt(), query.value("content").toString(), query.value("date").toString(), query.value("author").toString()});
    }
    return list;
}

bool AcademicManager::addTeacher(const Teacher &t)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Teachers (id, name, email, department, designation) VALUES (:id, :name, :email, :dept, :desig)");
    query.bindValue(":id", t.getId());
    query.bindValue(":name", t.getName());
    query.bindValue(":email", t.getEmail());
    query.bindValue(":dept", t.getDepartment());
    query.bindValue(":desig", t.getDesignation());

    if (!query.exec())
    {
        qDebug() << "Add Teacher Failed:" << query.lastError();
        return false;
    }
    return true;
}

bool AcademicManager::updateTeacher(const Teacher &t)
{
    QSqlQuery query;
    query.prepare("UPDATE Teachers SET name = :name, email = :email, department = :dept, designation = :desig WHERE id = :id");
    query.bindValue(":name", t.getName());
    query.bindValue(":email", t.getEmail());
    query.bindValue(":dept", t.getDepartment());
    query.bindValue(":desig", t.getDesignation());
    query.bindValue(":id", t.getId());
    return query.exec();
}

bool AcademicManager::removeTeacher(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM Teachers WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

Teacher *AcademicManager::getTeacher(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Teachers WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next())
    {
        return new Teacher(query.value("id").toInt(),
                           query.value("name").toString(),
                           query.value("email").toString(),
                           query.value("department").toString(),
                           query.value("designation").toString());
    }
    return nullptr;
}

QVector<Teacher *> AcademicManager::getAllTeachers()
{
    QVector<Teacher *> list;
    QSqlQuery query("SELECT * FROM Teachers");
    while (query.next())
    {
        list.append(new Teacher(query.value("id").toInt(),
                                query.value("name").toString(),
                                query.value("email").toString(),
                                query.value("department").toString(),
                                query.value("designation").toString()));
    }
    return list;
}