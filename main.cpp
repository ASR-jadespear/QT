#include "mainwindow.hpp"
#include "academicmanager.hpp"
#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include "exceptions.hpp"

/**
 * @brief Defines the color palette for the application theme.
 */
struct AppTheme
{
    QString name;
    QColor primary;   // Backgrounds
    QColor secondary; // Buttons & Accents
};

/**
 * @brief Calculates a high-contrast text color (black or white) for a given background.
 */
QColor getContrastColor(const QColor &color)
{
    // Calculate luminance to determine best text color (Black or White)
    double luminance = 0.299 * color.redF() + 0.587 * color.greenF() + 0.114 * color.blueF();
    return luminance > 0.5 ? Qt::black : Qt::white;
}

/**
 * @brief Applies the selected theme to the application.
 */
void applyTheme(QApplication &a, const AppTheme &theme)
{
    a.setStyle(QStyleFactory::create("Fusion"));

    QPalette p;
    QColor primaryText = getContrastColor(theme.primary);
    QColor secondaryText = getContrastColor(theme.secondary);

    p.setColor(QPalette::Window, theme.primary);
    p.setColor(QPalette::WindowText, primaryText);
    p.setColor(QPalette::Base, theme.primary);
    p.setColor(QPalette::AlternateBase, theme.primary); // Keep consistent background
    p.setColor(QPalette::ToolTipBase, theme.primary);
    p.setColor(QPalette::ToolTipText, primaryText);
    p.setColor(QPalette::Text, primaryText);

    // Set global Button role to primary so tabs/panels match the background
    p.setColor(QPalette::Button, theme.primary);
    p.setColor(QPalette::ButtonText, primaryText);

    p.setColor(QPalette::BrightText, Qt::red);
    p.setColor(QPalette::Link, theme.secondary);
    p.setColor(QPalette::Highlight, theme.secondary);
    p.setColor(QPalette::HighlightedText, secondaryText);

    // Placeholder text (semi-transparent version of text color)
    QColor placeholder = primaryText;
    placeholder.setAlpha(128);
    p.setColor(QPalette::PlaceholderText, placeholder);

    a.setPalette(p);

    // Determine checkmark color (White or Black) based on secondary text color
    QString checkmarkSvg = (secondaryText.value() > 128)
                               ? "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0IiBmaWxsPSJub25lIiBzdHJva2U9IndoaXRlIiBzdHJva2Utd2lkdGg9IjMiIHN0cm9rZS1saW5lY2FwPSJyb3VuZCIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCI+PHBvbHlsaW5lIHBvaW50cz0iMjAgNiA5IDE3IDQgMTIiPjwvcG9seWxpbmU+PC9zdmc+"
                               : "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNCIgaGVpZ2h0PSIyNCIgdmlld0JveD0iMCAwIDI0IDI0IiBmaWxsPSJub25lIiBzdHJva2U9ImJsYWNrIiBzdHJva2Utd2lkdGg9IjMiIHN0cm9rZS1saW5lY2FwPSJyb3VuZCIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCI+PHBvbHlsaW5lIHBvaW50cz0iMjAgNiA5IDE3IDQgMTIiPjwvcG9seWxpbmU+PC9zdmc+";

    // Generate Arrow SVGs for SpinBoxes
    QByteArray upArrowXml = QString("<svg xmlns='http://www.w3.org/2000/svg' width='10' height='10' viewBox='0 0 10 10'><path d='M0 10 L5 0 L10 10 Z' fill='%1'/></svg>").arg(secondaryText.name()).toUtf8();
    QString upArrowBase64 = "data:image/svg+xml;base64," + upArrowXml.toBase64();
    QByteArray downArrowXml = QString("<svg xmlns='http://www.w3.org/2000/svg' width='10' height='10' viewBox='0 0 10 10'><path d='M0 0 L5 10 L10 0 Z' fill='%1'/></svg>").arg(secondaryText.name()).toUtf8();
    QString downArrowBase64 = "data:image/svg+xml;base64," + downArrowXml.toBase64();

    // Calculate a subtle border color based on text color (e.g., 30% opacity)
    QColor borderColor = primaryText;
    borderColor.setAlphaF(0.3);
    QString borderRgba = QString("rgba(%1, %2, %3, %4)").arg(borderColor.red()).arg(borderColor.green()).arg(borderColor.blue()).arg(borderColor.alphaF());

    QString qss = QString(
                      // General Widget Styling
                      "QWidget { font-size: 16px; font-family: 'Segoe UI', 'Roboto', sans-serif; }"

                      // Force Timer Font Size
                      "QLabel#label_timerDisplay, QLabel#label_workoutTimerDisplay { font-size: 72px; font-weight: bold; color: %1; font-family: monospace; }"

                      // Buttons
                      "QPushButton { background-color: %1; color: %2; border: none; border-radius: 8px; padding: 10px 20px; font-weight: bold; } "
                      "QPushButton:hover { background-color: %4; } "
                      "QPushButton:pressed { background-color: %5; } "
                      "QPushButton:disabled { background-color: %6; color: %7; border: none; } "

                      // Checkboxes
                      "QCheckBox { spacing: 8px; } "
                      "QCheckBox::indicator { width: 22px; height: 22px; border-radius: 6px; border: 2px solid %8; background: transparent; } "
                      "QCheckBox::indicator:checked { background-color: %1; border-color: %1; image: url(%9); } "
                      "QCheckBox::indicator:unchecked:hover { border-color: %1; } "

                      // Inputs (LineEdit, SpinBox, ComboBox)
                      "QLineEdit, QSpinBox, QComboBox, QDateEdit, QTimeEdit { border: 2px solid %8; border-radius: 8px; padding: 10px 12px; background-color: %10; selection-background-color: %1; selection-color: %2; } "
                      "QLineEdit:focus, QSpinBox:focus, QComboBox:focus { border: 2px solid %1; } "

                      // SpinBox Buttons
                      "QSpinBox::up-button { subcontrol-origin: border; subcontrol-position: top right; width: 20px; border-left-width: 0px; border-top-right-radius: 8px; background: transparent; margin: 2px; } "
                      "QSpinBox::down-button { subcontrol-origin: border; subcontrol-position: bottom right; width: 20px; border-left-width: 0px; border-bottom-right-radius: 8px; background: transparent; margin: 2px; } "
                      "QSpinBox::up-button:hover, QSpinBox::down-button:hover { background: %4; } "
                      "QSpinBox::up-button:pressed, QSpinBox::down-button:pressed { background: %5; } "
                      "QSpinBox::up-arrow { image: url(%12); width: 10px; height: 10px; } "
                      "QSpinBox::down-arrow { image: url(%13); width: 10px; height: 10px; } "

                      "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 30px; border-left-width: 0px; border-top-right-radius: 8px; border-bottom-right-radius: 8px; } "
                      "QComboBox::down-arrow { image: url(%13); width: 12px; height: 12px; } "

                      // GroupBox
                      "QGroupBox { border: 1px solid %8; border-radius: 12px; margin-top: 24px; padding-top: 20px; font-weight: bold; background: %10; } "
                      "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 16px; padding: 0 8px; color: %1; } "

                      // Tab Widget
                      "QTabWidget::pane { border: none; background: %10; } "
                      "QTabWidget::tab-bar { alignment: center; } "
                      "QTabBar::tab { background: transparent; color: %7; border: none; border-bottom: 3px solid transparent; padding: 12px 24px; margin: 0 4px; font-weight: bold; font-size: 14px; } "
                      "QTabBar::tab:selected { color: %1; border-bottom: 3px solid %1; } "
                      "QTabBar::tab:hover { color: %1; background-color: rgba(128, 128, 128, 0.05); border-radius: 4px; } "

                      // Tables & Lists
                      "QTableWidget, QTableView, QListWidget { border: none; border-radius: 8px; gridline-color: transparent; selection-background-color: %1; selection-color: %2; alternate-background-color: %11; background-color: %10; } "
                      "QHeaderView::section { background-color: transparent; padding: 10px; border: none; border-bottom: 2px solid %8; font-weight: bold; text-transform: uppercase; color: %7; } "
                      "QTableCornerButton::section { background-color: %10; border: none; } "
                      "QTableWidget::item { padding: 5px; border-bottom: 1px solid %8; } "

                      // Scrollbars (Minimalist)
                      "QScrollBar:vertical { border: none; background: %10; width: 8px; margin: 0px; border-radius: 4px; } "
                      "QScrollBar::handle:vertical { background: %8; min-height: 20px; border-radius: 4px; } "
                      "QScrollBar::handle:vertical:hover { background: %1; } "
                      "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; } ")
                      .arg(theme.secondary.name())              // %1: Secondary (Button/Accent)
                      .arg(secondaryText.name())                // %2: Secondary Text
                      .arg(theme.secondary.darker(110).name())  // %3: Button Border
                      .arg(theme.secondary.lighter(110).name()) // %4: Button Hover
                      .arg(theme.secondary.darker(120).name())  // %5: Button Pressed
                      .arg(theme.primary.darker(110).name())    // %6: Disabled BG
                      .arg(primaryText.name())                  // %7: Disabled Text (using primary text for visibility)
                      .arg(borderRgba)                          // %8: Subtle Border
                      .arg(checkmarkSvg)                        // %9: Checkmark SVG
                      .arg(theme.primary.name())                // %10: Primary BG (Window)
                      .arg(theme.primary.lighter(110).name())   // %11: Tab Hover
                      .arg(upArrowBase64)                       // %12: Up Arrow
                      .arg(downArrowBase64);                    // %13: Down Arrow

    a.setStyleSheet(qss);
}

