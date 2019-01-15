#include "parser.h"

#include <QFile>
#include <QDebug>
#include <QCoreApplication>

Parser::Parser()
{
    this->filePath = QCoreApplication::applicationDirPath() + "/text.txt";
}

Parser::Parser(QString filePath)
{
    this->filePath = filePath;
}

void Parser::readText()
{
    QFile file(filePath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd()) {
            text.append(file.readLine());
        }
        file.close();
    }
    qDebug() << "end read";
}

void Parser::deleteComments()
{
    if (text.isEmpty()) return;
    int i = 0;
    for (QString element : text) {
        text[i] = element.mid(0, element.indexOf(";"));
        ++i;
    }
    qDebug() << "end delete comments";
}

void Parser::deleteBadStrings()
{
    if (text.isEmpty()) return;
    while (!text.isEmpty() && !text.first().contains("States")) {
        text.removeFirst();
    }
    if (text.first().contains("States"))
        text.removeFirst();
    qDebug() << "end delete bad strings";
}

QStringList Parser::getText()
{
    readText();
    deleteBadStrings();
    deleteComments();
    return text;
}
