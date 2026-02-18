#include "mainwindow.hpp"
#include "utils.hpp"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>
#include <QCheckBox>
#include <QDate>
#include <QTime>
#include <algorithm>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

/**
 * @brief Constructs the MainWindow.
 * @param role The role of the logged-in user (Student, Teacher, Admin).
 * @param uid The unique ID of the user.
 * @param name The display name of the user.
 * @param parent Parent widget.
 */
MainWindow::MainWindow(QString role, int uid, QString name, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), userRole(role), userId(uid), userName(name)
{
    activeTimerHabit = nullptr;
    ui->setupUi(this);
    resize(1280, 720);

    // --- UI Setup: Replace Labels with Custom CircularProgress Widgets ---
    m_focusProgress = new CircularProgress(this);
    if (ui->label_timerDisplay->parentWidget() && ui->label_timerDisplay->parentWidget()->layout())
    {
        auto layout = ui->label_timerDisplay->parentWidget()->layout();
        layout->replaceWidget(ui->label_timerDisplay, m_focusProgress);
        layout->setAlignment(m_focusProgress, Qt::AlignCenter); // Center the widget
        ui->label_timerDisplay->hide();
    }
    m_focusProgress->setTimeText("00:00");

    // --- Initialize Focus Timer ---
    m_focusTimer = new Timer(this);
    connect(m_focusTimer, &Timer::timeUpdated, [this](QString time, float progress)
            {
                m_focusProgress->setTimeText(time);
                m_focusProgress->setProgress(1.0f - progress); // Invert so it fills up or empties based on preference
            });
    connect(m_focusTimer, &Timer::finished, [this]()
            { QMessageBox::information(this, "Timer", "Focus session complete!"); });

    // --- Initialize Workout Timer ---
    m_workoutProgress = new CircularProgress(this);
    if (ui->label_workoutTimerDisplay->parentWidget() && ui->label_workoutTimerDisplay->parentWidget()->layout())
    {
        auto layout = ui->label_workoutTimerDisplay->parentWidget()->layout();
        layout->replaceWidget(ui->label_workoutTimerDisplay, m_workoutProgress);
        layout->setAlignment(m_workoutProgress, Qt::AlignCenter); // Center the widget
        ui->label_workoutTimerDisplay->hide();
    }
    m_workoutProgress->setTimeText("00:00");

    m_workoutTimer = new Timer(this);
    connect(m_workoutTimer, &Timer::timeUpdated, [this](QString time, float progress)
            {
                m_workoutProgress->setTimeText(time);
                m_workoutProgress->setProgress(1.0f - progress); });
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

    // --- Styling: Add Shadow to Profile Box ---
    QGraphicsDropShadowEffect *profileShadow = new QGraphicsDropShadowEffect(ui->groupBox_profile);
    profileShadow->setBlurRadius(20);
    profileShadow->setXOffset(0);
    profileShadow->setYOffset(5);
    profileShadow->setColor(QColor(0, 0, 0, 40));
    ui->groupBox_profile->setGraphicsEffect(profileShadow);

    // --- Profile Actions ---
    QPushButton *btnChangePass = new QPushButton("Change Password", this);
    btnChangePass->setFixedWidth(200);
    if (ui->groupBox_profile->layout())
    {
        ui->groupBox_profile->layout()->addWidget(btnChangePass);
    }
    connect(btnChangePass, &QPushButton::clicked, this, &MainWindow::onChangePasswordClicked);

    // --- Admin Panel Setup ---
    adminModel = new QStandardItemModel(this);

    // Proxy Model for Search/Filter functionality in Admin Table
    adminProxyModel = new QSortFilterProxyModel(this);
    adminProxyModel->setSourceModel(adminModel);
    adminProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    adminProxyModel->setFilterKeyColumn(-1); // Filter all columns
    ui->adminTableView->setModel(adminProxyModel);
    csvDelegate = new CsvDelegate(this);
    ui->adminTableView->setItemDelegate(csvDelegate);
    ui->adminTableView->setAlternatingRowColors(true);

    // Populate Admin Table Selector
    QStringList tables = {
        "admins", "students", "teachers", "courses", "routine", "grades", "notices"};
    ui->tableComboBox->addItems(tables);

    if (!tables.isEmpty())
        on_tableComboBox_currentTextChanged(tables.first());

    // --- Role-Based UI Visibility Logic ---
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

    // --- Initial Data Load ---
    try
    {
        refreshDashboard();
        refreshQueries();
        if (role == "Student")
        {
            refreshPlanner();
            refreshHabits();

            // Auto-select the current day's routine (Qt days: 1=Mon ... 7=Sun)
            int currentDayIndex = QDate::currentDate().dayOfWeek() % 7;
            ui->comboRoutineDay->setCurrentIndex(currentDayIndex);

            ui->tableRoutine->setAlternatingRowColors(true);
            ui->tableTeacherRoutine->setAlternatingRowColors(true);
            ui->tableAcademics->setAlternatingRowColors(true);
            ui->tableGrading->setAlternatingRowColors(true);
            ui->tableAttendance->setAlternatingRowColors(true);

            refreshAcademics();
        }
    }
    catch (const Acadence::Exception &e)
    {
        QMessageBox::critical(this, "Data Error", QString("Failed to load initial data:\n%1").arg(e.what()));
    }
}

