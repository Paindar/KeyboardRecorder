#ifndef KEYBOARDTHREAD_H
#define KEYBOARDTHREAD_H
#include <QThread>
#include "windows.h"

class KeyboardThread : public QThread
{
    Q_OBJECT
signals:
    void fresh();
private:
    DWORD id;
protected:
     void run();
public:
    KeyboardThread(QObject *parent=0);
    void tryStop();
    void emitSignal();
    ~KeyboardThread();
};

#endif // KEYBOARDTHREAD_H
