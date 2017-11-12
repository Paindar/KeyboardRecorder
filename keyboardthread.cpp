#include "keyboardthread.h"
#include "globals.h"

void KeyboardThread::run()
{

    g_kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL, //Installs a hook procedure that monitors low-level keyboard input events
            (HOOKPROC)&kb_proc, //A hook procedure that monitors low-level keyboard input events
            NULL,//A handle to the DLL containing the hook procedure pointed to by the lpfn parameter. The hMod parameter must be set to NULL if the dwThreadId parameter specifies a thread created by the current process and if the hook procedure is within the code associated with the current process.(GetModuleHandle(NULL))
            0);//The identifier of the thread with which the hook procedure is to be associated. For desktop apps, if this parameter is zero, the hook procedure is associated with all existing threads running in the same desktop as the calling thread. For Windows Store apps, see the Remarks section.
    if (g_kb_hook == NULL) {
        printf("SetWindowsHookEx failed, %ld\n", GetLastError());
        return ;
    }
    id=(DWORD)GetCurrentThreadId();
    // Message loop
    MSG msg;

    //TODO maybe more QT-style?
    //If GetMessage retrieves the WM_QUIT message, the return value is zero.
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(g_kb_hook);

    //exit the thread
    return;
}

KeyboardThread::KeyboardThread(QObject *parent):QThread(parent)
{

}

void KeyboardThread::tryStop()
{
    if(isFinished()){
        return;
    }
    PostThreadMessage(id,WM_QUIT,0,0);
    exit();
}

void KeyboardThread::emitSignal()
{
    emit fresh();
}

KeyboardThread::~KeyboardThread()
{
    tryStop();
    wait();
}
