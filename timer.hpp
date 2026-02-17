#ifndef TIMER_HPP
#define TIMER_HPP

#include <QObject>
#include <QTimer>

/**
 * @brief A high-precision timer for tracking duration habits.
 *
 * Emits signals for UI updates and completion.
 */
class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(QObject *parent = nullptr);

    /**
     * @brief Starts the timer for a specific duration.
     * @param minutes Duration in minutes.
     */
    void start(int minutes);

    /**
     * @brief Pauses or resumes the timer.
     */
    void pause();

    /**
     * @brief Stops and resets the timer.
     */
    void stop();

signals:
    void timeUpdated(QString time, float progress); ///< Emitted every tick with formatted time and progress (0.0-1.0).
    void finished();                                ///< Emitted when timer reaches zero.

private slots:
    void onTimeout();

private:
    QTimer *internalTimer;
    int remainingTime; ///< Remaining time in milliseconds.
    int totalTime;     ///< Total duration in milliseconds.
    bool isPaused;
};
#endif // TIMER_HPP