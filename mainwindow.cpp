#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>
#include <QCheckBox>
#include <QDate>
#include <QTime>
#include <algorithm>

MainWindow::MainWindow(QString role, int uid, QString name, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), userRole(role), userId(uid), userName(name)
{
    activeTimerHabit = nullptr;
    ui->setupUi(this);

    // Initialize Timer
    m_focusTimer = new Timer(this);
    connect(m_focusTimer, &Timer::timeUpdated, [this](QString time)
            { ui->label_timerDisplay->setText(time); });
    connect(m_focusTimer, &Timer::finished, [this]()
            { QMessageBox::information(this, "Timer", "Focus session complete!"); });

    // Initialize Workout Timer
    m_workoutTimer = new Timer(this);
    connect(m_workoutTimer, &Timer::timeUpdated, [this](QString time)
            { ui->label_workoutTimerDisplay->setText(time); });
    connect(m_workoutTimer, &Timer::finished, [this]()
            { 
                QMessageBox::information(this, "Habit", "Session complete!");
                if(activeTimerHabit) {
                    activeTimerHabit->currentMinutes = activeTimerHabit->targetMinutes;
                    activeTimerHabit->markComplete();
                    myManager.updateHabit(activeTimerHabit);
                    refreshHabits();
                } });

    ui->label_welcome->setText("Welcome, " + role + " " + name);

    // Initialize Admin Model
    adminModel = new QSqlTableModel(this);
    adminModel->setEditStrategy(QSqlTableModel::OnFieldChange); // Auto-save on edit

    // Initialize Proxy Model for Search/Filter
    adminProxyModel = new QSortFilterProxyModel(this);
    adminProxyModel->setSourceModel(adminModel);
    adminProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    adminProxyModel->setFilterKeyColumn(-1); // Filter all columns
    ui->adminTableView->setModel(adminProxyModel);

    // Populate Table Selector
    QStringList tables = QSqlDatabase::database().tables();
    ui->tableComboBox->addItems(tables);
    // Trigger initial load
    if (!tables.isEmpty())
        on_tableComboBox_currentTextChanged(tables.first());

    // Role-based UI adjustments
    if (role == "Student")
    {
        ui->addNoticeButton->setVisible(false);  // Only teachers post notices
        ui->tabWidget->setTabVisible(5, false);  // Hide Teacher Routine
        ui->tabWidget->setTabVisible(6, false);  // Hide Teacher Assessment
        ui->tabWidget->setTabVisible(7, false);  // Hide Teacher Grades
        ui->tabWidget->setTabVisible(8, false);  // Hide Teacher Attendance
        ui->tabWidget->setTabVisible(10, false); // Hide Admin Panel
    }
    else if (role == "Teacher")
    {
        // Teacher
        ui->tabWidget->setTabVisible(1, false);  // Teachers don't need study planner
        ui->tabWidget->setTabVisible(2, false);  // Teachers don't need habit tracker
        ui->tabWidget->setTabVisible(3, false);  // Hide Student Routine
        ui->tabWidget->setTabVisible(4, false);  // Hide Student Academics view
        ui->tabWidget->setTabVisible(10, false); // Hide Admin Panel
        // Teacher Tabs (5, 6, 7, 8) and Q&A (9) remain visible
        refreshTeacherRoutine();
        refreshTeacherTools();
    }
    else if (role == "Admin")
    {
        // Admin
        ui->tabWidget->setTabVisible(1, false); // Hide Planner
        ui->tabWidget->setTabVisible(2, false); // Hide Habits
        ui->tabWidget->setTabVisible(3, false); // Hide Routine
        ui->tabWidget->setTabVisible(4, false); // Hide Academics
        ui->tabWidget->setTabVisible(5, false); // Hide Teacher Routine
        ui->tabWidget->setTabVisible(6, false); // Hide Teacher Assessment
        ui->tabWidget->setTabVisible(7, false); // Hide Teacher Grades
        ui->tabWidget->setTabVisible(8, false); // Hide Teacher Attendance
        ui->tabWidget->setTabVisible(9, false); // Hide Q&A
        // Admin Panel (10) visible
        ui->addNoticeButton->setVisible(false);
        ui->label_notices->setVisible(false);
        ui->noticeListWidget->setVisible(false);
    }
    // Tab 6 (Queries) is visible to both

    // Load initial data (Students by default)
    refreshDashboard();
    refreshQueries();
    if (role == "Student")
    {
        refreshPlanner();
        refreshHabits();

        // Auto-select the current day's routine (Qt days: 1=Mon ... 7=Sun)
        int currentDayIndex = QDate::currentDate().dayOfWeek() % 7;
        ui->comboRoutineDay->setCurrentIndex(currentDayIndex);

        refreshAcademics();
    }
}

