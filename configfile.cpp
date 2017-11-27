#include "configfile.h"
#include <fstream>
#include <string>
#include <qDebug>



ConfigFile::Parser::Parser(std::string input)
{
    data=input;
    pos=0;
    qData=QString::fromStdString(data);
    /*
    functionList[0][([](char c){return c==' ' or c=='\n';})]=0;
    functionList[0][([](char c){})]*/
    lineExpr=QRegExp("([A-Za-z0-9_]+):([^\n]*)");
    intExpr=QRegExp("[0-9]+");
    floatExpr=QRegExp("[0-9]+(\.[0-9]+)?(e[0-9]+)?");
    boolExpr=QRegExp("true|false");
}

std::pair<ConfigFile::Parser::ParserType, QString> ConfigFile::Parser::get()
{
    lastKeyName="";
    if(pos==-1)
        return std::pair<ConfigFile::Parser::ParserType, QString>();
    int nextPos=qData.indexOf(lineExpr,pos);

    if(nextPos==-1){
        pos=-1;
        return std::pair<ConfigFile::Parser::ParserType, QString>();
    }
    lastKeyName=lineExpr.cap(1).toStdString();
    pos=nextPos+lineExpr.cap(0).length();

    QString value=lineExpr.cap(2);
    if(floatExpr.indexIn(value,0)!=-1){
        value=floatExpr.capturedTexts()[0];
        return std::pair<ConfigFile::Parser::ParserType, QString>(ParserType::FLOAT,value);
    }
    else if(intExpr.indexIn(value,0)!=-1){
        value=intExpr.capturedTexts()[0];
        return std::pair<ConfigFile::Parser::ParserType, QString>(ParserType::INT,value);
    }
    else if(boolExpr.indexIn(value,0)!=-1){
        value=boolExpr.capturedTexts()[0];
        return std::pair<ConfigFile::Parser::ParserType, QString>(ParserType::BOOLEAN,value);
    }
    return std::pair<ConfigFile::Parser::ParserType, QString>(ParserType::UNKNOWN,value);
}

std::string ConfigFile::Parser::getLastKeyName() const
{
    return lastKeyName;
}

bool ConfigFile::Parser::isEnd() const
{
    return pos==-1;
}


bool ConfigFile::init()
{
    std::string filePath=path;
    std::fstream file(filePath,std::ios_base::in);
    if(!file.good()){
        lastError=std::string("[Error]Failed to open ").append(path).append(". File isn't existed.");
        return false;
    }
    std::istreambuf_iterator<char> beg(file), end;
    std::string str(beg, end);
    Parser parser(str);
    qDebug()<<str.c_str();
    while(parser.isEnd()==false){
        std::pair<Parser::ParserType,QString> kv=parser.get();
        if(parser.getLastKeyName().empty()){
            lastError="Key name is empty.";
            continue;
        }
        qDebug()<<"find "<<QString::fromStdString(parser.getLastKeyName().append(" : ")).append(kv.second);
        switch(kv.first){
        case Parser::ParserType::INT:
            configMap[parser.getLastKeyName()]=std::pair<Parser::ParserType,QString>(Parser::ParserType::INT,kv.second);
            break;
        case Parser::ParserType::FLOAT:
            configMap[parser.getLastKeyName()]=std::pair<Parser::ParserType,QString>(Parser::ParserType::FLOAT,kv.second);
            break;
        case Parser::ParserType::BOOLEAN:
            configMap[parser.getLastKeyName()]=std::pair<Parser::ParserType,QString>(Parser::ParserType::BOOLEAN,kv.second);
            break;
        default:
            qDebug()<<QString("cannot recognize string : ")<<kv.second;
        }
    }
    return true;
}

ConfigFile::ConfigFile(std::string path)
{
    this->path=path;
    init();
}

std::string ConfigFile::getString(std::string path, std::string &defaultValue) const
{
    if(configMap.find(path)==configMap.end())
        return defaultValue;
    return configMap.at(path).second.toStdString();
}

int ConfigFile::getInteger(std::string path, int &defaultValue) const
{
    if(configMap.find(path)==configMap.end() && configMap.at(path).first!=Parser::ParserType::INT)
        return defaultValue;
    return configMap.at(path).second.toInt();
}

float ConfigFile::getFloat(std::string path, float &defaultValue) const
{
    if(configMap.find(path)==configMap.end() && configMap.at(path).first!=Parser::ParserType::INT)
        return defaultValue;
    return configMap.at(path).second.toInt();
}

bool ConfigFile::getBool(std::string path, bool defaultValue) const
{
    if(configMap.find(path)==configMap.end() && configMap.at(path).first!=Parser::ParserType::BOOLEAN)
        return defaultValue;
    return configMap.at(path).second=="true";
}