/**
 * @brief Ensures that the necessary text files exist for data storage.
 */
void initializeDataFiles()
{
    QFile adminsFile("admins.csv");
    if (!adminsFile.exists())
    {
        if (adminsFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&adminsFile);
            // Format: ID,Username,Password,Name,Email (Admin is a Person only)
            out << "1,admin,admin,System Admin,admin@school.edu\n";
            adminsFile.close();
        }
    }

    QVector<QString> otherFiles = {
        "students.csv", "teachers.csv", "courses.csv", "enrollments.csv",
        "routine.csv", "attendance.csv", "grades.csv", "notices.csv",
        "tasks.csv", "habits.csv", "queries.csv", "assessments.csv", "prayers.csv"};

    for (const QString &fileName : otherFiles)
    {
        QFile file(fileName);
        if (!file.exists() && !file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            // Log warning or handle error, but don't crash main loop yet
        }
        else
        {
            file.close();
        }
    }
}

int main(int argc, char *argv[])
{
    // Initialize Application
    QApplication a(argc, argv);
    a.setApplicationName("Acadence");
    a.setOrganizationName("MyOrganization");

    // === Themes Configuration ===
    QVector<AppTheme> themes = {
        // 1. Deep Space (Dark Blue-Grey & Soft Blue)
        {"Deep Space", QColor("#1E1E2E"), QColor("#89B4FA")},
        // 2. Clean Slate (Off-White & Bootstrap Blue)
        {"Clean Slate", QColor("#F8F9FA"), QColor("#0D6EFD")},
        // 3. Forest Focus (Dark Green-Grey & Vibrant Green)
        {"Forest Focus", QColor("#2D333B"), QColor("#46954A")},
        // 4. Cyber Punk (Midnight Blue & Neon Pink)
        {"Cyber Punk", QColor("#0B132B"), QColor("#FF007F")}};

    int currentThemeIdx = 0;
    applyTheme(a, themes[currentThemeIdx]);

    // Ensure text files exist before login
    initializeDataFiles();

    int exitCode = 0;

    do
    {
        QString role = "";
        int userId = -1;
        QString name = "";

        // Scope the authManager
        {
            AcadenceManager authManager;

            // Custom Login Dialog
            QDialog loginDialog;
            loginDialog.setWindowTitle("Welcome to Acadence");
            loginDialog.setModal(true);
            loginDialog.setFixedSize(1280, 720);

            QVBoxLayout *mainLayout = new QVBoxLayout(&loginDialog);
            mainLayout->addStretch(); // Top spacer

            // Welcome Text (Moved out of card)
            QLabel *welcomeLabel = new QLabel("Welcome to Acadence", &loginDialog);
            welcomeLabel->setAlignment(Qt::AlignCenter);
            welcomeLabel->setStyleSheet("font-size: 48pt; font-weight: bold; margin-bottom: 20px;");
            mainLayout->addWidget(welcomeLabel, 0, Qt::AlignCenter);

            // --- Center Card ---
            QFrame *centerFrame = new QFrame(&loginDialog);
            centerFrame->setFixedWidth(360);
            centerFrame->setStyleSheet(QString(".QFrame { border: 2px solid %1; border-radius: 16px; background-color: palette(base); }").arg(themes[currentThemeIdx].secondary.name()));

            QVBoxLayout *frameLayout = new QVBoxLayout(centerFrame);
            frameLayout->setContentsMargins(40, 40, 40, 40);
            frameLayout->setSpacing(20);

            // Add Shadow to Login Card
            QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(centerFrame);
            shadow->setBlurRadius(30);
            shadow->setXOffset(0);
            shadow->setYOffset(10);
            shadow->setColor(QColor(0, 0, 0, 60));
            centerFrame->setGraphicsEffect(shadow);

            // Inputs
            QLineEdit *userEdit = new QLineEdit(centerFrame);
            userEdit->setPlaceholderText("Username");
            userEdit->setAlignment(Qt::AlignCenter);

            QLineEdit *passEdit = new QLineEdit(centerFrame);
            passEdit->setPlaceholderText("Password");
            passEdit->setEchoMode(QLineEdit::Password);
            passEdit->setAlignment(Qt::AlignCenter);

            frameLayout->addWidget(userEdit);
            frameLayout->addWidget(passEdit);

            // Buttons
            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, centerFrame);
            buttonBox->setCenterButtons(true);
            frameLayout->addWidget(buttonBox);

            mainLayout->addWidget(centerFrame, 0, Qt::AlignCenter);
            mainLayout->addStretch(); // Bottom spacer

            // --- Bottom Right Theme Button ---
            QHBoxLayout *bottomLayout = new QHBoxLayout();
            bottomLayout->addStretch();

            QPushButton *themeBtn = new QPushButton(&loginDialog);
            themeBtn->setFixedSize(40, 40);
            themeBtn->setCursor(Qt::PointingHandCursor);

            auto updateThemeBtn = [&](const AppTheme &t)
            {
                themeBtn->setStyleSheet(QString("QPushButton { background-color: %1; border: 2px solid %2; border-radius: 20px; } QPushButton:hover { border-color: white; }").arg(t.secondary.name()).arg(t.primary.name()));
                themeBtn->setToolTip("Change Theme: " + t.name);
            };
            updateThemeBtn(themes[currentThemeIdx]);

            bottomLayout->addWidget(themeBtn);
            bottomLayout->setContentsMargins(0, 0, 20, 20);
            mainLayout->addLayout(bottomLayout);

            QObject::connect(themeBtn, &QPushButton::clicked, [&]()
                             {
                currentThemeIdx = (currentThemeIdx + 1) % themes.size();
                applyTheme(a, themes[currentThemeIdx]);
                updateThemeBtn(themes[currentThemeIdx]); });

            QObject::connect(buttonBox, &QDialogButtonBox::rejected, &loginDialog, &QDialog::reject);
            QObject::connect(buttonBox, &QDialogButtonBox::accepted, [&]()
                             {
                name = userEdit->text();
                QString inputUsername = userEdit->text(); // Use a distinct variable for input username
                QString inputPassword = passEdit->text();
                
                try {
                    role = authManager.login(inputUsername, inputPassword, userId);
                } catch (const Acadence::Exception &e) {
                    QMessageBox::critical(&loginDialog, "System Error", e.what());
                    return;
                }

                if (!role.isEmpty()) {
                    // Fetch the actual user's name based on role and ID
                    if (role == "Admin") {
                        // Admins are in admins.csv: ID,Username,Password,Name,Email
                        QVector<QStringList> admins = AcadenceManager::readCsv("admins.csv");
                        for (const auto &row : admins) {
                            if (row.size() >= 4 && row[0].toInt() == userId) {
                                name = row[3]; // Get Name
                                break;
                            }
                        }
                    } else if (role == "Student") {
                        Student *s = authManager.getStudent(userId);
                        if (s) {
                            name = s->getName();
                            delete s;
                        }
                    } else if (role == "Teacher") {
                        Teacher *t = authManager.getTeacher(userId);
                        if (t) {
                            name = t->getName();
                            delete t;
                        }
                    }

                    loginDialog.accept();
                } else {
                    QMessageBox::warning(&loginDialog, "Login Failed", "Invalid credentials.\n");
                    passEdit->clear();
                    passEdit->setFocus();
                } });

            if (loginDialog.exec() != QDialog::Accepted)
            {
                return 0; // Cancelled
            }
        }

        MainWindow w(role, userId, name);
        w.show();
        exitCode = a.exec();

    } while (exitCode == 99); // 99 is our custom logout code

    return exitCode;
}