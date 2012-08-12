#include "widget.h"
#include "ui_widget.h"
#include "iostream"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //设置标签字体颜色、居中
    QPalette paletteWhite;
    paletteWhite.setColor(QPalette::WindowText, Qt::white);
    ui->modelLabel->setPalette(paletteWhite);
    ui->tipLabel->setPalette(paletteWhite);
    QPalette paletteGray;
    paletteGray.setColor(QPalette::WindowText, Qt::gray);
    ui->timeLabel->setPalette(paletteGray);
    ui->nextLabel->setPalette(paletteGray);
    ui->modelLabel->setAlignment(Qt::AlignCenter);
    ui->timeLabel->setAlignment(Qt::AlignRight);
    ui->nextLabel->setAlignment(Qt::AlignLeft);
    ui->tipLabel->setAlignment(Qt::AlignRight);
    //

    QFile file(":/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint
                       | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
                       | Qt::WindowMaximizeButtonHint | Qt::WindowStaysOnTopHint);
    this->setWindowOpacity(0.8); //半透明
    //this->setStyleSheet(stylesheet); //载入StyleSheet

    //状态栏设置
    tary = new QSystemTrayIcon(this);
    tary->setIcon(QIcon(":/bahrsi.png"));
    aboutAction = new QAction(tr("About BahRSI"), this);
    quitAction = new QAction(tr("Quit BahRSI"), this);
    connect(this->aboutAction, SIGNAL(triggered()), this, SLOT(aboutBahRSI()));
    connect(this->quitAction, SIGNAL(triggered()), this, SLOT(close()));
    menu = new QMenu;
    menu->addAction(this->aboutAction);
    menu->addSeparator();
    menu->addAction(this->quitAction);
    tary->setContextMenu(menu);
    tary->show();
    //

    ui->progressBar->setTextVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton->setText(tr("Postpone"));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(postPone()));

    secondTimer = new QTimer(this);
    connect(this->secondTimer, SIGNAL(timeout()), this, SLOT(updateUI()));

    misecondTimer = new QTimer(this);
    connect(this->misecondTimer, SIGNAL(timeout()), this, SLOT(stepOne()));

    microPauseWorkTimer = new QTimer(this);
    connect(this->microPauseWorkTimer, SIGNAL(timeout()), this, SLOT(microPauseRest()));
    this->microPauseWorkTimer->start(1000*60*4);

    microPauseRestTimer = new QTimer(this);
    connect(this->microPauseRestTimer, SIGNAL(timeout()), this, SLOT(microPauseWork()));
    this->setMouseTracking(true);

    workBreakWorkTimer = new QTimer(this);
    connect(this->workBreakWorkTimer, SIGNAL(timeout()), this, SLOT(workBreakRest()));
    this->workBreakWorkTimer->start(1000*60*50);

    workBreakRestTimer = new QTimer(this);
    connect(this->workBreakRestTimer, SIGNAL(timeout()), this, SLOT(workBreakWork()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setBrush(QColor("100,100,100,100"));
    p.setPen(QColor("100,100,100,100"));
    p.drawRoundedRect(0, 0, width() - 1, height() - 1, 20, 20);
}

void Widget::stepOne()
{
    ui->progressBar->setValue(ui->progressBar->value() + 1);
}

void Widget::updateUI()
{
    QTime t(QTime::fromString(ui->timeLabel->text()));
    ui->timeLabel->setText(t.addSecs(-1).toString("hh:mm:ss"));
}

void Widget::microPauseRest()
{
    if (workBreakRestTimer->isActive()){
        this->microPauseWorkTimer->start(1000*60*4);
    }
    else{
        this->microPauseWorkTimer->stop();
        QTime n(0,0,13);
        ui->progressBar->setRange(0,130);
        ui->progressBar->setValue(1);
        ui->timeLabel->setText(n.toString("hh:mm:ss"));
        ui->modelLabel->setText(tr("Micro Pause"));
        ui->tipLabel->setText(tr("Close your eyes take a deep breath"));
        this->show();
        this->secondTimer->start(1000);
        this->misecondTimer->start(100);
        this->microPauseRestTimer->start(1000*13);
    }
}
void Widget::microPauseWork()
{
    this->microPauseRestTimer->stop();
    this->secondTimer->stop();
    this->misecondTimer->stop();
    this->hide();
    this->microPauseWorkTimer->start(1000*60*4);
}

void Widget::workBreakRest()
{
    this->workBreakWorkTimer->stop();
    QTime n(0,8,0);
    ui->progressBar->setRange(0,4800);
    ui->progressBar->setValue(1);
    ui->timeLabel->setText(n.toString("hh:mm:ss"));
    ui->modelLabel->setText(tr("Work Break"));
    ui->tipLabel->setText(tr("Why not have a drink"));
    ui->pushButton->setVisible(true);
    this->show();
    this->secondTimer->start(1000);
    this->misecondTimer->start(100);
    this->workBreakRestTimer->start(1000*60*8);
}

void Widget::workBreakWork()
{
    this->workBreakRestTimer->stop();
    this->secondTimer->stop();
    this->misecondTimer->stop();
    this->hide();
    this->workBreakWorkTimer->start(1000*60*50);
}

void Widget::mouseMoveEvent(QMouseEvent *)
{
    if(this->microPauseRestTimer->isActive()){
        this->microPauseRest();
    }
    else{
        if(workBreakRestTimer->isActive()){
            this->workBreakRest();
        }
    }
}

void Widget::keyPressEvent(QKeyEvent *)
{
    if(this->microPauseRestTimer->isActive()){
        this->microPauseRest();
    }
    else{
        this->workBreakRest();
    }
}
void Widget::aboutBahRSI()
{
    qDebug("TODO About BahRSI");
}

void Widget::postPone()
{
    this->workBreakWork();
}
