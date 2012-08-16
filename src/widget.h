#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QTimer>
#include <QPaintEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    Ui::Widget *ui;
    QTimer *secondTimer; // 每秒更新一次
    QTimer *misecondTimer; // 每毫秒更新一次
    QTimer *microPauseWorkTimer; // Micro Pause 模式时的工作时间计算
    QTimer *microPauseRestTimer; // Micro Pause 模式时的休息时间计算
    QTimer *workBreakWorkTimer; // Work Break 模式时的工作时间计算
    QTimer *workBreakRestTimer; // Work Break 模式时的休息时间计算
    QSystemTrayIcon *tary;
    QAction *quitAction;
    QAction *aboutAction;
    QMenu *menu;
    int mousePosX;
    int mousePosY;
    QCursor *wcursor;
    int workBreakWorkTime, workBreakRestTime; // WorkBreak 模式下的工作、休息时间 单位分钟
    int microPauseWorkTime, microPauseRestTime; // MicroPause 模式下的工作、休息时间 单位秒

private slots:
    void updateUI();
    void stepOne();
    void microPauseWork(); // Micro Pause 工作状态
    void microPauseRest(); // Micro Pause 休息状态
    void workBreakWork(); // Work Break 工作状态
    void workBreakRest(); // Work Break 休息状态
    void aboutBahRSI();
    void quitBahRSI();
    void postPone();
    void isMouseMoving(); // 判断鼠标是否移动
    void readSettings();
    void writeSettings();
};

#endif // WIDGET_H
