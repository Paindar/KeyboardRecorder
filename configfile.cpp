#include "configfile.h"
#include <fstream>
#include <string>

bool ConfigFile::init()
{
    std::filePath=path.toStdString();
    std::fstream file(filePath,std::ios_base::in);
    if(!file.good()){
        lastError="[Error]Failed to open "+filePath+". File isn't existed.";
        file.open(filePath,std::ios_base::out);
        file.close();
        return false;
    }

    std::string buffer="";
    int line=0;
    while(std::getline(file,buffer)){

        line++;
        int pos=buffer.find(':');
        if(pos==-1)
        {
            lastError="[Error]Cannot init configuation at line "+QString::number(line);
            return false;
        }

        configMap[buffer.substr(0,pos)]=buffer.substr(pos);
    }
    return true;
}

ConfigFile::ConfigFile(QString path)
{
    this->path=path;

}

QString ConfigFile::getQString(QString path, QString defaultValue)
{

}

bool ConfigFile::getBool(QString path, bool defaultValue)
{

}