MainWindow::~MainWindow()
{
    qDeleteAll(currentHabitList);
    delete ui;
}

void MainWindow::onChangePasswordClicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Change Password");
    dlg.setModal(true);
    QFormLayout *layout = new QFormLayout(&dlg);

    QLineEdit *oldPass = new QLineEdit(&dlg);
    oldPass->setEchoMode(QLineEdit::Password);
    layout->addRow("Old Password:", oldPass);

    QLineEdit *newPass = new QLineEdit(&dlg);
    newPass->setEchoMode(QLineEdit::Password);
    layout->addRow("New Password:", newPass);

    QLineEdit *confirmPass = new QLineEdit(&dlg);
    confirmPass->setEchoMode(QLineEdit::Password);
    layout->addRow("Confirm Password:", confirmPass);

    QDialogButtonBox *btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    layout->addRow(btns);

    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted)
    {
        if (newPass->text() != confirmPass->text())
        {
            QMessageBox::warning(this, "Error", "New passwords do not match.");
            return;
        }

        QString error = Utils::validatePassword(newPass->text());
        if (!error.isEmpty())
        {
            QMessageBox::warning(this, "Validation Error", error);
            return;
        }

        try
        {
            if (myManager.changePassword(userId, userRole, oldPass->text(), newPass->text()))
            {
                QMessageBox::information(this, "Success", "Password changed successfully.");
            }
        }
        catch (const Acadence::Exception &e)
        {
            QMessageBox::critical(this, "Error", e.what());
        }
    }
}

// ========================== DASHBOARD ==========================

void MainWindow::refreshDashboard()
{
    ui->noticeListWidget->clear();
    QVector<Notice> notices;
    try
    {
        notices = myManager.getNotices();
    }
    catch (const Acadence::Exception &e)
    {
        // Notices are non-critical, log or ignore
    }
    for (const auto &n : notices)
    {
        ui->noticeListWidget->addItem("[" + n.getDate() + "] " + n.getAuthor() + ": " + n.getContent());
    }

    // Update Next Class - Only for Students
    if (userRole == "Student")
    {
        QString nextClass = myManager.getNextClass(userId);
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
        try
        {
            myManager.addNotice(content, userName);
        }
        catch (const Acadence::Exception &e)
        {
            QMessageBox::critical(this, "Error", e.what());
        }
        refreshDashboard();
    }
}

void MainWindow::on_logoutButton_clicked()
{
    QApplication::exit(99);
}

// ========================== PLANNER ==========================

