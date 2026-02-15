#ifndef HABIT_H
#define HABIT_H

#include <QString>
#include <QDate>
#include <QStringList>

/**
 * @brief Defines the type of habit for tracking logic.
 */
enum class HabitType
{
    DURATION, ///< Time-based habit (e.g., Workout).
    COUNT     ///< Quantity-based habit (e.g., Water intake).
};

/**
 * @brief Defines the frequency of the habit reset.
 */
enum class Frequency
{
    DAILY, ///< Resets every day.
    WEEKLY ///< Resets every week.
};

/**
 * @brief Abstract base class for a user habit.
 *
 * Handles common logic for streaks, completion status, and serialization.
 */
class Habit
{
public:
    int id;              ///< Database ID.
    int studentId;       ///< Owner ID.
    QString name;        ///< Habit Name.
    HabitType type;      ///< Type of habit.
    Frequency frequency; ///< Reset frequency.
    int streak;          ///< Current streak count.
    QDate lastUpdated;   ///< Last date the habit was updated.
    bool isCompleted;    ///< Completion status for current period.

    Habit(int id, int sid, QString n, HabitType t, Frequency f);
    virtual ~Habit() = default;

    /**
     * @brief Gets a formatted string representing current progress.
     */
    virtual QString getProgressString() const = 0;

    /**
     * @brief Serializes subclass-specific data for database storage.
     */
    virtual QString serializeValue() const = 0;

    /**
     * @brief Deserializes subclass-specific data from database storage.
     */
    virtual void deserializeValue(const QString &val) = 0;

    /**
     * @brief Checks if the habit needs to be reset based on frequency.
     * @return True if a reset occurred, false otherwise.
     */
    bool checkReset();

    /**
     * @brief Marks the habit as complete and increments streak.
     */
    void markComplete();
    QString getFrequencyString() const;
    QString getTypeString() const;
};

/**
 * @brief A habit tracked by duration (minutes).
 */
class DurationHabit : public Habit
{
public:
    int targetMinutes;
    int currentMinutes;

    DurationHabit(int id, int sid, QString n, Frequency f, int target);
    QString getProgressString() const override;
    QString serializeValue() const override;
    void deserializeValue(const QString &val) override;
};

/**
 * @brief A habit tracked by count (units).
 */
class CountHabit : public Habit
{
public:
    int targetCount;
    int currentCount;
    QString unit;

    CountHabit(int id, int sid, QString n, Frequency f, int target, QString u);
    QString getProgressString() const override;
    QString serializeValue() const override;
    void deserializeValue(const QString &val) override;
};

#endif // HABIT_H