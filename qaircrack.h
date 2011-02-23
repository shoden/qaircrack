#ifndef QAIRCRACK_H
#define QAIRCRACK_H

#include <QWidget>
#include <QMessageBox>
#include <QProcess>
#include <iostream>
#include <QDebug>

enum action{
    waiting,
    wifiInfo,
    startMonitor,
    list,
    capture,
    authenticate,
    inyect,
    key
};

using namespace std;

#define BASH_HELPER "/home/juan/bin/helper.sh"

namespace Ui {
    class QAircrack;
}

class QAircrack : public QWidget {
    Q_OBJECT
public:
    QAircrack(QWidget *parent = 0);
    ~QAircrack();

    bool startMonitor();
    bool stopMonitor();
    void bash(const QString &);

public slots:
    void myInterfaceInfo();
    void init();
    void list();
    void capture();
    void authenticate();
    void inyect();
    void key();

    void readFromStdout();
    void readFromStdErr();
    void processError(QProcess::ProcessError);
    void processFinished(int,QProcess::ExitStatus);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::QAircrack *ui;
    bool monitor;
    QProcess *proc;

    QString _wlan;
    QString _mon;
    QString _mac;
    action _action;
};

#endif // QAIRCRACK_H