void MainWindow::refreshPlanner()
{
    ui->taskListWidget->clear();
    QVector<Task> tasks = myManager.getTasks(userId);
    for (const auto &t : tasks)
    {
        QString status = t.getIsCompleted() ? "[DONE] " : "[TODO] ";
        QListWidgetItem *item = new QListWidgetItem(status + t.getDescription());
        item->setData(Qt::UserRole, t.getId());
        if (t.getIsCompleted())
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

// ========================== HABITS ==========================

void MainWindow::refreshHabits()
{
    // 1. Refresh Prayers
    DailyPrayerStatus prayers = myManager.getDailyPrayers(userId, QDate::currentDate().toString(Qt::ISODate));
    ui->chkFajr->setChecked(prayers.getFajr());
    ui->chkDhuhr->setChecked(prayers.getDhuhr());
    ui->chkAsr->setChecked(prayers.getAsr());
    ui->chkMaghrib->setChecked(prayers.getMaghrib());
    ui->chkIsha->setChecked(prayers.getIsha());

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

// ========================== ROUTINE ==========================

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
    QVector<RoutineSession> items = myManager.getRoutineForDay(day, semester);

    QTime currentTime = QTime::currentTime();
    QString currentDay = QDate::currentDate().toString("dddd");
    bool isToday = (day == currentDay);

    for (const auto &i : items)
    {
        QString status = "Upcoming";

        if (isToday)
        {
            QTime classTime = QTime::fromString(i.getStartTime(), "HH:mm");
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
        ui->tableRoutine->setItem(row, 0, new QTableWidgetItem(i.getStartTime() + " - " + i.getEndTime()));
        ui->tableRoutine->setItem(row, 1, new QTableWidgetItem(i.getCourseCode() + ": " + i.getCourseName()));
        ui->tableRoutine->setItem(row, 2, new QTableWidgetItem(i.getRoom()));
        ui->tableRoutine->setItem(row, 3, new QTableWidgetItem(i.getInstructor()));
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
    QVector<RoutineSession> items = myManager.getRoutineForDay(day);

    for (const auto &i : items)
    {
        int row = ui->tableTeacherRoutine->rowCount();
        ui->tableTeacherRoutine->insertRow(row);
        ui->tableTeacherRoutine->setItem(row, 0, new QTableWidgetItem(i.getStartTime() + " - " + i.getEndTime()));
        ui->tableTeacherRoutine->setItem(row, 1, new QTableWidgetItem(i.getCourseCode() + ": " + i.getCourseName()));
        ui->tableTeacherRoutine->setItem(row, 2, new QTableWidgetItem(i.getRoom()));
        ui->tableTeacherRoutine->setItem(row, 3, new QTableWidgetItem(QString::number(i.getSemester())));
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

// ========================== ACADEMICS (Student) ==========================

void MainWindow::refreshAcademics()
{
    // Assessments
    ui->listAssessments->clear();
    QVector<Assessment> assessments = myManager.getAssessments(); // Get all for now
    for (const auto &a : assessments)
    {
        ui->listAssessments->addItem(a.getDate() + " - " + a.getCourseName() + ": " + a.getTitle() + " (" + a.getType() + ")");
    }

    // Attendance & Grades
    ui->tableAcademics->setRowCount(0);
    QVector<AttendanceRecord> att = myManager.getStudentAttendance(userId);
    for (int i = 0; i < att.size(); ++i)
    {
        ui->tableAcademics->insertRow(i);
        ui->tableAcademics->setItem(i, 0, new QTableWidgetItem(att[i].getCourseName()));

        double pct = (att[i].getTotalClasses() > 0) ? (double)att[i].getAttendedClasses() / att[i].getTotalClasses() * 100.0 : 0.0;
        QTableWidgetItem *pctItem = new QTableWidgetItem(QString::number(pct, 'f', 1) + "%");
        if (pct < 85.0)
            pctItem->setForeground(Qt::red);
        ui->tableAcademics->setItem(i, 1, pctItem);

        QString scoreStr = QString::number(att[i].getTotalMarksObtained()) + " / " + QString::number(att[i].getTotalMaxMarks());
        ui->tableAcademics->setItem(i, 2, new QTableWidgetItem(scoreStr));

        QString status = (pct < 85.0) ? "Low Attendance" : "Good";
        ui->tableAcademics->setItem(i, 3, new QTableWidgetItem(status));
    }
}

// ========================== TEACHER TOOLS ==========================

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
        ui->comboTeacherAssessment->addItem(a.getCourseName() + " - " + a.getTitle(), a.getId());
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
        if (a.getId() == assessmentId)
        {
            courseId = a.getCourseId();
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

// ========================== ATTENDANCE ==========================

void MainWindow::refreshTeacherAttendance()
{
    ui->tableAttendance->clear();
    int courseId = 0;
    if (ui->comboAttendanceCourse->count() > 0)
    {
        courseId = ui->comboAttendanceCourse->currentData().toInt();
    }
    else
    {
        return;
    }
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

// ========================== QUERIES ==========================

void MainWindow::refreshQueries()
{
    ui->listQueries->clear();
    QVector<Query> queries = myManager.getQueries(userId, userRole);

    for (const auto &q : queries)
    {
        QString label;
        if (userRole == "Student")
        {
            label = "Q: " + q.getQuestion() + "\n   A: " + (q.getAnswer().isEmpty() ? "(Waiting...)" : q.getAnswer());
        }
        else
        {
            label = "[" + q.getStudentName() + "] Q: " + q.getQuestion() + "\n   A: " + (q.getAnswer().isEmpty() ? "(Select to Reply)" : q.getAnswer());
        }

        QListWidgetItem *item = new QListWidgetItem(label);
        item->setData(Qt::UserRole, q.getId());
        if (q.getAnswer().isEmpty())
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

// ========================== ADMIN PANEL ==========================

// Helper to save model data to text file
void saveTableData(QStandardItemModel *model, const QString &tableName)
{
    QVector<QStringList> data;
    for (int i = 0; i < model->rowCount(); ++i)
    {
        QStringList rowData;
        for (int j = 0; j < model->columnCount(); ++j)
        {
            QStandardItem *item = model->item(i, j);
            rowData << (item ? item->text() : "");
        }
        data.append(rowData);
    }
    AcadenceManager::writeCsv(tableName + ".csv", data);
}

void MainWindow::on_tableComboBox_currentTextChanged(const QString &tableName)
{
    adminModel->clear();

    // Disconnect previous connections to prevent multiple saves during load
    disconnect(adminModel, nullptr, this, nullptr);
    csvDelegate->currentTable = tableName;

    QVector<QStringList> data;
    try
    {
        data = AcadenceManager::readCsv(tableName + ".csv");
    }
    catch (const Acadence::Exception &e)
    {
        QMessageBox::warning(this, "Load Error", e.what());
    }
    for (const auto &row : data)
    {
        QList<QStandardItem *> items;
        for (const QString &field : row)
            items.append(new QStandardItem(field));
        adminModel->appendRow(items);
    }

    // Set Headers
    QStringList headers;
    if (tableName == "admins")
        headers << "ID" << "Username" << "Password" << "Name" << "Email";
    else if (tableName == "students")
        headers << "ID" << "Name" << "Email" << "Username" << "Password" << "Dept" << "Batch" << "Sem" << "Admission Date" << "CGPA";
    else if (tableName == "teachers")
        headers << "ID" << "Name" << "Email" << "Username" << "Password" << "Dept" << "Designation" << "Salary";
    else if (tableName == "courses")
        headers << "ID" << "Code" << "Name" << "Teacher ID" << "Semester" << "Credits";
    else if (tableName == "routine")
        headers << "Day" << "Start" << "End" << "Code" << "Name" << "Room" << "Instructor" << "Semester";
    else if (tableName == "grades")
        headers << "Student ID" << "Assessment ID" << "Marks";
    else if (tableName == "notices")
        headers << "Date" << "Author" << "Content";
    adminModel->setHorizontalHeaderLabels(headers);

    // Auto-save on edit
    connect(adminModel, &QStandardItemModel::itemChanged, this, [this, tableName]()
            { saveTableData(adminModel, tableName); });

    ui->adminTableView->resizeColumnsToContents();
}

void MainWindow::on_btnAddRow_clicked()
{
    // Insert a new empty row at the end
    int row = adminModel->rowCount();
    adminModel->insertRow(row);

    QString currentTable = ui->tableComboBox->currentText();
    QString nextIdStr = "";

    // Auto-generate ID for tables with a primary key at column 0
    if (currentTable == "admins" || currentTable == "students" ||
        currentTable == "teachers" || currentTable == "courses")
    {
        int maxId = 0;
        for (int i = 0; i < row; ++i)
        {
            QStandardItem *item = adminModel->item(i, 0);
            if (item && item->text().toInt() > maxId)
                maxId = item->text().toInt();
        }
        nextIdStr = QString::number(maxId + 1);
    }

    // Initialize items to prevent null pointers if saved immediately
    for (int j = 0; j < adminModel->columnCount(); ++j)
    {
        if (j == 0 && !nextIdStr.isEmpty())
            adminModel->setItem(row, j, new QStandardItem(nextIdStr));
        else
            adminModel->setItem(row, j, new QStandardItem(""));
    }

    // Trigger save
    saveTableData(adminModel, currentTable);
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

    // Trigger save
    saveTableData(adminModel, ui->tableComboBox->currentText());
}

void MainWindow::on_searchLineEdit_textChanged(const QString &arg1)
{
    adminProxyModel->setFilterFixedString(arg1);
}

// ========================== CSV DELEGATE ==========================

CsvDelegate::CsvDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

QWidget *CsvDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int col = index.column();

    // Admins: ID,Username,Password,Name,Email
    if (currentTable == "admins")
    {
        if (col == 0)
        { // ID
            QSpinBox *sb = new QSpinBox(parent);
            sb->setRange(1, 999999);
            return sb;
        }
    }
    // Students: ID,Name,Email,Username,Password,Dept,Batch,Sem,StudentID,DateAdmission,CGPA
    else if (currentTable == "students")
    {
        if (col == 0)
        { // ID
            QSpinBox *sb = new QSpinBox(parent);
            sb->setRange(1, 999999);
            return sb;
        }
        if (col == 7)
        { // Sem
            QSpinBox *sb = new QSpinBox(parent);
            sb->setRange(1, 8);
            return sb;
        }
        if (col == 8)
        { // DateAdmission
            QDateEdit *de = new QDateEdit(parent);
            de->setDisplayFormat("yyyy-MM-dd");
            de->setCalendarPopup(true);
            return de;
        }
        if (col == 9)
        { // CGPA
            QDoubleSpinBox *dsb = new QDoubleSpinBox(parent);
            dsb->setRange(0.0, 4.0);
            dsb->setSingleStep(0.01);
            return dsb;
        }
        if (col == 5)
        { // Dept
            QComboBox *cb = new QComboBox(parent);
            cb->addItems({"CSE", "EEE", "MCE", "CEE", "BTM", "TVE", "SWE"});
            return cb;
        }
    }
    // Teachers: ID,Name,Email,Username,Password,Dept,Designation,TeacherID,Salary
    else if (currentTable == "teachers")
    {
        if (col == 0)
        {
            QSpinBox *sb = new QSpinBox(parent);
            sb->setRange(1, 999999);
            return sb;
        }
        if (col == 7)
        { // Salary
            QDoubleSpinBox *dsb = new QDoubleSpinBox(parent);
            dsb->setRange(0.0, 1000000.0);
            return dsb;
        }
        if (col == 5)
        { // Dept
            QComboBox *cb = new QComboBox(parent);
            cb->addItems({"CSE", "EEE", "MCE", "CEE", "BTM", "TVE", "SWE"});
            return cb;
        }
    }
    // Courses: ID,Code,Name,TeacherID,Semester,Credits
    else if (currentTable == "courses")
    {
        if (col == 0 || col == 3 || col == 5)
        {
            QSpinBox *sb = new QSpinBox(parent);
            sb->setRange(1, 999999);
            return sb;
        }
        if (col == 4)
        { // Semester
            QSpinBox *sb = new QSpinBox(parent);
            sb->setRange(1, 8);
            return sb;
        }
    }
    // Routine: Day,Start,End,Code,Name,Room,Instructor,Semester
    else if (currentTable == "routine")
    {
        if (col == 1 || col == 2)
        { // Start, End
            QTimeEdit *te = new QTimeEdit(parent);
            te->setDisplayFormat("HH:mm");
            return te;
        }
        if (col == 7)
        { // Semester
            QSpinBox *sb = new QSpinBox(parent);
            sb->setRange(1, 8);
            return sb;
        }
    }
    // Notices: Date,Author,Content
    else if (currentTable == "notices")
    {
        if (col == 0)
        {
            QDateEdit *de = new QDateEdit(parent);
            de->setDisplayFormat("yyyy-MM-dd");
            de->setCalendarPopup(true);
            return de;
        }
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void CsvDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString val = index.model()->data(index, Qt::EditRole).toString();
    if (QSpinBox *sb = qobject_cast<QSpinBox *>(editor))
        sb->setValue(val.toInt());
    else if (QDoubleSpinBox *dsb = qobject_cast<QDoubleSpinBox *>(editor))
        dsb->setValue(val.toDouble());
    else if (QComboBox *cb = qobject_cast<QComboBox *>(editor))
        cb->setCurrentText(val);
    else if (QDateEdit *de = qobject_cast<QDateEdit *>(editor))
        de->setDate(QDate::fromString(val, "yyyy-MM-dd"));
    else if (QTimeEdit *te = qobject_cast<QTimeEdit *>(editor))
        te->setTime(QTime::fromString(val, "HH:mm"));
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void CsvDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // --- 1. Get Value from Editor ---
    int col = index.column();
    QString newVal;

    if (QSpinBox *sb = qobject_cast<QSpinBox *>(editor))
        newVal = QString::number(sb->value());
    else if (QDoubleSpinBox *dsb = qobject_cast<QDoubleSpinBox *>(editor))
        newVal = QString::number(dsb->value());
    else if (QComboBox *cb = qobject_cast<QComboBox *>(editor))
        newVal = cb->currentText();
    else if (QDateEdit *de = qobject_cast<QDateEdit *>(editor))
        newVal = de->date().toString("yyyy-MM-dd");
    else if (QTimeEdit *te = qobject_cast<QTimeEdit *>(editor))
        newVal = te->time().toString("HH:mm");
    else if (QLineEdit *le = qobject_cast<QLineEdit *>(editor)) // Handle default text editor
        newVal = le->text();
    else
    {
        // Fallback for any other editor type
        QStyledItemDelegate::setModelData(editor, model, index);
        return;
    }

    // --- 2. Perform Validation ---

    // ID Validation (Column 0 for user tables)
    if (col == 0 && (currentTable == "admins" || currentTable == "students" || currentTable == "teachers" || currentTable == "courses"))
    {
        // Check for duplicate ID in current model
        for (int i = 0; i < model->rowCount(); ++i)
        {
            if (i != index.row() && model->index(i, 0).data(Qt::EditRole).toString() == newVal)
            {
                QMessageBox::warning(editor->parentWidget(), "Validation Error", "ID must be unique.");
                return; // Reject change
            }
        }
    }

    // Username Validation
    // Admins: Col 1, Students: Col 3, Teachers: Col 3
    bool isUsernameCol = (currentTable == "admins" && col == 1) ||
                         ((currentTable == "students" || currentTable == "teachers") && col == 3);

    if (isUsernameCol)
    {
        QString error = Utils::validateUsername(newVal);
        if (!error.isEmpty())
        {
            QMessageBox::warning(editor->parentWidget(), "Validation Error", error);
            return;
        }

        // Check uniqueness across all user tables
        QStringList userTables = {"admins", "students", "teachers"};
        for (const QString &table : userTables)
        {
            int uCol = (table == "admins") ? 1 : 3;
            if (table == currentTable)
            {
                for (int i = 0; i < model->rowCount(); ++i)
                {
                    if (i != index.row() && model->index(i, uCol).data(Qt::EditRole).toString() == newVal)
                    {
                        QMessageBox::warning(editor->parentWidget(), "Validation Error", "Username '" + newVal + "' already taken.");
                        return;
                    }
                }
            }
            else
            {
                QVector<QStringList> data;
                try
                {
                    data = AcadenceManager::readCsv(table + ".csv");
                }
                catch (...)
                {
                    continue;
                }

                for (const auto &row : data)
                {
                    if (row.size() > uCol && row[uCol] == newVal)
                    {
                        QMessageBox::warning(editor->parentWidget(), "Validation Error", "Username '" + newVal + "' already taken in " + table + ".");
                        return;
                    }
                }
            }
        }
    }

    // Password Validation
    // Admins: Col 2, Students: Col 4, Teachers: Col 4
    bool isPasswordCol = (currentTable == "admins" && col == 2) ||
                         ((currentTable == "students" || currentTable == "teachers") && col == 4);

    if (isPasswordCol)
    {
        QString error = Utils::validatePassword(newVal);
        if (!error.isEmpty())
        {
            QMessageBox::warning(editor->parentWidget(), "Validation Error", error);
            return;
        }
    }

    // --- 3. Apply Data ---
    model->setData(index, newVal, Qt::EditRole);
}