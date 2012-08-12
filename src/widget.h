#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QTimer>
#include <QPaintEvent>
#include <QMouseEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    Ui::Widget *ui;
    QTimer *secondTimer;
    QTimer *misecondTimer;
    QTimer *microPauseWorkTimer;
    QTimer *microPauseRestTimer;
    QTimer *workBreakWorkTimer;
    QTimer *workBreakRestTimer;
    QSystemTrayIcon *tary;
    QAction *quitAction;
    QAction *aboutAction;
    QMenu *menu;

private slots:
    void updateUI();
    void stepOne();
    void microPauseWork();
    void microPauseRest();
    void workBreakWork();
    void workBreakRest();
    void aboutBahRSI();
    void postPone();
};

#endif // WIDGET_H