MainWindow::~MainWindow()
{
    qDeleteAll(currentHabitList);
    delete ui;
}

// === DASHBOARD ===

void MainWindow::refreshDashboard()
{
    ui->noticeListWidget->clear();
    QVector<Notice> notices = myManager.getNotices();
    for (const auto &n : notices)
    {
        ui->noticeListWidget->addItem("[" + n.date + "] " + n.author + ": " + n.content);
    }

    // Update Next Class - Only for Students
    if (userRole == "Student")
    {
        QString nextClass = myManager.getNextClass();
        if (nextClass.isEmpty())
        {
            ui->label_nextClass->setVisible(false);
        }
        else
        {
            ui->label_nextClass->setVisible(true);
            ui->label_nextClass->setText("Next Class: " + nextClass);
        }
    }
    else
    {
        ui->label_nextClass->setVisible(false);
    }

    QString stats = myManager.getDashboardStats(userId, userRole);
    ui->label_welcome->setToolTip(stats);

    // Update Profile Info
    if (userRole == "Student")
    {
        Student *s = myManager.getStudent(userId);
        if (s)
        {
            ui->val_p_name->setText(s->getName());
            ui->lbl_p_id->setVisible(true);
            ui->val_p_id->setVisible(true);
            ui->val_p_id->setText(QString::number(s->getId()));
            ui->val_p_dept->setText(s->getDepartment());
            ui->val_p_sem->setText(QString::number(s->getSemester()));
            ui->val_p_email->setText(s->getEmail());
            delete s;
        }
    }
    else if (userRole == "Teacher")
    {
        Teacher *t = myManager.getTeacher(userId);
        if (t)
        {
            ui->val_p_name->setText(t->getName());
            ui->lbl_p_id->setVisible(false);
            ui->val_p_id->setVisible(false);
            ui->val_p_dept->setText(t->getDepartment());
            ui->lbl_p_sem->setText("Designation:");
            ui->val_p_sem->setText(t->getDesignation());
            ui->val_p_email->setText(t->getEmail());
            delete t;
        }
    }
    else if (userRole == "Admin")
    {
        ui->groupBox_profile->setTitle("Administrator");
        ui->val_p_name->setText("System Admin");
        ui->lbl_p_id->setVisible(false);
        ui->val_p_id->setVisible(false);
        ui->val_p_dept->setText("IT / Admin");
        ui->lbl_p_sem->setText("Role:");
        ui->val_p_sem->setText("Super User");
        ui->val_p_email->setText("admin@school.edu");
    }
}

void MainWindow::on_addNoticeButton_clicked()
{
    bool ok;
    QString content = QInputDialog::getText(this, "Post Notice", "Content:", QLineEdit::Normal, "", &ok);
    if (ok && !content.isEmpty())
    {
        myManager.addNotice(content, userName);
        refreshDashboard();
    }
}

void MainWindow::on_logoutButton_clicked()
{
    QApplication::exit(99);
}

// === PLANNER ===

