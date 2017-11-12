#include "globals.h"
#include <QDebug>
#include <iostream>
#include <fstream>

unordered_map<int,long> record={};
unordered_map<int,QString> key_name =
{
    {27,"Esc"},
    {112,"F1"},
    {113,"F2"},
    {114,"F3"},
    {115,"F4"},
    {116,"F5"},
    {117,"F6"},
    {118,"F7"},
    {119,"F8"},
    {120,"F9"},
    {121,"F10"},
    {122,"F11"},
    {123,"F12"},
    {48,"0"},
    {49,"1"},
    {50,"2"},
    {51,"3"},
    {52,"4"},
    {53,"5"},
    {54,"6"},
    {55,"7"},
    {56,"8"},
    {57,"9"},
    {65,"A"},
    {66,"B"},
    {67,"C"},
    {68,"D"},
    {69,"E"},
    {70,"F"},
    {71,"G"},
    {72,"H"},
    {73,"I"},
    {74,"J"},
    {75,"K"},
    {76,"L"},
    {77,"M"},
    {78,"N"},
    {79,"O"},
    {80,"P"},
    {81,"Q"},
    {82,"R"},
    {83,"S"},
    {84,"T"},
    {85,"U"},
    {86,"V"},
    {87,"W"},
    {88,"X"},
    {89,"Y"},
    {90,"Z"},
    {144,"NumLock"},
    {96,QString::fromWCharArray(L"数字小键盘0")},
    {97,QString::fromWCharArray(L"数字小键盘1")},
    {98,QString::fromWCharArray(L"数字小键盘2")},
    {99,QString::fromWCharArray(L"数字小键盘3")},
    {100,QString::fromWCharArray(L"数字小键盘4")},
    {101,QString::fromWCharArray(L"数字小键盘5")},
    {102,QString::fromWCharArray(L"数字小键盘6")},
    {103,QString::fromWCharArray(L"数字小键盘7")},
    {104,QString::fromWCharArray(L"数字小键盘8")},
    {105,QString::fromWCharArray(L"数字小键盘9")},
    {107,QString::fromWCharArray(L"数字小键盘+")},
    {109,QString::fromWCharArray(L"数字小键盘-")},
    {106,QString::fromWCharArray(L"数字小键盘*")},
    {111,QString::fromWCharArray(L"数字小键盘/")},
    {110,QString::fromWCharArray(L"数字小键盘.")},
    {13,QString::fromWCharArray(L"数字小键盘Enter")},
    {45,"Insert"},
    {46,"Delete"},
    {32,"SpaceBar"},
    {33,"PageUp"},
    {34,"PageDown"},
    {35,"End"},
    {36,"Home"},
    {37,QString::fromWCharArray(L"左边方向键←")},
    {38,QString::fromWCharArray(L"上方向键↑")},
    {39,QString::fromWCharArray(L"右方向键→")},
    {40,QString::fromWCharArray(L"下方向键↓")},
    {9,"Tab"},
    {20,"Caps Lock"},
    {160,"Shift(Left)"},
    {161,"Shift(Right)"},
    {162,"Ctrl(Left)"},
    {163,"Ctrl(Right)"},
    {91,"Win(Left)"},
    {92,"Win(Right)"},
    {164,"Alt(Left)"},
    {165,"Alt(Right)"},
    {189,"-"},
    {187,"="},
    {8,"Backspace"},
    {219,"["},
    {221,"]"},
    {220,"\\"},
    {186,";"},
    {222,"‘"},
    {188,","},
    {190,"."},
    {191,"/"},
    {13,"Enter"}
};
QReadWriteLock rwLock;
KeyboardThread* kThread=0;
HHOOK g_kb_hook = 0;
string storagePath="./data.dat";

LRESULT CALLBACK kb_proc(int code, WPARAM w, LPARAM l)
{
    PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)l;
    if (w == WM_KEYDOWN || w==WM_SYSKEYDOWN)
    {
        //qDebug("%s - vkCode [%04x], scanCode [%04x]\n",
        //   info, p->vkCode, p->scanCode);
        rwLock.tryLockForWrite();
        record[p->vkCode]++;
        rwLock.unlock();
        if(kThread!=0){
            kThread->emitSignal();
        }
    }
    // always call next hook
    return CallNextHookEx(g_kb_hook, code, w, l);
}


void loadStorage()
{
    ifstream ifs(storagePath,ios_base::in);
    if(ifs.good()){
        long time;
        int keyCode;
        while(ifs>>keyCode>>time){
            record[keyCode]=time;
        }
    }
    ifs.close();
}

void saveStorage()
{
    ofstream ofs(storagePath,ios_base::out);
    if(ofs.good()){
        for(auto it:record){
            ofs<<it.first<<"\t"<<it.second<<endl;
        }
    }
    ofs.close();
}
