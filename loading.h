#ifndef LOADING_H
#define LOADING_H

#include <QWidget>

class Loading : public QWidget
{
    Q_OBJECT
public:
    explicit Loading(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);

private:
    QPixmap m_bk;
    QPixmap m_progress;
    int m_dist = 15;
signals:
};

#endif // LOADING_H
