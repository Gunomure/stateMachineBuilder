#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startStateName = "";
    parser = new Parser();

    layout = new QGridLayout(this);

    takeFilePath = new QPushButton("путь до файла", this);
    updateWidget = new QPushButton("Обновить изображение", this);
    createImage = new QPushButton("Сохранить", this);
    connect(createImage, &QPushButton::clicked, this, &MainWindow::saveSnapShot);
    connect(takeFilePath, &QPushButton::clicked, this, &MainWindow::readSelectedFile);
    connect(updateWidget, &QPushButton::clicked, this, &MainWindow::processData);

    drawArea = new DrawArea(this);

    layout->addWidget(takeFilePath, 0, 0, 1, 1);
    layout->addWidget(updateWidget, 0, 1, 1, 1);
    layout->addWidget(createImage, 0, 2, 1, 1);
    layout->addWidget(drawArea, 1, 0, 3, 3);
    ui->centralWidget->setLayout(layout);

    processData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processData()
{
    stateObjects.clear();
    drawArea->clearArea();

    QStringList text = parser->getText();
    if (text.isEmpty()) return;
    while(rememberState(text));
    qDebug() << "test";
    text.removeFirst();
    QStringList optionParam = getTrimmedList(text.first());
    configureStartState(optionParam.last());
    drawArea->setStates(stateObjects);
}

PossibleObjects MainWindow::whatIsIt(QString text)
{
    int numberSpace = 0;
    int i = 0;
    while (text[i] == ' ') {
        ++numberSpace;
        ++i;
    }

    switch (numberSpace) {
    case 2:
        return options;
    case 4:
        return stateName;
    case 6:
        return stateParam;
    case 8:
        return relative;
    case 10:
        return CanRaise;
    case 12:
        return raiseParam;
    default:
        return unknown;
    }
}

bool MainWindow::rememberState(QStringList &allText)
{
    PossibleObjects thisIs = whatIsIt(allText.first());

    StateObj* state = new StateObj();
    QString strStateName = "";
    if (thisIs == stateName) {
        strStateName = allText.first().trimmed();
        state->setName(strStateName);
        allText.removeFirst();
        thisIs = whatIsIt(allText.first());
        stateObjects.insert(strStateName.toLower(), state);
    }

    //запоминаем последнее условие перехода из состояния
    QString conditionName = "";
    while (thisIs != stateName) {
        QString text;
        if (!allText.isEmpty()) text = allText.first().trimmed();
        switch (thisIs) {
        case stateParam:
            if(needConfigure(text)) {
                configureObject(*state, text);
            }
            break;
        case relative:
            conditionName = getTrimmedList(text).first();
            addRelative(*state, text);
            break;
        case CanRaise:
            break;
        case raiseParam:
            state->addRaise(conditionName, text.trimmed());
            break;
        case options:
            return false;
            break;
        case unknown:
            QMessageBox msg;
            msg.setText("Ошибка при парсинге входного файла");
            msg.exec();
            return false;
            break;
        }
        allText.removeFirst();
        if (!allText.isEmpty())
            thisIs = whatIsIt(allText.first());
    }
    return true;
}

bool MainWindow::needConfigure(QString text)
{
    QStringList temp = getTrimmedList(text);
    return temp.first() == "isendstate"
            || temp.first() == "transitions"
            || temp.first() == "timeout";
}

bool MainWindow::configureObject(StateObj& obj, QString text)
{
    QStringList stateParam =  getTrimmedList(text);

    if (checkEnd(stateParam.first(), stateParam.last())) {
        obj.configure(end);
        return true;
    }
    else if (checkIgnoreFilter(stateParam.first(), stateParam.last())) {
        obj.configure(ignoreFilter);
        return true;
    }
    else if (checkExceptFilter(stateParam.first(), stateParam.last())) {
        obj.configure(exceptionFilter);
        return true;
    } else if (checkTimeOut(stateParam.first())) {
        obj.setTimeOut(stateParam.last());
    }
    return false;
}

bool MainWindow::checkEnd(QString name, QString param)
{
    return name == "isendstate" && param == "true";
}

bool MainWindow::checkIgnoreFilter(QString name, QString param)
{
    return name == "transitions" && param == "1";
}


bool MainWindow::checkExceptFilter(QString name, QString param)
{
    return name == "transitions" && param == "0";
}

bool MainWindow::checkTimeOut(QString name)
{
    return name == "timeout";
}

void MainWindow::addRelative(StateObj& obj, QString text)
{
    QStringList relation = getTrimmedList(text);
    obj.addRelation(relation.first(), relation.last());
}

QStringList MainWindow::getTrimmedList(QString text)
{
    QStringList result = text.split(":");
    result[0] = result.first().toLower().trimmed();
    result[1] = result.last().toLower().trimmed();
    return result;
}

void MainWindow::configureStartState(QString stateName)
{
    if (stateObjects.contains(stateName))
        stateObjects[stateName]->configure(start);
}

void MainWindow::saveSnapShot()
{
    QPixmap picture = drawArea->grab(QRect(QPoint(0,0), drawArea->size()));
    picture.save(QCoreApplication::applicationDirPath() + "/pic.png");
}

void MainWindow::readSelectedFile()
{
    QString selectedFilePath = "";
    selectedFilePath = QFileDialog::getOpenFileName(NULL,
                                                    QString::fromUtf8("Открыть файл"),
                                                    QDir::currentPath(),""
                                                                        "Text (*.txt);;All files (*.*)");

    parser->setFilePath(selectedFilePath);
    processData();
}
