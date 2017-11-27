#ifndef CONFIGFILE_H
#define CONFIGFILE_H
#include <unordered_map>
#include <vector>
#include <QRegExp>
#include <QString>

/**
 * You never know tomorrow or mishap which one will come first.
 * However, the fate knows, for it is ahead of them.
 *
 * @brief The ConfigFile class controls all the configuation about this KeyboardRecorder.
 */

class ConfigFile
{
private:
    class Parser{
    private:
        typedef bool (*ParserFunction)(char c) ;
        std::string data="";
        QString qData;
        std::string lastKeyName="";
        unsigned pos=0;
        QRegExp lineExpr,intExpr,floatExpr,boolExpr;

        std::unordered_map<int,std::unordered_map<ParserFunction,int>> functionList;
    public:
        enum ParserType{STRING,INT,FLOAT,BOOLEAN,UNKNOWN};
        Parser(std::string input);
        std::pair<ParserType, QString> get();
        std::string getLastKeyName()const;
        bool isEnd() const;
    };
    std::string path;
    std::string lastError;
    std::unordered_map<std::string,std::pair<Parser::ParserType,QString>> configMap;
    bool init();
public:
    ConfigFile(std::string path="./config.cfg");
    std::string getString(std::string path,std::string& defaultValue)const;
    int getInteger(std::string path, int& defaultValue) const;
    float getFloat(std::string path, float &defaultValue) const;
    bool getBool(std::string path,bool defaultValue) const;
};

#endif // CONFIGFILE_H
