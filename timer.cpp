#include "timer.hpp"

Timer::Timer(QObject *parent) : QObject(parent), remainingTime(0), isPaused(false)
{
    internalTimer = new QTimer(this);
    connect(internalTimer, &QTimer::timeout, this, &Timer::onTimeout);
}

void Timer::start(int minutes)
{
    if (minutes <= 0)
        return;
    remainingTime = minutes * 60 * 1000; // Convert to milliseconds
    totalTime = remainingTime;
    isPaused = false;
    internalTimer->start(10); // 10ms interval for smooth updates
    onTimeout();              // Update display immediately
}

void Timer::pause()
{
    if (internalTimer->isActive())
    {
        internalTimer->stop();
        isPaused = true;
    }
    else if (isPaused && remainingTime > 0)
    {
        internalTimer->start(10);
        isPaused = false;
    }
}

void Timer::stop()
{
    internalTimer->stop();
    isPaused = false;
    remainingTime = 0;
    emit timeUpdated("00:00", 0.0f);
}

void Timer::onTimeout()
{
    int min = remainingTime / 60000;
    int sec = (remainingTime % 60000) / 1000;

    QString timeStr = QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));

    float progress = (totalTime > 0) ? (float)remainingTime / totalTime : 0.0f;
    emit timeUpdated(timeStr, progress);

    if (remainingTime <= 0)
    {
        internalTimer->stop();
        emit finished();
    }
    else
    {
        remainingTime -= 10;
    }
}