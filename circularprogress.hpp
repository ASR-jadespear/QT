#ifndef CIRCULARPROGRESS_HPP
#define CIRCULARPROGRESS_HPP

#include <QWidget>
#include <QColor>

class CircularProgress : public QWidget
{
    Q_OBJECT
public:
    explicit CircularProgress(QWidget *parent = nullptr);

    void setProgress(float value); // 0.0 to 1.0
    void setTimeText(const QString &text);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float m_progress;
    QString m_text;
};

#endif // CIRCULARPROGRESS_HPP