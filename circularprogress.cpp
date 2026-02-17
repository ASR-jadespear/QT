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

void CircularProgress::setTimeText(const QString &text)
{
    m_text = text;
    update();
}

void CircularProgress::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int side = qMin(width(), height());
    QRectF rect(15, 15, side - 30, side - 30); // Margin

    QColor ringColor = palette().highlight().color(); // Use theme secondary color
    QColor textColor = palette().text().color();      // Use theme text color

    // Draw Background Track
    QPen trackPen(ringColor.lighter(160), 6, Qt::SolidLine, Qt::RoundCap);
    if (ringColor.value() > 200)
        trackPen.setColor(ringColor.darker(120)); // Adjust for light themes
    p.setPen(trackPen);
    p.drawEllipse(rect);

    // Draw Progress Arc with Gradient
    QConicalGradient gradient(rect.center(), 90);
    gradient.setColorAt(0, ringColor);
    gradient.setColorAt(1, ringColor.lighter(140));

    QPen progressPen;
    progressPen.setWidth(12);
    progressPen.setCapStyle(Qt::RoundCap);
    progressPen.setBrush(gradient);
    p.setPen(progressPen);

    int startAngle = 90 * 16;
    int spanAngle = -m_progress * 360 * 16;
    p.drawArc(rect, startAngle, spanAngle);

    // Draw Text
    p.setPen(textColor);
    QFont f = font();
    f.setPixelSize(side / 5);
    f.setBold(true);
    f.setFamily("monospace");
    p.setFont(f);

    p.drawText(rect, Qt::AlignCenter, m_text);
}