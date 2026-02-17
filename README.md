# Acadence

## Short Description
Acadence is a GUI-based personal academic assistant developed in C++ using the Qt framework. It demonstrates core Object-Oriented Programming (OOP) principles by helping students and teachers organize their academic lives.
*   **Students** can track routines, view upcoming classes, manage a study planner, track habits (with timers), and view grades/attendance.
*   **Teachers** can manage course routines, create assessments, grade students, take attendance, and answer queries.
*   **Admins** have full control to manage users, courses, and system data via a dedicated panel.

The application uses persistent CSV storage for all data and features a role-based login system.

## How to Compile and Run

### Prerequisites
*   C++ Compiler (supporting C++17 or later)
*   CMake (Version 3.16+)
*   Qt 6 Development Libraries (Widgets module)

### Build Instructions
1.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
2.  Run CMake to configure the project:
    ```bash
    cmake ..
    ```
3.  Compile the application:
    ```bash
    make
    ```
    *(On Windows with MSVC, use `nmake` or open the generated solution in Visual Studio)*

4.  Run the executable:
    ```bash
    ./Acadence
    ```

## Sample Input Files
The application automatically generates necessary CSV files if they are missing. Data is stored in the same directory as the executable (or the working directory).

**Key Data Files:**
*   `admins.csv`: Stores admin credentials.
*   `students.csv`: Student profiles (ID, Name, Credentials, Dept, etc.).
*   `teachers.csv`: Teacher profiles (ID, Name, Credentials, Dept, Salary).
*   `courses.csv`: Course details.
*   `routine.csv`: Weekly class schedules.
*   `habits.csv`, `tasks.csv`, `grades.csv`, `attendance.csv`, `notices.csv`: User-specific data.

## Class Descriptions

### Core Logic
*   **`Person` (Abstract Base Class)**: Defines common attributes for all users (ID, name, email, username, password).
*   **`Student`**: Inherits `Person`. Adds attributes for department, batch, semester, admission date, and GPA.
*   **`Teacher`**: Inherits `Person`. Adds attributes for department, designation, and salary.
*   **`Admin`**: Inherits `Person`. Represents system administrators.
*   **`AcadenceManager`**: The "Controller" class. Handles all file I/O (CSV reading/writing), authentication logic, and data retrieval/updates for the UI.
*   **`Course`**: Represents an academic subject with code, name, credits, and assigned teacher.
*   **`RoutineSession` & `WeeklyRoutine`**: Encapsulates schedule data. `WeeklyRoutine` manages a collection of `RoutineSession` objects.

### Habits & Utilities
*   **`Habit` (Abstract Base Class)**: Defines the interface for habit tracking (streaks, completion status).
*   **`DurationHabit`**: Inherits `Habit`. Tracks time-based activities (e.g., "Study for 30 mins").
*   **`CountHabit`**: Inherits `Habit`. Tracks quantity-based activities (e.g., "Drink 8 glasses of water").
*   **`Timer`**: A helper class using `QTimer` to provide stopwatch functionality for `DurationHabit`.

### User Interface
*   **`MainWindow`**: The main GUI class inheriting `QMainWindow`. Manages all UI interactions, view switching based on user roles, and connects UI events to `AcadenceManager`.
*   **`CsvDelegate`**: Inherits `QStyledItemDelegate`. Provides custom input validation (spinboxes, date pickers, duplicate checks) for the Admin table view.

## Relationships & OOP Concepts

### Inheritance
*   **`Person` -> `Student`, `Teacher`, `Admin`**: Used to share common user identity logic while allowing specific roles to have unique attributes (e.g., Salary for Teachers, GPA for Students).
*   **`Habit` -> `DurationHabit`, `CountHabit`**: Used to implement different tracking mechanisms (Time vs. Count) under a unified interface.

### Polymorphism
*   **Virtual Functions**: `Person::getRole()` allows the system to identify user types dynamically. `Habit::getProgressString()` and `serializeValue()` allow the UI and storage system to treat all habits uniformly while executing subclass-specific logic.
*   **Pointers**: The application uses `QVector<Habit*>` to store mixed habit types and `Person*` for generic user handling.

### Encapsulation
*   All classes use `private` sections for data members (e.g., `id`, `password`, `salary`) and provide `public` getters/setters.
*   `AcadenceManager` hides the complexity of CSV parsing and file management from the `MainWindow`.

## Exception Handling & Error Checking
*   **File I/O**: The `AcadenceManager` checks if files open successfully. If a file cannot be opened, functions return empty containers or default values to prevent crashes.
*   **Input Validation**:
    *   The `CsvDelegate` class intercepts user input in the Admin panel to ensure data integrity (e.g., unique IDs, unique usernames, valid password characters).
    *   `QMessageBox` is used to alert users of invalid inputs (e.g., empty fields, duplicate entries).
*   **Dynamic Casts**: `dynamic_cast` is used safely to check habit types (`DurationHabit` vs `CountHabit`) before performing type-specific actions like starting a timer.

## File Structure
*   **Headers (`.hpp`)**: Contain class declarations and include guards.
*   **Sources (`.cpp`)**: Contain class definitions and implementation logic.
*   **One class per file pair**: Generally followed (e.g., `student.hpp`/`student.cpp`), with small helper structs/classes grouped logically (e.g., `RoutineSession` in `routine.hpp`).

## Naming & Conventions
*   **Classes**: PascalCase (e.g., `AcadenceManager`, `WeeklyRoutine`).
*   **Variables/Functions**: camelCase (e.g., `getStudent`, `currentHabitList`).
*   **Indentation**: 4 spaces.
*   **Const Correctness**: Getters and non-modifying methods are marked `const`.

## Known Issues
*   **Memory Management**: The application uses raw pointers for some polymorphic objects (e.g., `Student*` returned from `AcadenceManager`). While `MainWindow` handles deletion immediately after use in most cases, smart pointers (`std::unique_ptr` or `QSharedPointer`) would be a safer modern C++ improvement.
*   **CSV Parsing**: Basic CSV parsing is implemented. It handles quoted fields but might be sensitive to malformed external edits.