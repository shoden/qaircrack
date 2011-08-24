/*
 * Copyright (C) 2011 Juan Roldan Ruiz <juan@shoden.es>
 *  
 * This file is part of QAircrack.
 * 
 * QAircrack is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * QAircrack is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with QAircrack.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef QAIRCRACK_H
#define QAIRCRACK_H

#include <QWidget>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QCloseEvent>
#include <QPixmap>

enum action{
    waiting,
    updatingWlanList,
    monitorInit,
    monitorUp,
    monitorDown,
    list,
    capturing,
    authenticating,
    inyecting,
    cracking
};

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
    void bash(const QString &,const QString &,const QString &);

public slots:
    void updateWlanList();
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
    void listingFlag();

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

    QDir capDir;
    QString capFile;
    int ret;
};

#endif // QAIRCRACK_H
