cmake_minimum_required(VERSION 3.16)

project(MyFirstQtApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# This is the "magic" that handles Qt's special C++ code generation
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the Qt 6 libraries on your Fedora system
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

add_executable(MyFirstQtApp WIN32 MACOSX_BUNDLE main.cpp mainwindow.cpp mainwindow.hpp mainwindow.ui person.hpp student.hpp student.cpp teacher.hpp teacher.cpp course.hpp course.cpp academicmanager.hpp academicmanager.cpp timer.hpp timer.cpp habit.hpp habit.cpp)

# Link the Widgets module to your app
target_link_libraries(MyFirstQtApp PRIVATE Qt6::Widgets Qt6::Sql)

# Force CMake re-configuration to clear stale MOC files