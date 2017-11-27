#ifndef GLOBALS_H
#define GLOBALS_H
#include <unordered_map>
#include <QString>
#include <Windows.h>
#include <QReadWriteLock>
#include <configfile.h>
#include "keyboardthread.h"
using namespace std;
/**
 * If we lost our past, what would we leave?
 * Now, and future.
 * No. Now will become past, future is always coming.
 */

extern unordered_map<int,long> record;
extern unordered_map<int,QString> key_name;
extern HHOOK g_kb_hook;
extern KeyboardThread* kThread;
extern QReadWriteLock rwLock;
extern string storagePath;
extern ConfigFile config;

LRESULT CALLBACK kb_proc(int code, WPARAM w, LPARAM l);
void loadStorage();
void saveStorage();

#endif // GLOBALS_H
