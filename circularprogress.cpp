/**
 * @file circularprogress.cpp
 * @brief Implementation of the custom CircularProgress widget.
 *
 * Draws a circular progress bar using QPainter with antialiasing.
 */

#include "circularprogress.hpp"
#include <QPainter>
#include <QPainterPath>

CircularProgress::CircularProgress(QWidget *parent) : QWidget(parent), m_progress(0.0)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(200, 200);
}

void CircularProgress::setProgress(float value)
{
    m_progress = value;
    update();
}

//! Sets the text displayed in the center of the ring
void CircularProgress::setTimeText(const QString &text)
{
    m_text = text;
    update();
}

void CircularProgress::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Calculate the size of the circle based on the widget dimensions
    int side = qMin(width(), height());
    QRectF rect(15, 15, side - 30, side - 30); // Apply a margin to avoid clipping

    // Fetch colors from the current application palette (Theme aware)
    QColor ringColor = palette().highlight().color(); // Use theme secondary color
    QColor textColor = palette().text().color();      // Use theme text color

    // --- Draw Background Track ---
    // Create a lighter version of the accent color for the empty track
    QPen trackPen(ringColor.lighter(160), 6, Qt::SolidLine, Qt::RoundCap);
    if (ringColor.value() > 200)
        trackPen.setColor(ringColor.darker(120)); // If accent is already light, darken the track

    painter.setPen(trackPen);
    painter.drawEllipse(rect);

    // --- Draw Progress Arc ---
    QPen progressPen;
    progressPen.setWidth(12);
    progressPen.setCapStyle(Qt::RoundCap);
    progressPen.setColor(ringColor);
    painter.setPen(progressPen);

    // QPainter uses 1/16th of a degree units.
    // Start at 90 degrees (12 o'clock).
    int startAngle = 90 * 16;
    int spanAngle = -m_progress * 360 * 16; // Negative for clockwise direction
    painter.drawArc(rect, startAngle, spanAngle);

    // --- Draw Center Text ---
    painter.setPen(textColor);
    QFont f = font();
    f.setPixelSize(side / 5); // Dynamic font size based on widget size
    f.setBold(true);
    f.setFamily("monospace");
    painter.setFont(f);

    painter.drawText(rect, Qt::AlignCenter, m_text);
}