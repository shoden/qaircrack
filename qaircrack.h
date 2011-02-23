#ifndef QAIRCRACK_H
#define QAIRCRACK_H

#include <QWidget>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QCloseEvent>
#include <QPixmap>

#define CAPTURE_FILE "tawapo"

enum action{
    waiting,
    monitorInit,
    monitorUp,
    monitorDown,
    list,
    capturing,
    authenticating,
    inyecting,
    cracking
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

    void initMonitor();
    void startMonitor();
    void stopMonitor();
    void bash(const QString &);

public slots:
    void myInterfaceInfo();
    void toggleMonitor();
    void list();
    void capture();
    void authenticate();
    void inyect();
    void key();
    void readFromStdout();
    void readFromStdErr();
    void processError(QProcess::ProcessError);
    void processFinished(int,QProcess::ExitStatus);
signals:

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    Ui::QAircrack *ui;
    bool monitor;
    bool listing;
    QProcess *proc;
    QProcess *listingProc;

    QString _wlan;
    QString _mon;
    QString _mac;
    action _action;
};

#endif // QAIRCRACK_H
