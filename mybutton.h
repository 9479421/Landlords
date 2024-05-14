#ifndef MYBUTTON_H
#define MYBUTTON_H

#include<QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = nullptr);

    void setImage(QString normal,QString hover,QString pressed);


signals:

protected:
    // 鼠标按下
    void MousePressEvent(QMouseEvent *ev);
    // 鼠标释放
    void MouseReleaseEvent(QMouseEvent* ev);
    // 鼠标进入
    void enterEvent(QEnterEvent* ev);
    // 鼠标离开
    void leaveEvent(QEvent* ev);
    // 绘图
    void paintEvent(QPaintEvent* ev);


private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QPixmap m_pixmap;
};

#endif // MYBUTTON_H
