/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_dashboard;
    QVBoxLayout *verticalLayout_dash;
    QLabel *label_welcome;
    QGroupBox *groupBox_profile;
    QFormLayout *formLayout_profile;
    QLabel *lbl_p_name;
    QLabel *val_p_name;
    QLabel *lbl_p_id;
    QLabel *val_p_id;
    QLabel *lbl_p_dept;
    QLabel *val_p_dept;
    QLabel *lbl_p_sem;
    QLabel *val_p_sem;
    QLabel *lbl_p_email;
    QLabel *val_p_email;
    QLabel *label_nextClass;
    QLabel *label_notices;
    QListWidget *noticeListWidget;
    QPushButton *addNoticeButton;
    QPushButton *logoutButton;
    QWidget *tab_planner;
    QVBoxLayout *verticalLayout_plan;
    QListWidget *taskListWidget;
    QHBoxLayout *horizontalLayout_task;
    QLineEdit *taskLineEdit;
    QPushButton *addTaskButton;
    QPushButton *completeTaskButton;
    QGroupBox *groupBox_timer;
    QVBoxLayout *verticalLayout_timer;
    QLabel *label_timerDisplay;
    QHBoxLayout *horizontalLayout_timerControls;
    QLabel *label_min;
    QSpinBox *spinTimerMinutes;
    QPushButton *btnTimerStart;
    QPushButton *btnTimerPause;
    QPushButton *btnTimerStop;
    QWidget *tab_habits;
    QVBoxLayout *verticalLayout_habit;
    QGroupBox *groupBox_prayers;
    QHBoxLayout *hbox_prayers;
    QCheckBox *chkFajr;
    QCheckBox *chkDhuhr;
    QCheckBox *chkAsr;
    QCheckBox *chkMaghrib;
    QCheckBox *chkIsha;
    QListWidget *habitListWidget;
    QHBoxLayout *horizontalLayout_habitButtons;
    QPushButton *btnAddHabit;
    QPushButton *btnPerformHabit;
    QPushButton *btnDeleteHabit;
    QGroupBox *groupBox_workoutTimer;
    QVBoxLayout *verticalLayout_workoutTimer;
    QLabel *label_workoutTimerDisplay;
    QHBoxLayout *horizontalLayout_workoutTimerControls;
    QLabel *label_workoutMin;
    QSpinBox *spinWorkoutMinutes;
    QPushButton *btnWorkoutStart;
    QPushButton *btnWorkoutPause;
    QPushButton *btnWorkoutStop;
    QWidget *tab_routine;
    QVBoxLayout *verticalLayout_routine;
    QComboBox *comboRoutineDay;
    QTableWidget *tableRoutine;
    QWidget *tab_academics;
    QVBoxLayout *verticalLayout_academics;
    QLabel *lbl_assessments;
    QListWidget *listAssessments;
    QLabel *lbl_attendance;
    QTableWidget *tableAcademics;
    QWidget *tab_teacher_routine;
    QVBoxLayout *verticalLayout_teacher_routine;
    QGroupBox *grp_routine_manage;
    QHBoxLayout *hbox_routine_manage;
    QComboBox *comboRoutineDayInput;
    QLineEdit *editRoutineTime;
    QLineEdit *editRoutineEndTime;
    QComboBox *comboRoutineCourse;
    QLineEdit *editRoutineRoom;
    QPushButton *btnAddRoutine_Teacher;
    QTableWidget *tableTeacherRoutine;
    QWidget *tab_teacher_assessment;
    QVBoxLayout *verticalLayout_teacher_assessment;
    QGroupBox *grp_assessment;
    QHBoxLayout *hbox_assessment;
    QComboBox *comboTeacherCourse;
    QLineEdit *editAssessmentTitle;
    QComboBox *comboAssessmentType;
    QSpinBox *spinMaxMarks;
    QPushButton *btnCreateAssessment;
    QSpacerItem *verticalSpacer_assessment;
    QWidget *tab_teacher_grades;
    QVBoxLayout *verticalLayout_teacher_grades;
    QGroupBox *grp_grading;
    QVBoxLayout *vbox_grading;
    QHBoxLayout *hbox_grading_controls;
    QComboBox *comboTeacherAssessment;
    QPushButton *btnSaveGrades;
    QTableWidget *tableGrading;
    QWidget *tab_teacher_attendance;
    QVBoxLayout *verticalLayout_teacher_attendance;
    QHBoxLayout *hbox_attendance_controls;
    QComboBox *comboAttendanceCourse;
    QPushButton *btnAddClassDate;
    QPushButton *btnSaveAttendance;
    QTableWidget *tableAttendance;
    QWidget *tab_queries;
    QVBoxLayout *verticalLayout_queries;
    QListWidget *listQueries;
    QHBoxLayout *hbox_queries;
    QLineEdit *editQueryInput;
    QPushButton *btnQueryAction;
    QWidget *tab_admin;
    QVBoxLayout *verticalLayout_admin;
    QComboBox *tableComboBox;
    QLineEdit *searchLineEdit;
    QTableView *adminTableView;
    QHBoxLayout *horizontalLayout_buttons;
    QPushButton *btnAddRow;
    QPushButton *btnDeleteRow;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tab_dashboard = new QWidget();
        tab_dashboard->setObjectName("tab_dashboard");
        verticalLayout_dash = new QVBoxLayout(tab_dashboard);
        verticalLayout_dash->setObjectName("verticalLayout_dash");
        label_welcome = new QLabel(tab_dashboard);
        label_welcome->setObjectName("label_welcome");
        QFont font;
        font.setPointSize(16);
        font.setBold(true);
        label_welcome->setFont(font);

        verticalLayout_dash->addWidget(label_welcome);

        groupBox_profile = new QGroupBox(tab_dashboard);
        groupBox_profile->setObjectName("groupBox_profile");
        formLayout_profile = new QFormLayout(groupBox_profile);
        formLayout_profile->setObjectName("formLayout_profile");
        lbl_p_name = new QLabel(groupBox_profile);
        lbl_p_name->setObjectName("lbl_p_name");

        formLayout_profile->setWidget(0, QFormLayout::ItemRole::LabelRole, lbl_p_name);

        val_p_name = new QLabel(groupBox_profile);
        val_p_name->setObjectName("val_p_name");

        formLayout_profile->setWidget(0, QFormLayout::ItemRole::FieldRole, val_p_name);

        lbl_p_id = new QLabel(groupBox_profile);
        lbl_p_id->setObjectName("lbl_p_id");

        formLayout_profile->setWidget(1, QFormLayout::ItemRole::LabelRole, lbl_p_id);

        val_p_id = new QLabel(groupBox_profile);
        val_p_id->setObjectName("val_p_id");

        formLayout_profile->setWidget(1, QFormLayout::ItemRole::FieldRole, val_p_id);

        lbl_p_dept = new QLabel(groupBox_profile);
        lbl_p_dept->setObjectName("lbl_p_dept");

        formLayout_profile->setWidget(2, QFormLayout::ItemRole::LabelRole, lbl_p_dept);

        val_p_dept = new QLabel(groupBox_profile);
        val_p_dept->setObjectName("val_p_dept");

        formLayout_profile->setWidget(2, QFormLayout::ItemRole::FieldRole, val_p_dept);

        lbl_p_sem = new QLabel(groupBox_profile);
        lbl_p_sem->setObjectName("lbl_p_sem");

        formLayout_profile->setWidget(3, QFormLayout::ItemRole::LabelRole, lbl_p_sem);

        val_p_sem = new QLabel(groupBox_profile);
        val_p_sem->setObjectName("val_p_sem");

        formLayout_profile->setWidget(3, QFormLayout::ItemRole::FieldRole, val_p_sem);

        lbl_p_email = new QLabel(groupBox_profile);
        lbl_p_email->setObjectName("lbl_p_email");

        formLayout_profile->setWidget(4, QFormLayout::ItemRole::LabelRole, lbl_p_email);

        val_p_email = new QLabel(groupBox_profile);
        val_p_email->setObjectName("val_p_email");

        formLayout_profile->setWidget(4, QFormLayout::ItemRole::FieldRole, val_p_email);


        verticalLayout_dash->addWidget(groupBox_profile);

        label_nextClass = new QLabel(tab_dashboard);
        label_nextClass->setObjectName("label_nextClass");

        verticalLayout_dash->addWidget(label_nextClass);

        label_notices = new QLabel(tab_dashboard);
        label_notices->setObjectName("label_notices");

        verticalLayout_dash->addWidget(label_notices);

        noticeListWidget = new QListWidget(tab_dashboard);
        noticeListWidget->setObjectName("noticeListWidget");

        verticalLayout_dash->addWidget(noticeListWidget);

        addNoticeButton = new QPushButton(tab_dashboard);
        addNoticeButton->setObjectName("addNoticeButton");

        verticalLayout_dash->addWidget(addNoticeButton);

        logoutButton = new QPushButton(tab_dashboard);
        logoutButton->setObjectName("logoutButton");

        verticalLayout_dash->addWidget(logoutButton);

        tabWidget->addTab(tab_dashboard, QString());
        tab_planner = new QWidget();
        tab_planner->setObjectName("tab_planner");
        verticalLayout_plan = new QVBoxLayout(tab_planner);
        verticalLayout_plan->setObjectName("verticalLayout_plan");
        taskListWidget = new QListWidget(tab_planner);
        taskListWidget->setObjectName("taskListWidget");

        verticalLayout_plan->addWidget(taskListWidget);

        horizontalLayout_task = new QHBoxLayout();
        horizontalLayout_task->setObjectName("horizontalLayout_task");
        taskLineEdit = new QLineEdit(tab_planner);
        taskLineEdit->setObjectName("taskLineEdit");

        horizontalLayout_task->addWidget(taskLineEdit);

        addTaskButton = new QPushButton(tab_planner);
        addTaskButton->setObjectName("addTaskButton");

        horizontalLayout_task->addWidget(addTaskButton);


        verticalLayout_plan->addLayout(horizontalLayout_task);

        completeTaskButton = new QPushButton(tab_planner);
        completeTaskButton->setObjectName("completeTaskButton");

        verticalLayout_plan->addWidget(completeTaskButton);

        groupBox_timer = new QGroupBox(tab_planner);
        groupBox_timer->setObjectName("groupBox_timer");
        verticalLayout_timer = new QVBoxLayout(groupBox_timer);
        verticalLayout_timer->setObjectName("verticalLayout_timer");
        label_timerDisplay = new QLabel(groupBox_timer);
        label_timerDisplay->setObjectName("label_timerDisplay");
        QFont font1;
        font1.setPointSize(72);
        font1.setBold(true);
        label_timerDisplay->setFont(font1);
        label_timerDisplay->setAlignment(Qt::AlignCenter);

        verticalLayout_timer->addWidget(label_timerDisplay);

        horizontalLayout_timerControls = new QHBoxLayout();
        horizontalLayout_timerControls->setObjectName("horizontalLayout_timerControls");
        label_min = new QLabel(groupBox_timer);
        label_min->setObjectName("label_min");

        horizontalLayout_timerControls->addWidget(label_min);

        spinTimerMinutes = new QSpinBox(groupBox_timer);
        spinTimerMinutes->setObjectName("spinTimerMinutes");
        spinTimerMinutes->setValue(25);

        horizontalLayout_timerControls->addWidget(spinTimerMinutes);

        btnTimerStart = new QPushButton(groupBox_timer);
        btnTimerStart->setObjectName("btnTimerStart");

        horizontalLayout_timerControls->addWidget(btnTimerStart);

        btnTimerPause = new QPushButton(groupBox_timer);
        btnTimerPause->setObjectName("btnTimerPause");

        horizontalLayout_timerControls->addWidget(btnTimerPause);

        btnTimerStop = new QPushButton(groupBox_timer);
        btnTimerStop->setObjectName("btnTimerStop");

        horizontalLayout_timerControls->addWidget(btnTimerStop);


        verticalLayout_timer->addLayout(horizontalLayout_timerControls);


        verticalLayout_plan->addWidget(groupBox_timer);

        tabWidget->addTab(tab_planner, QString());
        tab_habits = new QWidget();
        tab_habits->setObjectName("tab_habits");
        verticalLayout_habit = new QVBoxLayout(tab_habits);
        verticalLayout_habit->setObjectName("verticalLayout_habit");
        groupBox_prayers = new QGroupBox(tab_habits);
        groupBox_prayers->setObjectName("groupBox_prayers");
        hbox_prayers = new QHBoxLayout(groupBox_prayers);
        hbox_prayers->setObjectName("hbox_prayers");
        chkFajr = new QCheckBox(groupBox_prayers);
        chkFajr->setObjectName("chkFajr");

        hbox_prayers->addWidget(chkFajr);

        chkDhuhr = new QCheckBox(groupBox_prayers);
        chkDhuhr->setObjectName("chkDhuhr");

        hbox_prayers->addWidget(chkDhuhr);

        chkAsr = new QCheckBox(groupBox_prayers);
        chkAsr->setObjectName("chkAsr");

        hbox_prayers->addWidget(chkAsr);

        chkMaghrib = new QCheckBox(groupBox_prayers);
        chkMaghrib->setObjectName("chkMaghrib");

        hbox_prayers->addWidget(chkMaghrib);

        chkIsha = new QCheckBox(groupBox_prayers);
        chkIsha->setObjectName("chkIsha");

        hbox_prayers->addWidget(chkIsha);


        verticalLayout_habit->addWidget(groupBox_prayers);

        habitListWidget = new QListWidget(tab_habits);
        habitListWidget->setObjectName("habitListWidget");

        verticalLayout_habit->addWidget(habitListWidget);

        horizontalLayout_habitButtons = new QHBoxLayout();
        horizontalLayout_habitButtons->setObjectName("horizontalLayout_habitButtons");
        btnAddHabit = new QPushButton(tab_habits);
        btnAddHabit->setObjectName("btnAddHabit");

        horizontalLayout_habitButtons->addWidget(btnAddHabit);

        btnPerformHabit = new QPushButton(tab_habits);
        btnPerformHabit->setObjectName("btnPerformHabit");

        horizontalLayout_habitButtons->addWidget(btnPerformHabit);

        btnDeleteHabit = new QPushButton(tab_habits);
        btnDeleteHabit->setObjectName("btnDeleteHabit");

        horizontalLayout_habitButtons->addWidget(btnDeleteHabit);


        verticalLayout_habit->addLayout(horizontalLayout_habitButtons);

        groupBox_workoutTimer = new QGroupBox(tab_habits);
        groupBox_workoutTimer->setObjectName("groupBox_workoutTimer");
        verticalLayout_workoutTimer = new QVBoxLayout(groupBox_workoutTimer);
        verticalLayout_workoutTimer->setObjectName("verticalLayout_workoutTimer");
        label_workoutTimerDisplay = new QLabel(groupBox_workoutTimer);
        label_workoutTimerDisplay->setObjectName("label_workoutTimerDisplay");
        label_workoutTimerDisplay->setFont(font1);
        label_workoutTimerDisplay->setAlignment(Qt::AlignCenter);

        verticalLayout_workoutTimer->addWidget(label_workoutTimerDisplay);

        horizontalLayout_workoutTimerControls = new QHBoxLayout();
        horizontalLayout_workoutTimerControls->setObjectName("horizontalLayout_workoutTimerControls");
        label_workoutMin = new QLabel(groupBox_workoutTimer);
        label_workoutMin->setObjectName("label_workoutMin");

        horizontalLayout_workoutTimerControls->addWidget(label_workoutMin);

        spinWorkoutMinutes = new QSpinBox(groupBox_workoutTimer);
        spinWorkoutMinutes->setObjectName("spinWorkoutMinutes");
        spinWorkoutMinutes->setValue(30);

        horizontalLayout_workoutTimerControls->addWidget(spinWorkoutMinutes);

        btnWorkoutStart = new QPushButton(groupBox_workoutTimer);
        btnWorkoutStart->setObjectName("btnWorkoutStart");

        horizontalLayout_workoutTimerControls->addWidget(btnWorkoutStart);

        btnWorkoutPause = new QPushButton(groupBox_workoutTimer);
        btnWorkoutPause->setObjectName("btnWorkoutPause");

        horizontalLayout_workoutTimerControls->addWidget(btnWorkoutPause);

        btnWorkoutStop = new QPushButton(groupBox_workoutTimer);
        btnWorkoutStop->setObjectName("btnWorkoutStop");

        horizontalLayout_workoutTimerControls->addWidget(btnWorkoutStop);


        verticalLayout_workoutTimer->addLayout(horizontalLayout_workoutTimerControls);


        verticalLayout_habit->addWidget(groupBox_workoutTimer);

        tabWidget->addTab(tab_habits, QString());
        tab_routine = new QWidget();
        tab_routine->setObjectName("tab_routine");
        verticalLayout_routine = new QVBoxLayout(tab_routine);
        verticalLayout_routine->setObjectName("verticalLayout_routine");
        comboRoutineDay = new QComboBox(tab_routine);
        comboRoutineDay->addItem(QString());
        comboRoutineDay->addItem(QString());
        comboRoutineDay->addItem(QString());
        comboRoutineDay->addItem(QString());
        comboRoutineDay->addItem(QString());
        comboRoutineDay->addItem(QString());
        comboRoutineDay->addItem(QString());
        comboRoutineDay->setObjectName("comboRoutineDay");

        verticalLayout_routine->addWidget(comboRoutineDay);

        tableRoutine = new QTableWidget(tab_routine);
        if (tableRoutine->columnCount() < 5)
            tableRoutine->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableRoutine->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableRoutine->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableRoutine->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableRoutine->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableRoutine->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        tableRoutine->setObjectName("tableRoutine");
        tableRoutine->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableRoutine->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_routine->addWidget(tableRoutine);

        tabWidget->addTab(tab_routine, QString());
        tab_academics = new QWidget();
        tab_academics->setObjectName("tab_academics");
        verticalLayout_academics = new QVBoxLayout(tab_academics);
        verticalLayout_academics->setObjectName("verticalLayout_academics");
        lbl_assessments = new QLabel(tab_academics);
        lbl_assessments->setObjectName("lbl_assessments");

        verticalLayout_academics->addWidget(lbl_assessments);

        listAssessments = new QListWidget(tab_academics);
        listAssessments->setObjectName("listAssessments");

        verticalLayout_academics->addWidget(listAssessments);

        lbl_attendance = new QLabel(tab_academics);
        lbl_attendance->setObjectName("lbl_attendance");

        verticalLayout_academics->addWidget(lbl_attendance);

        tableAcademics = new QTableWidget(tab_academics);
        if (tableAcademics->columnCount() < 4)
            tableAcademics->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableAcademics->setHorizontalHeaderItem(0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableAcademics->setHorizontalHeaderItem(1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableAcademics->setHorizontalHeaderItem(2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableAcademics->setHorizontalHeaderItem(3, __qtablewidgetitem8);
        tableAcademics->setObjectName("tableAcademics");

        verticalLayout_academics->addWidget(tableAcademics);

        tabWidget->addTab(tab_academics, QString());
        tab_teacher_routine = new QWidget();
        tab_teacher_routine->setObjectName("tab_teacher_routine");
        verticalLayout_teacher_routine = new QVBoxLayout(tab_teacher_routine);
        verticalLayout_teacher_routine->setObjectName("verticalLayout_teacher_routine");
        grp_routine_manage = new QGroupBox(tab_teacher_routine);
        grp_routine_manage->setObjectName("grp_routine_manage");
        hbox_routine_manage = new QHBoxLayout(grp_routine_manage);
        hbox_routine_manage->setObjectName("hbox_routine_manage");
        comboRoutineDayInput = new QComboBox(grp_routine_manage);
        comboRoutineDayInput->addItem(QString());
        comboRoutineDayInput->addItem(QString());
        comboRoutineDayInput->addItem(QString());
        comboRoutineDayInput->addItem(QString());
        comboRoutineDayInput->addItem(QString());
        comboRoutineDayInput->addItem(QString());
        comboRoutineDayInput->addItem(QString());
        comboRoutineDayInput->setObjectName("comboRoutineDayInput");

        hbox_routine_manage->addWidget(comboRoutineDayInput);

        editRoutineTime = new QLineEdit(grp_routine_manage);
        editRoutineTime->setObjectName("editRoutineTime");

        hbox_routine_manage->addWidget(editRoutineTime);

        editRoutineEndTime = new QLineEdit(grp_routine_manage);
        editRoutineEndTime->setObjectName("editRoutineEndTime");

        hbox_routine_manage->addWidget(editRoutineEndTime);

        comboRoutineCourse = new QComboBox(grp_routine_manage);
        comboRoutineCourse->setObjectName("comboRoutineCourse");

        hbox_routine_manage->addWidget(comboRoutineCourse);

        editRoutineRoom = new QLineEdit(grp_routine_manage);
        editRoutineRoom->setObjectName("editRoutineRoom");

        hbox_routine_manage->addWidget(editRoutineRoom);

        btnAddRoutine_Teacher = new QPushButton(grp_routine_manage);
        btnAddRoutine_Teacher->setObjectName("btnAddRoutine_Teacher");

        hbox_routine_manage->addWidget(btnAddRoutine_Teacher);


        verticalLayout_teacher_routine->addWidget(grp_routine_manage);

        tableTeacherRoutine = new QTableWidget(tab_teacher_routine);
        if (tableTeacherRoutine->columnCount() < 4)
            tableTeacherRoutine->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableTeacherRoutine->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableTeacherRoutine->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableTeacherRoutine->setHorizontalHeaderItem(2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableTeacherRoutine->setHorizontalHeaderItem(3, __qtablewidgetitem12);
        tableTeacherRoutine->setObjectName("tableTeacherRoutine");
        tableTeacherRoutine->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableTeacherRoutine->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_teacher_routine->addWidget(tableTeacherRoutine);

        tabWidget->addTab(tab_teacher_routine, QString());
        tab_teacher_assessment = new QWidget();
        tab_teacher_assessment->setObjectName("tab_teacher_assessment");
        verticalLayout_teacher_assessment = new QVBoxLayout(tab_teacher_assessment);
        verticalLayout_teacher_assessment->setObjectName("verticalLayout_teacher_assessment");
        grp_assessment = new QGroupBox(tab_teacher_assessment);
        grp_assessment->setObjectName("grp_assessment");
        hbox_assessment = new QHBoxLayout(grp_assessment);
        hbox_assessment->setObjectName("hbox_assessment");
        comboTeacherCourse = new QComboBox(grp_assessment);
        comboTeacherCourse->setObjectName("comboTeacherCourse");

        hbox_assessment->addWidget(comboTeacherCourse);

        editAssessmentTitle = new QLineEdit(grp_assessment);
        editAssessmentTitle->setObjectName("editAssessmentTitle");

        hbox_assessment->addWidget(editAssessmentTitle);

        comboAssessmentType = new QComboBox(grp_assessment);
        comboAssessmentType->addItem(QString());
        comboAssessmentType->addItem(QString());
        comboAssessmentType->setObjectName("comboAssessmentType");

        hbox_assessment->addWidget(comboAssessmentType);

        spinMaxMarks = new QSpinBox(grp_assessment);
        spinMaxMarks->setObjectName("spinMaxMarks");
        spinMaxMarks->setValue(20);

        hbox_assessment->addWidget(spinMaxMarks);

        btnCreateAssessment = new QPushButton(grp_assessment);
        btnCreateAssessment->setObjectName("btnCreateAssessment");

        hbox_assessment->addWidget(btnCreateAssessment);


        verticalLayout_teacher_assessment->addWidget(grp_assessment);

        verticalSpacer_assessment = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_teacher_assessment->addItem(verticalSpacer_assessment);

        tabWidget->addTab(tab_teacher_assessment, QString());
        tab_teacher_grades = new QWidget();
        tab_teacher_grades->setObjectName("tab_teacher_grades");
        verticalLayout_teacher_grades = new QVBoxLayout(tab_teacher_grades);
        verticalLayout_teacher_grades->setObjectName("verticalLayout_teacher_grades");
        grp_grading = new QGroupBox(tab_teacher_grades);
        grp_grading->setObjectName("grp_grading");
        vbox_grading = new QVBoxLayout(grp_grading);
        vbox_grading->setObjectName("vbox_grading");
        hbox_grading_controls = new QHBoxLayout();
        hbox_grading_controls->setObjectName("hbox_grading_controls");
        comboTeacherAssessment = new QComboBox(grp_grading);
        comboTeacherAssessment->setObjectName("comboTeacherAssessment");

        hbox_grading_controls->addWidget(comboTeacherAssessment);

        btnSaveGrades = new QPushButton(grp_grading);
        btnSaveGrades->setObjectName("btnSaveGrades");

        hbox_grading_controls->addWidget(btnSaveGrades);


        vbox_grading->addLayout(hbox_grading_controls);

        tableGrading = new QTableWidget(grp_grading);
        if (tableGrading->columnCount() < 3)
            tableGrading->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableGrading->setHorizontalHeaderItem(0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableGrading->setHorizontalHeaderItem(1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableGrading->setHorizontalHeaderItem(2, __qtablewidgetitem15);
        tableGrading->setObjectName("tableGrading");

        vbox_grading->addWidget(tableGrading);


        verticalLayout_teacher_grades->addWidget(grp_grading);

        tabWidget->addTab(tab_teacher_grades, QString());
        tab_teacher_attendance = new QWidget();
        tab_teacher_attendance->setObjectName("tab_teacher_attendance");
        verticalLayout_teacher_attendance = new QVBoxLayout(tab_teacher_attendance);
        verticalLayout_teacher_attendance->setObjectName("verticalLayout_teacher_attendance");
        hbox_attendance_controls = new QHBoxLayout();
        hbox_attendance_controls->setObjectName("hbox_attendance_controls");
        comboAttendanceCourse = new QComboBox(tab_teacher_attendance);
        comboAttendanceCourse->setObjectName("comboAttendanceCourse");

        hbox_attendance_controls->addWidget(comboAttendanceCourse);

        btnAddClassDate = new QPushButton(tab_teacher_attendance);
        btnAddClassDate->setObjectName("btnAddClassDate");

        hbox_attendance_controls->addWidget(btnAddClassDate);

        btnSaveAttendance = new QPushButton(tab_teacher_attendance);
        btnSaveAttendance->setObjectName("btnSaveAttendance");

        hbox_attendance_controls->addWidget(btnSaveAttendance);


        verticalLayout_teacher_attendance->addLayout(hbox_attendance_controls);

        tableAttendance = new QTableWidget(tab_teacher_attendance);
        tableAttendance->setObjectName("tableAttendance");
        tableAttendance->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout_teacher_attendance->addWidget(tableAttendance);

        tabWidget->addTab(tab_teacher_attendance, QString());
        tab_queries = new QWidget();
        tab_queries->setObjectName("tab_queries");
        verticalLayout_queries = new QVBoxLayout(tab_queries);
        verticalLayout_queries->setObjectName("verticalLayout_queries");
        listQueries = new QListWidget(tab_queries);
        listQueries->setObjectName("listQueries");

        verticalLayout_queries->addWidget(listQueries);

        hbox_queries = new QHBoxLayout();
        hbox_queries->setObjectName("hbox_queries");
        editQueryInput = new QLineEdit(tab_queries);
        editQueryInput->setObjectName("editQueryInput");

        hbox_queries->addWidget(editQueryInput);

        btnQueryAction = new QPushButton(tab_queries);
        btnQueryAction->setObjectName("btnQueryAction");

        hbox_queries->addWidget(btnQueryAction);


        verticalLayout_queries->addLayout(hbox_queries);

        tabWidget->addTab(tab_queries, QString());
        tab_admin = new QWidget();
        tab_admin->setObjectName("tab_admin");
        verticalLayout_admin = new QVBoxLayout(tab_admin);
        verticalLayout_admin->setObjectName("verticalLayout_admin");
        tableComboBox = new QComboBox(tab_admin);
        tableComboBox->setObjectName("tableComboBox");

        verticalLayout_admin->addWidget(tableComboBox);

        searchLineEdit = new QLineEdit(tab_admin);
        searchLineEdit->setObjectName("searchLineEdit");

        verticalLayout_admin->addWidget(searchLineEdit);

        adminTableView = new QTableView(tab_admin);
        adminTableView->setObjectName("adminTableView");
        adminTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        adminTableView->setSortingEnabled(true);

        verticalLayout_admin->addWidget(adminTableView);

        horizontalLayout_buttons = new QHBoxLayout();
        horizontalLayout_buttons->setObjectName("horizontalLayout_buttons");
        btnAddRow = new QPushButton(tab_admin);
        btnAddRow->setObjectName("btnAddRow");

        horizontalLayout_buttons->addWidget(btnAddRow);

        btnDeleteRow = new QPushButton(tab_admin);
        btnDeleteRow->setObjectName("btnDeleteRow");

        horizontalLayout_buttons->addWidget(btnDeleteRow);


        verticalLayout_admin->addLayout(horizontalLayout_buttons);

        tabWidget->addTab(tab_admin, QString());

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Academic Manager", nullptr));
        label_welcome->setText(QCoreApplication::translate("MainWindow", "Welcome!", nullptr));
        groupBox_profile->setTitle(QCoreApplication::translate("MainWindow", "Academic Profile", nullptr));
        lbl_p_name->setText(QCoreApplication::translate("MainWindow", "Name:", nullptr));
        val_p_name->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lbl_p_id->setText(QCoreApplication::translate("MainWindow", "ID:", nullptr));
        val_p_id->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lbl_p_dept->setText(QCoreApplication::translate("MainWindow", "Department:", nullptr));
        val_p_dept->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lbl_p_sem->setText(QCoreApplication::translate("MainWindow", "Semester:", nullptr));
        val_p_sem->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lbl_p_email->setText(QCoreApplication::translate("MainWindow", "Email:", nullptr));
        val_p_email->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_nextClass->setText(QCoreApplication::translate("MainWindow", "Next Class: --", nullptr));
        label_nextClass->setStyleSheet(QCoreApplication::translate("MainWindow", "font-weight: bold; color: #2c3e50; font-size: 14px; margin-bottom: 10px;", nullptr));
        label_notices->setText(QCoreApplication::translate("MainWindow", "Class Notices:", nullptr));
        addNoticeButton->setText(QCoreApplication::translate("MainWindow", "Post Notice (Teacher Only)", nullptr));
        logoutButton->setText(QCoreApplication::translate("MainWindow", "Logout", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_dashboard), QCoreApplication::translate("MainWindow", "Dashboard", nullptr));
        taskLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "New Task Description...", nullptr));
        addTaskButton->setText(QCoreApplication::translate("MainWindow", "Add Task", nullptr));
        completeTaskButton->setText(QCoreApplication::translate("MainWindow", "Mark Selected as Completed", nullptr));
        groupBox_timer->setTitle(QCoreApplication::translate("MainWindow", "Focus Timer", nullptr));
        label_timerDisplay->setText(QCoreApplication::translate("MainWindow", "00:00", nullptr));
        label_min->setText(QCoreApplication::translate("MainWindow", "Minutes:", nullptr));
        btnTimerStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        btnTimerPause->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        btnTimerStop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_planner), QCoreApplication::translate("MainWindow", "Study Planner", nullptr));
        groupBox_prayers->setTitle(QCoreApplication::translate("MainWindow", "Daily Prayers (Today)", nullptr));
        chkFajr->setText(QCoreApplication::translate("MainWindow", "Fajr", nullptr));
        chkDhuhr->setText(QCoreApplication::translate("MainWindow", "Dhuhr", nullptr));
        chkAsr->setText(QCoreApplication::translate("MainWindow", "Asr", nullptr));
        chkMaghrib->setText(QCoreApplication::translate("MainWindow", "Maghrib", nullptr));
        chkIsha->setText(QCoreApplication::translate("MainWindow", "Isha", nullptr));
        btnAddHabit->setText(QCoreApplication::translate("MainWindow", "Create Habit", nullptr));
        btnPerformHabit->setText(QCoreApplication::translate("MainWindow", "Perform / Start Timer", nullptr));
        btnDeleteHabit->setText(QCoreApplication::translate("MainWindow", "Delete Habit", nullptr));
        groupBox_workoutTimer->setTitle(QCoreApplication::translate("MainWindow", "Habit Timer", nullptr));
        label_workoutTimerDisplay->setText(QCoreApplication::translate("MainWindow", "00:00.00", nullptr));
        label_workoutMin->setText(QCoreApplication::translate("MainWindow", "Target:", nullptr));
        btnWorkoutStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        btnWorkoutPause->setText(QCoreApplication::translate("MainWindow", "Pause", nullptr));
        btnWorkoutStop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_habits), QCoreApplication::translate("MainWindow", "Habit Tracker", nullptr));
        comboRoutineDay->setItemText(0, QCoreApplication::translate("MainWindow", "Sunday", nullptr));
        comboRoutineDay->setItemText(1, QCoreApplication::translate("MainWindow", "Monday", nullptr));
        comboRoutineDay->setItemText(2, QCoreApplication::translate("MainWindow", "Tuesday", nullptr));
        comboRoutineDay->setItemText(3, QCoreApplication::translate("MainWindow", "Wednesday", nullptr));
        comboRoutineDay->setItemText(4, QCoreApplication::translate("MainWindow", "Thursday", nullptr));
        comboRoutineDay->setItemText(5, QCoreApplication::translate("MainWindow", "Friday", nullptr));
        comboRoutineDay->setItemText(6, QCoreApplication::translate("MainWindow", "Saturday", nullptr));

        QTableWidgetItem *___qtablewidgetitem = tableRoutine->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Time", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableRoutine->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Course", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableRoutine->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Room", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableRoutine->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Instructor", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableRoutine->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_routine), QCoreApplication::translate("MainWindow", "Routine", nullptr));
        lbl_assessments->setText(QCoreApplication::translate("MainWindow", "Upcoming Assessments:", nullptr));
        lbl_attendance->setText(QCoreApplication::translate("MainWindow", "Attendance & Grades:", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableAcademics->horizontalHeaderItem(0);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Course", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableAcademics->horizontalHeaderItem(1);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Attendance %", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableAcademics->horizontalHeaderItem(2);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Score", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableAcademics->horizontalHeaderItem(3);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_academics), QCoreApplication::translate("MainWindow", "Academics", nullptr));
        grp_routine_manage->setTitle(QCoreApplication::translate("MainWindow", "Manage Routine", nullptr));
        comboRoutineDayInput->setItemText(0, QCoreApplication::translate("MainWindow", "Sunday", nullptr));
        comboRoutineDayInput->setItemText(1, QCoreApplication::translate("MainWindow", "Monday", nullptr));
        comboRoutineDayInput->setItemText(2, QCoreApplication::translate("MainWindow", "Tuesday", nullptr));
        comboRoutineDayInput->setItemText(3, QCoreApplication::translate("MainWindow", "Wednesday", nullptr));
        comboRoutineDayInput->setItemText(4, QCoreApplication::translate("MainWindow", "Thursday", nullptr));
        comboRoutineDayInput->setItemText(5, QCoreApplication::translate("MainWindow", "Friday", nullptr));
        comboRoutineDayInput->setItemText(6, QCoreApplication::translate("MainWindow", "Saturday", nullptr));

        editRoutineTime->setPlaceholderText(QCoreApplication::translate("MainWindow", "Start (09:00)", nullptr));
        editRoutineEndTime->setPlaceholderText(QCoreApplication::translate("MainWindow", "End (10:30)", nullptr));
#if QT_CONFIG(tooltip)
        comboRoutineCourse->setToolTip(QCoreApplication::translate("MainWindow", "Select Course", nullptr));
#endif // QT_CONFIG(tooltip)
        editRoutineRoom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Room", nullptr));
        btnAddRoutine_Teacher->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableTeacherRoutine->horizontalHeaderItem(0);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Time", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableTeacherRoutine->horizontalHeaderItem(1);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Course", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableTeacherRoutine->horizontalHeaderItem(2);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "Room", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableTeacherRoutine->horizontalHeaderItem(3);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "Semester", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_teacher_routine), QCoreApplication::translate("MainWindow", "Routine Management", nullptr));
        grp_assessment->setTitle(QCoreApplication::translate("MainWindow", "Create Assessment", nullptr));
        editAssessmentTitle->setPlaceholderText(QCoreApplication::translate("MainWindow", "Title", nullptr));
        comboAssessmentType->setItemText(0, QCoreApplication::translate("MainWindow", "Quiz", nullptr));
        comboAssessmentType->setItemText(1, QCoreApplication::translate("MainWindow", "Assignment", nullptr));

        btnCreateAssessment->setText(QCoreApplication::translate("MainWindow", "Create", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_teacher_assessment), QCoreApplication::translate("MainWindow", "Post Assessment", nullptr));
        grp_grading->setTitle(QCoreApplication::translate("MainWindow", "Enter Grades", nullptr));
        btnSaveGrades->setText(QCoreApplication::translate("MainWindow", "Save Grades", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableGrading->horizontalHeaderItem(0);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Student ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableGrading->horizontalHeaderItem(1);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableGrading->horizontalHeaderItem(2);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "Marks", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_teacher_grades), QCoreApplication::translate("MainWindow", "Grading", nullptr));