void MainWindow::refreshPlanner()
{
    ui->taskListWidget->clear();
    QVector<Task> tasks = myManager.getTasks(userId);
    for (const auto &t : tasks)
    {
        QString status = t.isCompleted ? "[DONE] " : "[TODO] ";
        QListWidgetItem *item = new QListWidgetItem(status + t.description);
        item->setData(Qt::UserRole, t.id);
        if (t.isCompleted)
            item->setForeground(Qt::gray);
        ui->taskListWidget->addItem(item);
    }
}

void MainWindow::on_addTaskButton_clicked()
{
    QString desc = ui->taskLineEdit->text();
    if (!desc.isEmpty())
    {
        myManager.addTask(userId, desc);
        ui->taskLineEdit->clear();
        refreshPlanner();
    }
}

void MainWindow::on_completeTaskButton_clicked()
{
    QListWidgetItem *item = ui->taskListWidget->currentItem();
    if (item)
    {
        int id = item->data(Qt::UserRole).toInt();
        myManager.completeTask(id, true);
        refreshPlanner();
    }
}

void MainWindow::on_btnTimerStart_clicked()
{
    m_focusTimer->start(ui->spinTimerMinutes->value());
}

void MainWindow::on_btnTimerPause_clicked()
{
    m_focusTimer->pause();
}

void MainWindow::on_btnTimerStop_clicked()
{
    m_focusTimer->stop();
}

// === HABITS ===

void MainWindow::refreshHabits()
{
    // 1. Refresh Prayers
    DailyPrayerStatus prayers = myManager.getDailyPrayers(userId, QDate::currentDate().toString(Qt::ISODate));
    ui->chkFajr->setChecked(prayers.fajr);
    ui->chkDhuhr->setChecked(prayers.dhuhr);
    ui->chkAsr->setChecked(prayers.asr);
    ui->chkMaghrib->setChecked(prayers.maghrib);
    ui->chkIsha->setChecked(prayers.isha);

    // 2. Refresh Habits List
    qDeleteAll(currentHabitList);
    currentHabitList = myManager.getHabits(userId);
    ui->habitListWidget->clear();

    for (auto h : currentHabitList)
    {
        QString label = QString("[%1] %2 | %3 | Streak: %4 %5")
                            .arg(h->getTypeString())
                            .arg(h->name)
                            .arg(h->getProgressString())
                            .arg(h->streak)
                            .arg(h->isCompleted ? "[DONE]" : "");

        QListWidgetItem *item = new QListWidgetItem(label);
        item->setData(Qt::UserRole, h->id);
        if (h->isCompleted)
            item->setForeground(Qt::green);
        ui->habitListWidget->addItem(item);
    }
}

