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
                      "QWidget { font-size: 14px; }"

                      // Force Timer Font Size
                      "QLabel#label_timerDisplay, QLabel#label_workoutTimerDisplay { font-size: 72px; font-weight: bold; }"

                      // Buttons
                      "QPushButton { background-color: %1; color: %2; border: 1px solid %3; border-radius: 6px; padding: 6px 12px; font-weight: bold; } "
                      "QPushButton:hover { background-color: %4; border-color: %4; } "
                      "QPushButton:pressed { background-color: %5; border-color: %5; } "
                      "QPushButton:disabled { background-color: %6; color: %7; border: none; } "

                      // Checkboxes
                      "QCheckBox { spacing: 8px; } "
                      "QCheckBox::indicator { width: 20px; height: 20px; border-radius: 4px; border: 1px solid %8; background: transparent; } "
                      "QCheckBox::indicator:checked { background-color: %1; border-color: %1; image: url(%9); } "
                      "QCheckBox::indicator:unchecked:hover { border-color: %1; } "

                      // Inputs (LineEdit, SpinBox, ComboBox)
                      "QLineEdit, QSpinBox, QComboBox { border: 1px solid %8; border-radius: 6px; padding: 6px; background-color: %10; selection-background-color: %1; selection-color: %2; } "
                      "QLineEdit:focus, QSpinBox:focus, QComboBox:focus { border: 1px solid %1; } "

                      // SpinBox Buttons
                      "QSpinBox::up-button { subcontrol-origin: border; subcontrol-position: top right; width: 20px; border-left-width: 1px; border-left-color: %8; border-top-right-radius: 6px; background: %1; } "
                      "QSpinBox::down-button { subcontrol-origin: border; subcontrol-position: bottom right; width: 20px; border-left-width: 1px; border-left-color: %8; border-bottom-right-radius: 6px; background: %1; } "
                      "QSpinBox::up-button:hover, QSpinBox::down-button:hover { background: %4; } "
                      "QSpinBox::up-button:pressed, QSpinBox::down-button:pressed { background: %5; } "
                      "QSpinBox::up-arrow { image: url(%12); width: 8px; height: 8px; } "
                      "QSpinBox::down-arrow { image: url(%13); width: 8px; height: 8px; } "

                      "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: top right; width: 20px; border-left-width: 0px; border-top-right-radius: 6px; border-bottom-right-radius: 6px; } "

                      // GroupBox
                      "QGroupBox { border: 1px solid %8; border-radius: 8px; margin-top: 24px; padding-top: 10px; } "
                      "QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top left; left: 12px; padding: 0 5px; font-weight: bold; color: %1; } "

                      // Tab Widget
                      "QTabWidget::pane { border: 1px solid %8; border-radius: 6px; top: -1px; } "
                      "QTabBar::tab { background: %10; border: 1px solid %8; padding: 8px 16px; margin-right: 4px; border-top-left-radius: 6px; border-top-right-radius: 6px; } "
                      "QTabBar::tab:selected { background: %1; color: %2; border-color: %1; } "
                      "QTabBar::tab:!selected:hover { background: %11; } "

                      // Tables & Lists
                      "QTableWidget, QTableView, QListWidget { border: 1px solid %8; border-radius: 6px; gridline-color: %8; selection-background-color: %1; selection-color: %2; } "
                      "QHeaderView::section { background-color: %10; padding: 6px; border: none; border-bottom: 2px solid %1; font-weight: bold; } "
                      "QTableCornerButton::section { background-color: %10; border: none; } "

                      // Scrollbars (Minimalist)
                      "QScrollBar:vertical { border: none; background: %10; width: 10px; margin: 0px; } "
                      "QScrollBar::handle:vertical { background: %8; min-height: 20px; border-radius: 5px; } "
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

int main(int argc, char *argv[])
{
    // Initialize Application
    QApplication a(argc, argv);
    a.setApplicationName("AcademicManager");
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

    int exitCode = 0;

    do
    {
        QString role = "";
        int userId = -1;
        QString name = "";

        // Scope the authManager so it closes DB connection before MainWindow opens
        {
            AcademicManager authManager;

            // Custom Login Dialog
            QDialog loginDialog;
            loginDialog.setWindowTitle("Login");
            loginDialog.setModal(true);
            loginDialog.resize(350, 180);

            QVBoxLayout *layout = new QVBoxLayout(&loginDialog);

            // Top Layout for Theme Button
            QHBoxLayout *topLayout = new QHBoxLayout();
            topLayout->addStretch();
            QPushButton *themeBtn = new QPushButton("Theme", &loginDialog);
            themeBtn->setToolTip("Current Theme: " + themes[currentThemeIdx].name);
            topLayout->addWidget(themeBtn);
            layout->addLayout(topLayout);

            QObject::connect(themeBtn, &QPushButton::clicked, [&]()
                             {
                currentThemeIdx = (currentThemeIdx + 1) % themes.size();
                applyTheme(a, themes[currentThemeIdx]);
                themeBtn->setToolTip("Current Theme: " + themes[currentThemeIdx].name); });

            QFormLayout *formLayout = new QFormLayout();

            QLineEdit *userEdit = new QLineEdit(&loginDialog);
            userEdit->setPlaceholderText("Username");
            QLineEdit *passEdit = new QLineEdit(&loginDialog);
            passEdit->setPlaceholderText("Password");
            passEdit->setEchoMode(QLineEdit::Password);

            formLayout->addRow("Username:", userEdit);
            formLayout->addRow("Password:", passEdit);
            layout->addLayout(formLayout);

            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &loginDialog);
            layout->addWidget(buttonBox);

            QObject::connect(buttonBox, &QDialogButtonBox::rejected, &loginDialog, &QDialog::reject);
            QObject::connect(buttonBox, &QDialogButtonBox::accepted, [&]()
                             {
                name = userEdit->text();
                QString pass = passEdit->text();
                role = authManager.login(name, pass, userId);

                if (!role.isEmpty()) {
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