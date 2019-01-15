#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parser.h"
#include "drawarea.h"
#include "stateobj.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QVariantMap>

namespace Ui {
class MainWindow;
}

enum PossibleObjects{
    stateName,
    stateParam,
    relative,
    CanRaise,
    raiseParam,
    options,
    unknown
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool rememberState(QStringList &allText);
    bool needConfigure(QString text);
    bool configureObject(StateObj& obj, QString text);
    bool checkEnd(QString name, QString param);
    bool checkIgnoreFilter(QString name, QString param);
    bool checkExceptFilter(QString name, QString param);
    bool checkTimeOut(QString name);
    void addRelative(StateObj& obj, QString text);
    QStringList getTrimmedList(QString text);
    void configureStartState(QString stateName);
    PossibleObjects whatIsIt(QString text);

private slots:
    void processData();
    void saveSnapShot();
    void readSelectedFile();

private:
    Ui::MainWindow *ui;
    Parser* parser;
    QGridLayout* layout;
    QPushButton* takeFilePath;
    QPushButton* updateWidget;
    QPushButton* createImage;
    DrawArea* drawArea;

    QMap<QString, StateObj*> stateObjects;
    QString startStateName;

};

#endif // MAINWINDOW_H
