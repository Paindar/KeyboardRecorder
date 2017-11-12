#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSystemTrayIcon>
#include "keyboardthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void stopThread();
    void fresh();
    void onActivatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

public:
    explicit MainWindow(QWidget *parent = 0);
    void flushTable();
    ~MainWindow();

protected:
    void changeEvent(QEvent * event) override;
private:
    Ui::MainWindow *ui;
    QStandardItemModel* model = new QStandardItemModel();
    QSystemTrayIcon* mSysTrayIcon;
};

#endif // MAINWINDOW_H