#if QT_CONFIG(tooltip)
        comboAttendanceCourse->setToolTip(QCoreApplication::translate("MainWindow", "Select Course", nullptr));
#endif // QT_CONFIG(tooltip)
        btnAddClassDate->setText(QCoreApplication::translate("MainWindow", "Add Class Column", nullptr));
        btnSaveAttendance->setText(QCoreApplication::translate("MainWindow", "Save Attendance", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_teacher_attendance), QCoreApplication::translate("MainWindow", "Attendance", nullptr));
        editQueryInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ask a question (Student) or Reply (Teacher)...", nullptr));
        btnQueryAction->setText(QCoreApplication::translate("MainWindow", "Send", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_queries), QCoreApplication::translate("MainWindow", "Q&A", nullptr));
#if QT_CONFIG(tooltip)
        tableComboBox->setToolTip(QCoreApplication::translate("MainWindow", "Select Database Table", nullptr));
#endif // QT_CONFIG(tooltip)
        searchLineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Search / Filter...", nullptr));
        btnAddRow->setText(QCoreApplication::translate("MainWindow", "Add Row", nullptr));
        btnDeleteRow->setText(QCoreApplication::translate("MainWindow", "Delete Selected Row", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_admin), QCoreApplication::translate("MainWindow", "Admin Panel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
