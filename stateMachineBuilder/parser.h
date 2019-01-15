#ifndef PARSER_H
#define PARSER_H
#include <QString>
#include <QStringList>

class Parser
{
public:
    Parser();
    Parser(QString filePath);
    void readText();
    void deleteComments();
    void deleteBadStrings();
    QStringList getText();
    void setFilePath(QString filePath) { this->filePath = filePath; }

private:
    QString filePath;
    QStringList text;
};

#endif // PARSER_H
