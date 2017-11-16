#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include <QString>
#include <unordered_map>
/**
 * You never know tomorrow or mishap which one will come first.
 * However, the fate knows, for it is ahead of them.
 *
 * @brief The ConfigFile class controls all the configuation about this KeyboardRecorder.
 */

class ConfigFile
{
private:
    QString path;
    QString lastError;
    std::unordered_map<QString,QString> configMap;
    bool init();
public:
    ConfigFile(QString path="./config.cfg");
    QString getQString(QString path,QString defaultValue);
    bool getBool(QString path,bool defaultValue);
};

#endif // CONFIGFILE_H
