# Academic Manager Project

## 1. Introduction
Something that has brought all of us here, yet hardly anyone likes to do is Academic Studies. Managing academic responsibilities is increasingly difficult for both students and teachers—classes shift, deadlines stack up, routines overlap, and communication gets lost. Maintaining stuff across Whatsapp, Messenger, Google Classroom and notes-taking apps is as easy as looking for a needle in a haystack. A lightweight assistant can help organize these tasks without the complexity of multiple apps.
Our project targets IUT students and teachers who need a simple but structured way to track schedules, reminders, academic progress, and classroom notices.

**Why it matters:** It centralizes routine academic tasks into one tool and demonstrates strong practical application of OOP principles.

## 2. Proposal
We propose to build a terminal-based personal academic assistant developed entirely in C++ using Object Oriented Programming principles.
*   Students can track routines, deadlines, study plans, habits, and receive notifications.
*   Teachers can manage class notices, quiz schedules, class rescheduling, attendance, and grading.

The program will load/store user data from text files and simulate notifications based on system time. The final goal is a fully functioning prototype demonstrating our understanding of object-oriented design, data abstraction, and clean code structure.

## 3. Description (Usage Story)
A student, Airah, launches the program and logs in. The assistant immediately shows her Today’s Schedule, along with the next class (starting in 30 mins but she feels sleepy) and approaching deadlines of 3 quizzes. The student checks their study planner, marks a completed task, and receives a notification in the evening that her daily goal of studying remains unfulfilled. So she finishes it and updates the system.

Later, a teacher, Rafid, logs in from his account. He answers some student queries, enters a quiz date with syllabus, and publishes a class cancellation notice for tomorrow’s class. He also updated attendance records for today’s class and entered marks for a previous quiz.

Later, upon entering the app another student, Abeed, automatically receives the class cancellation notice but his joy is short-lived for the next thing he sees is his poor grades in quiz and attendance below 85%.

Currently all app actions occur through a clean terminal menu. Data is stored as simple text files, and internal classes handle schedules, reminders, grades, and notifications in a structured OOP approach.

## 4. Features

### MVP (Minimum Viable Product)
*   User Login System (student/teacher profiles).
*   Student Routine Loader (read routine from text; show next-day classes).
*   Student Reminders (quiz, assignment, study tasks).
*   Study Planner (add/edit tasks, mark completed, show remaining goals).
*   Habit Tracker (sleep, hydration, workout logs).
*   Teacher Notices (publish/update public class notices).
*   Teacher Basic Quiz & Deadline Scheduling.
*   Grading System: add marks, show student stats (highest/lowest/average).
*   Attendance Tracking: per student with 85% warning.
*   In-App Notification System using timestamps.
*   Persistent Storage using text-based files for all profiles and data.

### Stretch Features (Optional After MVP)
*   Room Booking System for teachers.
*   Class Slot Availability Viewer.
*   Student–Teacher Query Panel (questions & replies).
*   Peer “Buddy Finder” based on interest matching.
*   Weekly Syllabus Progress Tracker with reminders.
*   Deadline Conflict Detector (alerts when a student is overloaded).
*   “Simulated Time” mode for easier demonstration of reminder behavior.