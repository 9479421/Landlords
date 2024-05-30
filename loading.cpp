#include "loading.h"

#include "gamepanel.h"

#include <QPainter>
#include <QTimer>

Loading::Loading(QWidget *parent)
    : QWidget{parent}
{
    m_bk.load(":/images/loading.png");
    setFixedSize(m_bk.size());

    // 去边框
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // 背景透明
    setAttribute(Qt::WA_TranslucentBackground);

    QPixmap pixmap(":/images/progress.png");
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,[=](){
        m_progress = pixmap.copy(0,0,m_dist,pixmap.height());
        update();
        if(m_dist >= pixmap.width()){
            timer->stop();
            timer->deleteLater();
            GamePanel* panel = new GamePanel;
            panel->show();
            close();
        }
        m_dist+=5;
    });
    timer->start(15);
}

void Loading::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawPixmap(rect(),m_bk);
    p.drawPixmap(62,417,m_progress.width(),m_progress.height(),m_progress);
}
