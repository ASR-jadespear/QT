#include "habit.hpp"

Habit::Habit(int id, int sid, QString n, HabitType t, Frequency f)
    : id(id), studentId(sid), name(n), type(t), frequency(f), streak(0), lastUpdated(QDate::currentDate()), isCompleted(false) {}

bool Habit::checkReset()
{
    QDate today = QDate::currentDate();
    bool reset = false;

    if (frequency == Frequency::DAILY)
    {
        if (lastUpdated != today)
            reset = true;
    }
    else if (frequency == Frequency::WEEKLY)
    {
        if (lastUpdated.weekNumber() != today.weekNumber() || lastUpdated.year() != today.year())
            reset = true;
    }

    if (reset)
    {
        // If we missed the previous period and it wasn't completed, reset streak
        // Logic: If daily, and lastUpdated was yesterday and completed, keep streak.
        // If lastUpdated was > 1 day ago, reset streak.
        // Simplified logic from prompt:
        if (!isCompleted && streak > 0)
            streak = 0;

        isCompleted = false;
        lastUpdated = today;
        return true;
    }
    return false;
}

void Habit::markComplete()
{
    if (!isCompleted)
    {
        isCompleted = true;
        streak++;
    }
}

QString Habit::getFrequencyString() const { return (frequency == Frequency::DAILY) ? "Daily" : "Weekly"; }
QString Habit::getTypeString() const
{
    switch (type)
    {
    case HabitType::DURATION:
        return "Duration";
    case HabitType::COUNT:
        return "Count";
    default:
        return "Unknown";
    }
}

// --- Duration Habit ---
DurationHabit::DurationHabit(int id, int sid, QString n, Frequency f, int target)
    : Habit(id, sid, n, HabitType::DURATION, f), targetMinutes(target), currentMinutes(0) {}

QString DurationHabit::getProgressString() const
{
    return QString("%1/%2 mins").arg(currentMinutes).arg(targetMinutes);
}

QString DurationHabit::serializeValue() const
{
    return QString::number(currentMinutes);
}

void DurationHabit::deserializeValue(const QString &val)
{
    currentMinutes = val.toInt();
}

// --- Count Habit ---
CountHabit::CountHabit(int id, int sid, QString n, Frequency f, int target, QString u)
    : Habit(id, sid, n, HabitType::COUNT, f), targetCount(target), currentCount(0), unit(u) {}

QString CountHabit::getProgressString() const
{
    return QString("%1/%2 %3").arg(currentCount).arg(targetCount).arg(unit);
}

QString CountHabit::serializeValue() const
{
    return QString::number(currentCount);
}

void CountHabit::deserializeValue(const QString &val)
{
    currentCount = val.toInt();
}