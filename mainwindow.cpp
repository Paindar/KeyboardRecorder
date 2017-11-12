#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "globals.h"
#include <vector>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model->setColumnCount(2);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("Key"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("Time"));
    ui->tableView->horizontalHeader()->setDefaultSectionSize(260);
    //model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("Code"));
    ui->tableView->setModel(model);

    mSysTrayIcon = new QSystemTrayIcon(this);
    QIcon icon = QIcon(":/ico/ico.ico");
    mSysTrayIcon->setIcon(icon);
    mSysTrayIcon->setToolTip("显示程序");
    connect(mSysTrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(onActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    mSysTrayIcon->show();
}


MainWindow::~MainWindow()
{
    delete ui;
    saveStorage();
}

void MainWindow::stopThread()
{
    kThread->tryStop();
}

void MainWindow::fresh()
{
    flushTable();
}

void MainWindow::onActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger:
            break;
        case QSystemTrayIcon::DoubleClick:
            if(!this->isHidden())
            {
                this->hide();
            }
            else
            {
                if (this->isMinimized())
                {
                    this->showNormal();
                }
                ::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
                ::SetWindowPos(HWND(this->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
                this->show();
                this->activateWindow();
                //设置窗口置顶
            }

            break;
        default:
            break;
    }
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange)
        return;
    if(this->windowState()==Qt::WindowMinimized)
    {
        this->hide();
    }
}


void MainWindow::flushTable()
{
    int i=0;
    rwLock.tryLockForRead();
    vector<pair<int,long>> sortArr(record.begin(),record.end());
    sort(sortArr.begin(),sortArr.end(),[](pair<int,long> a,pair<int,long> b){
        return a.second>b.second;
    });

    for(auto iter:sortArr)
    {
        model->setItem(i,0,new QStandardItem(key_name[iter.first]));
        //model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));
        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        QStandardItem *item = new QStandardItem;
        item->setData(QVariant(iter.second),Qt::EditRole);
        item->setTextAlignment(Qt::AlignCenter);
        model->setItem(i,1,item);
        //model->setItem(i,2,new QStandardItem(QString::number(iter.first)));
        i++;
    }
    rwLock.unlock();
}