void MainWindow::on_btnAddHabit_clicked()
{
    QStringList types = {"Duration (Timer)", "Count (Counter)"};
    bool ok;
    QString typeStr = QInputDialog::getItem(this, "Create Habit", "Type:", types, 0, false, &ok);
    if (!ok)
        return;

    QString name = QInputDialog::getText(this, "Create Habit", "Name:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;

    QStringList freqs = {"Daily", "Weekly"};
    QString freqStr = QInputDialog::getItem(this, "Create Habit", "Frequency:", freqs, 0, false, &ok);
    Frequency f = (freqStr == "Daily") ? Frequency::DAILY : Frequency::WEEKLY;

    if (typeStr.startsWith("Duration"))
    {
        int target = QInputDialog::getInt(this, "Create Habit", "Target Minutes:", 30, 1, 1440, 1, &ok);
        if (!ok)
            return;
        DurationHabit *h = new DurationHabit(0, userId, name, f, target);
        myManager.addHabit(h);
        delete h;
    }
    else
    {
        int target = QInputDialog::getInt(this, "Create Habit", "Target Count:", 5, 1, 1000, 1, &ok);
        if (!ok)
            return;
        QString unit = QInputDialog::getText(this, "Create Habit", "Unit (e.g. glasses):", QLineEdit::Normal, "", &ok);
        CountHabit *h = new CountHabit(0, userId, name, f, target, unit);
        myManager.addHabit(h);
        delete h;
    }
    refreshHabits();
}

void MainWindow::on_btnPerformHabit_clicked()
{
    int row = ui->habitListWidget->currentRow();
    if (row < 0 || row >= currentHabitList.size())
        return;

    Habit *h = currentHabitList[row];
    bool changed = false;

    if (auto dh = dynamic_cast<DurationHabit *>(h))
    {
        // Setup Timer
        activeTimerHabit = dh;
        ui->groupBox_workoutTimer->setTitle("Timer: " + dh->name);
        ui->spinWorkoutMinutes->setValue(dh->targetMinutes - dh->currentMinutes);
        if (ui->spinWorkoutMinutes->value() <= 0)
            ui->spinWorkoutMinutes->setValue(dh->targetMinutes);

        QMessageBox::information(this, "Timer Ready", "Timer set for '" + dh->name + "'.\nClick Start in the Habit Timer box.");
    }
    else if (auto ch = dynamic_cast<CountHabit *>(h))
    {
        bool ok;
        int count = QInputDialog::getInt(this, "Perform Habit", "Add Count:", 1, 1, 100, 1, &ok);
        if (ok)
        {
            ch->currentCount += count;
            if (ch->currentCount >= ch->targetCount)
                ch->markComplete();
            changed = true;
        }
    }

    if (changed)
    {
        myManager.updateHabit(h);
        refreshHabits();
    }
}

void MainWindow::on_btnDeleteHabit_clicked()
{
    int row = ui->habitListWidget->currentRow();
    if (row < 0 || row >= currentHabitList.size())
        return;

    int id = currentHabitList[row]->id;
    myManager.deleteHabit(id);
    refreshHabits();
}

void MainWindow::on_btnWorkoutStart_clicked()
{
    m_workoutTimer->start(ui->spinWorkoutMinutes->value());
}

void MainWindow::on_btnWorkoutPause_clicked()
{
    m_workoutTimer->pause();
}

void MainWindow::on_btnWorkoutStop_clicked()
{
    m_workoutTimer->stop();
}

// === PRAYER SLOTS ===
void MainWindow::on_chkFajr_toggled(bool checked) { myManager.updateDailyPrayer(userId, QDate::currentDate().toString(Qt::ISODate), "fajr", checked); }
void MainWindow::on_chkDhuhr_toggled(bool checked) { myManager.updateDailyPrayer(userId, QDate::currentDate().toString(Qt::ISODate), "dhuhr", checked); }
void MainWindow::on_chkAsr_toggled(bool checked) { myManager.updateDailyPrayer(userId, QDate::currentDate().toString(Qt::ISODate), "asr", checked); }
void MainWindow::on_chkMaghrib_toggled(bool checked) { myManager.updateDailyPrayer(userId, QDate::currentDate().toString(Qt::ISODate), "maghrib", checked); }
void MainWindow::on_chkIsha_toggled(bool checked) { myManager.updateDailyPrayer(userId, QDate::currentDate().toString(Qt::ISODate), "isha", checked); }

// === ROUTINE ===

void MainWindow::refreshRoutine()
{
    ui->tableRoutine->setRowCount(0);
    QString day = ui->comboRoutineDay->currentText();

    int semester = -1;
    if (userRole == "Student")
    {
        Student *s = myManager.getStudent(userId);
        if (s)
        {
            semester = s->getSemester();
            delete s;
        }
    }
    QVector<RoutineItem> items = myManager.getRoutineForDay(day, semester);

    QTime currentTime = QTime::currentTime();
    QString currentDay = QDate::currentDate().toString("dddd");
    bool isToday = (day == currentDay);

    for (const auto &i : items)
    {
        QString status = "Upcoming";

        if (isToday)
        {
            QTime classTime = QTime::fromString(i.startTime, "HH:mm");
            int diff = currentTime.secsTo(classTime) / 60; // Difference in minutes

            if (diff < -90)
                status = "Completed";
            else if (diff <= 0)
                status = "In Progress";
            else if (diff <= 15)
                status = "Starting Soon";
        }

        int row = ui->tableRoutine->rowCount();
        ui->tableRoutine->insertRow(row);
        ui->tableRoutine->setItem(row, 0, new QTableWidgetItem(i.startTime + " - " + i.endTime));
        ui->tableRoutine->setItem(row, 1, new QTableWidgetItem(i.courseCode + ": " + i.courseName));
        ui->tableRoutine->setItem(row, 2, new QTableWidgetItem(i.room));
        ui->tableRoutine->setItem(row, 3, new QTableWidgetItem(i.instructor));
        ui->tableRoutine->setItem(row, 4, new QTableWidgetItem(status));
    }
}

void MainWindow::on_comboRoutineDay_currentIndexChanged(int index)
{
    refreshRoutine();
}

void MainWindow::refreshTeacherRoutine()
{
    ui->tableTeacherRoutine->setRowCount(0);
    QString day = ui->comboRoutineDayInput->currentText();
    // Teachers see all routines for the day to avoid conflicts, or we could filter.
    // Let's show all for now so they know room availability.
    QVector<RoutineItem> items = myManager.getRoutineForDay(day);

    for (const auto &i : items)
    {
        int row = ui->tableTeacherRoutine->rowCount();
        ui->tableTeacherRoutine->insertRow(row);
        ui->tableTeacherRoutine->setItem(row, 0, new QTableWidgetItem(i.startTime + " - " + i.endTime));
        ui->tableTeacherRoutine->setItem(row, 1, new QTableWidgetItem(i.courseCode + ": " + i.courseName));
        ui->tableTeacherRoutine->setItem(row, 2, new QTableWidgetItem(i.room));
        ui->tableTeacherRoutine->setItem(row, 3, new QTableWidgetItem(QString::number(i.semester)));
    }
}

void MainWindow::on_comboRoutineDayInput_currentIndexChanged(int index)
{
    refreshTeacherRoutine();
}

void MainWindow::on_btnAddRoutine_Teacher_clicked()
{
    QString day = ui->comboRoutineDayInput->currentText();
    QString startTime = ui->editRoutineTime->text();
    QString endTime = ui->editRoutineEndTime->text();
    QString room = ui->editRoutineRoom->text();

    // Get Course Details from ComboBox
    int courseId = ui->comboRoutineCourse->currentData().toInt();
    Course *c = myManager.getCourse(courseId);

    if (!c)
    {
        QMessageBox::warning(this, "Error", "Please select a valid course.");
        return;
    }

    if (startTime.isEmpty() || room.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Time and Room are required.");
        delete c;
        return;
    }

    // Instructor is the current logged in teacher
    Teacher *t = myManager.getTeacher(userId);
    QString instructorName = t ? t->getName() : "Unknown";
    if (t)
        delete t;

    myManager.addRoutineItem(day, startTime, endTime, c->getCode(), c->getName(), room, instructorName, c->getSemester());
    delete c;

    refreshTeacherRoutine();
    QMessageBox::information(this, "Success", "Routine item added.");
    // Clear inputs
    ui->editRoutineTime->clear();
    ui->editRoutineEndTime->clear();
    ui->editRoutineRoom->clear();
}

// === ACADEMICS (Student) ===

void MainWindow::refreshAcademics()
{
    // Assessments
    ui->listAssessments->clear();
    QVector<Assessment> assessments = myManager.getAssessments(); // Get all for now
    for (const auto &a : assessments)
    {
        ui->listAssessments->addItem(a.date + " - " + a.courseName + ": " + a.title + " (" + a.type + ")");
    }

    // Attendance & Grades
    ui->tableAcademics->setRowCount(0);
    QVector<AttendanceRecord> att = myManager.getStudentAttendance(userId);
    for (int i = 0; i < att.size(); ++i)
    {
        ui->tableAcademics->insertRow(i);
        ui->tableAcademics->setItem(i, 0, new QTableWidgetItem(att[i].courseName));

        double pct = (att[i].totalClasses > 0) ? (double)att[i].attendedClasses / att[i].totalClasses * 100.0 : 0.0;
        QTableWidgetItem *pctItem = new QTableWidgetItem(QString::number(pct, 'f', 1) + "%");
        if (pct < 85.0)
            pctItem->setForeground(Qt::red);
        ui->tableAcademics->setItem(i, 1, pctItem);

        QString scoreStr = QString::number(att[i].totalMarksObtained) + " / " + QString::number(att[i].totalMaxMarks);
        ui->tableAcademics->setItem(i, 2, new QTableWidgetItem(scoreStr));

        QString status = (pct < 85.0) ? "Low Attendance" : "Good";
        ui->tableAcademics->setItem(i, 3, new QTableWidgetItem(status));
    }
}

// === TEACHER TOOLS ===

void MainWindow::refreshTeacherTools()
{
    // Populate Courses
    ui->comboTeacherCourse->clear();
    ui->comboRoutineCourse->clear();
    ui->comboAttendanceCourse->clear();
    QVector<Course *> courses = myManager.getTeacherCourses(userId); // Only show courses taught by this teacher
    for (auto c : courses)
    {
        ui->comboTeacherCourse->addItem(c->getName(), c->getId());
        ui->comboRoutineCourse->addItem(c->getCode() + " - " + c->getName(), c->getId());
        ui->comboAttendanceCourse->addItem(c->getCode() + " - " + c->getName(), c->getId());
    }
    qDeleteAll(courses);

    // Populate Assessments for Grading
    ui->comboTeacherAssessment->clear();
    QVector<Assessment> assessments = myManager.getAssessments();
    for (const auto &a : assessments)
    {
        ui->comboTeacherAssessment->addItem(a.courseName + " - " + a.title, a.id);
    }

    // Trigger table refresh
    refreshTeacherGrades();
    refreshTeacherAttendance();
}

void MainWindow::on_btnCreateAssessment_clicked()
{
    int courseId = ui->comboTeacherCourse->currentData().toInt();
    QString title = ui->editAssessmentTitle->text();
    if (title.isEmpty())
        return;

    myManager.addAssessment(courseId, title, ui->comboAssessmentType->currentText(),
                            QDate::currentDate().toString("yyyy-MM-dd"), ui->spinMaxMarks->value());
    ui->editAssessmentTitle->clear();
    refreshTeacherTools();
    QMessageBox::information(this, "Success", "Assessment Created");
}

void MainWindow::on_comboTeacherAssessment_currentIndexChanged(int index)
{
    refreshTeacherGrades();
}

void MainWindow::refreshTeacherGrades()
{
    ui->tableGrading->setRowCount(0);

    // Get selected assessment to find course and semester
    int assessmentId = ui->comboTeacherAssessment->currentData().toInt();

    QVector<Assessment> allAssessments = myManager.getAssessments();
    int courseId = -1;
    for (const auto &a : allAssessments)
    {
        if (a.id == assessmentId)
        {
            courseId = a.courseId;
            break;
        }
    }

    if (courseId == -1)
        return;

    Course *c = myManager.getCourse(courseId);

    QVector<Student *> students;
    if (c)
    {
        // Only show students in the semester of the selected course
        students = myManager.getStudentsBySemester(c->getSemester());
        delete c;
    }

    for (int i = 0; i < students.size(); ++i)
    {
        ui->tableGrading->insertRow(i);
        ui->tableGrading->setItem(i, 0, new QTableWidgetItem(QString::number(students[i]->getId())));
        ui->tableGrading->setItem(i, 1, new QTableWidgetItem(students[i]->getName()));

        double currentGrade = myManager.getGrade(students[i]->getId(), assessmentId);
        QString gradeStr = (currentGrade >= 0) ? QString::number(currentGrade) : "0";
        ui->tableGrading->setItem(i, 2, new QTableWidgetItem(gradeStr));
    }
    qDeleteAll(students);
}

void MainWindow::on_btnSaveGrades_clicked()
{
    int assessmentId = ui->comboTeacherAssessment->currentData().toInt();
    int rows = ui->tableGrading->rowCount();

    double max = -1, min = 1000, sum = 0;
    int count = 0;

    for (int i = 0; i < rows; ++i)
    {
        int sid = ui->tableGrading->item(i, 0)->text().toInt();
        double marks = ui->tableGrading->item(i, 2)->text().toDouble();
        myManager.addGrade(sid, assessmentId, marks);

        if (marks > max)
            max = marks;
        if (marks < min)
            min = marks;
        sum += marks;
        count++;
    }

    if (count > 0)
    {
        QMessageBox::information(this, "Grading Complete",
                                 QString("Highest: %1\nLowest: %2\nAverage: %3").arg(max).arg(min).arg(sum / count));
    }
}

// === ATTENDANCE ===

void MainWindow::refreshTeacherAttendance()
{
    ui->tableAttendance->clear();
    int courseId = ui->comboAttendanceCourse->currentData().toInt();
    Course *c = myManager.getCourse(courseId);
    if (!c)
        return;

    QVector<Student *> students = myManager.getStudentsBySemester(c->getSemester());
    QVector<QString> dates = myManager.getCourseDates(courseId);

    // Setup Columns: ID, Name, %, Total, [Dates...]
    QStringList headers;
    headers << "ID" << "Name" << "%" << "Total";
    for (const QString &d : dates)
        headers << d;

    ui->tableAttendance->setColumnCount(headers.size());
    ui->tableAttendance->setHorizontalHeaderLabels(headers);
    ui->tableAttendance->setRowCount(students.size());

    for (int i = 0; i < students.size(); ++i)
    {
        int sid = students[i]->getId();
        ui->tableAttendance->setItem(i, 0, new QTableWidgetItem(QString::number(sid)));
        ui->tableAttendance->setItem(i, 1, new QTableWidgetItem(students[i]->getName()));

        int presentCount = 0;
        for (int j = 0; j < dates.size(); ++j)
        {
            bool present = myManager.isPresent(courseId, sid, dates[j]);
            if (present)
                presentCount++;

            QTableWidgetItem *checkItem = new QTableWidgetItem();
            checkItem->setCheckState(present ? Qt::Checked : Qt::Unchecked);
            ui->tableAttendance->setItem(i, 4 + j, checkItem);
        }

        double pct = dates.isEmpty() ? 0.0 : (double)presentCount / dates.size() * 100.0;
        ui->tableAttendance->setItem(i, 2, new QTableWidgetItem(QString::number(pct, 'f', 1) + "%"));
        ui->tableAttendance->setItem(i, 3, new QTableWidgetItem(QString::number(presentCount) + "/" + QString::number(dates.size())));
    }

    qDeleteAll(students);
    delete c;
}

void MainWindow::on_comboAttendanceCourse_currentIndexChanged(int index)
{
    refreshTeacherAttendance();
}

void MainWindow::on_btnAddClassDate_clicked()
{
    bool ok;
    QString date = QInputDialog::getText(this, "Add Class", "Date (yyyy-MM-dd):", QLineEdit::Normal, QDate::currentDate().toString("yyyy-MM-dd"), &ok);
    if (ok && !date.isEmpty())
    {
        int col = ui->tableAttendance->columnCount();
        ui->tableAttendance->insertColumn(col);
        ui->tableAttendance->setHorizontalHeaderItem(col, new QTableWidgetItem(date));

        // Add checkboxes for all rows
        for (int i = 0; i < ui->tableAttendance->rowCount(); ++i)
        {
            QTableWidgetItem *checkItem = new QTableWidgetItem();
            checkItem->setCheckState(Qt::Unchecked);
            ui->tableAttendance->setItem(i, col, checkItem);
        }
    }
}

void MainWindow::on_btnSaveAttendance_clicked()
{
    int courseId = ui->comboAttendanceCourse->currentData().toInt();
    if (courseId <= 0)
        return;

    int rows = ui->tableAttendance->rowCount();
    int cols = ui->tableAttendance->columnCount();

    // Columns 0-3 are info. Dates start at 4.
    for (int j = 4; j < cols; ++j)
    {
        QString date = ui->tableAttendance->horizontalHeaderItem(j)->text();
        for (int i = 0; i < rows; ++i)
        {
            int sid = ui->tableAttendance->item(i, 0)->text().toInt();
            bool present = (ui->tableAttendance->item(i, j)->checkState() == Qt::Checked);
            myManager.markAttendance(courseId, sid, date, present);
        }
    }
    QMessageBox::information(this, "Success", "Attendance Saved");
    refreshTeacherAttendance();
}

// === QUERIES ===

void MainWindow::refreshQueries()
{
    ui->listQueries->clear();
    QVector<Query> queries = myManager.getQueries(userId, userRole);

    for (const auto &q : queries)
    {
        QString label;
        if (userRole == "Student")
        {
            label = "Q: " + q.question + "\n   A: " + (q.answer.isEmpty() ? "(Waiting...)" : q.answer);
        }
        else
        {
            label = "[" + q.studentName + "] Q: " + q.question + "\n   A: " + (q.answer.isEmpty() ? "(Select to Reply)" : q.answer);
        }

        QListWidgetItem *item = new QListWidgetItem(label);
        item->setData(Qt::UserRole, q.id);
        if (q.answer.isEmpty())
            item->setForeground(Qt::red);
        ui->listQueries->addItem(item);
    }
}

void MainWindow::on_btnQueryAction_clicked()
{
    QString text = ui->editQueryInput->text();
    if (text.isEmpty())
        return;

    if (userRole == "Student")
    {
        // Student asks question
        myManager.addQuery(userId, text);
        ui->editQueryInput->clear();
        refreshQueries();
    }
    else
    {
        // Teacher replies
        QListWidgetItem *item = ui->listQueries->currentItem();
        if (!item)
        {
            QMessageBox::warning(this, "Selection", "Select a query to reply to.");
            return;
        }
        int qid = item->data(Qt::UserRole).toInt();
        myManager.answerQuery(qid, text);
        ui->editQueryInput->clear();
        refreshQueries();
    }
}

// === ADMIN PANEL ===

void MainWindow::on_tableComboBox_currentTextChanged(const QString &tableName)
{
    adminModel->setTable(tableName);
    adminModel->select(); // Refresh data
    ui->adminTableView->resizeColumnsToContents();
}

void MainWindow::on_btnAddRow_clicked()
{
    // Insert a new empty row at the end
    int row = adminModel->rowCount();
    adminModel->insertRow(row);
}

void MainWindow::on_btnDeleteRow_clicked()
{
    QModelIndexList selected = ui->adminTableView->selectionModel()->selectedRows();
    if (selected.isEmpty())
    {
        QMessageBox::warning(this, "Selection", "Please select a row to delete.");
        return;
    }

    // Collect source rows
    QList<int> sourceRows;
    for (const auto &index : selected)
    {
        QModelIndex sourceIndex = adminProxyModel->mapToSource(index);
        if (sourceIndex.isValid())
            sourceRows.append(sourceIndex.row());
    }

    std::sort(sourceRows.begin(), sourceRows.end());
    for (int i = sourceRows.size() - 1; i >= 0; --i)
    {
        adminModel->removeRow(sourceRows[i]);
    }
    adminModel->select(); // Refresh
}

void MainWindow::on_searchLineEdit_textChanged(const QString &arg1)
{
    adminProxyModel->setFilterFixedString(arg1);
}