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
 
#include "qaircrack.h"
#include "ui_qaircrack.h"

QAircrack::QAircrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QAircrack)
{
    ui->setupUi(this);

    ui->monitorButton->setIcon(QIcon("/usr/share/pixmaps/qaircrack/monitor-off.png"));
    ui->listButton->setIcon(QIcon("/usr/share/pixmaps/qaircrack/listar.png"));
    ui->captureButton->setIcon(QIcon("/usr/share/pixmaps/qaircrack/capturar.png"));
    ui->authButton->setIcon(QIcon("/usr/share/pixmaps/qaircrack/autenticar.png"));
    ui->inyectButton->setIcon(QIcon("/usr/share/pixmaps/qaircrack/inyectar.png"));
    ui->keyButton->setIcon(QIcon("/usr/share/pixmaps/qaircrack/clave.png"));
    setWindowIcon(QIcon("/usr/share/pixmaps/qaircrack.png"));

    setFixedSize(width(), height());
    QDesktopWidget desk;
    move( (desk.width()/2) - (width()/2), (desk.height()/2) - (height()/2));

    monitor = false;
    listing = false;
    _action = waiting;
    proc = new QProcess();
    listingProc = new QProcess();
    capDir.setPath(QDir::homePath().append("/capturas"));
    capFile = capDir.absolutePath().append("/captura");

    connect(ui->monitorButton, SIGNAL(clicked()), this, SLOT(toggleMonitor()));
    connect(ui->listButton, SIGNAL(clicked()), this, SLOT(list()));
    connect(ui->captureButton, SIGNAL(clicked()), this, SLOT(capture()));
    connect(ui->authButton, SIGNAL(clicked()), this, SLOT(authenticate()));
    connect(ui->inyectButton, SIGNAL(clicked()), this, SLOT(inyect()));
    connect(ui->keyButton, SIGNAL(clicked()), this, SLOT(key()));

    connect(proc,SIGNAL(readyReadStandardOutput()),this,SLOT(readFromStdout()));
    connect(proc,SIGNAL(readyReadStandardError()),this,SLOT(readFromStdErr()));
    connect(proc,SIGNAL(error(QProcess::ProcessError)),this,SLOT(processError(QProcess::ProcessError)));
    connect(proc,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(processFinished(int,QProcess::ExitStatus)) );

    proc->setProcessChannelMode(QProcess::MergedChannels);
    ui->myInterface->setInsertPolicy(QComboBox::InsertAtBottom);
    updateWlanList();
}

void QAircrack::readFromStdout()
{
    QString output = proc->readAllStandardOutput().data();
    QRegExp reg;
    QStringList list;

    switch(_action){
    case updatingWlanList:
        list = output.split("\n", QString::SkipEmptyParts);
        foreach (QString wlan, list)
            if(!wlan.contains("mon"))
                ui->myInterface->insertItem(ui->myInterface->count(), wlan);
        break;
    case waiting:
        qDebug() << "Stdout during waiting";
        break;
    case monitorInit:
        _mac = output;
        ui->myMac->setText(_mac);
        break;
    case monitorUp:
        reg.setPattern("\\bmonitor mode enabled on\\b");
        if( reg.indexIn(output) >= 0 ){
            ui->myMonitor->setText( output.trimmed().replace(")","").split(" ").last() );
            ui->listButton->setEnabled(true);
            ui->listLabel->setEnabled(true);
        }
        break;
    case monitorDown:
        qDebug() << "Stdout during monitorDown:" << output;
        break;

    default:
        qDebug() << "Wrong action!";
    }
}

void QAircrack::readFromStdErr()
{
    qDebug() << "Error:" << proc->readAllStandardError().data();
}

void QAircrack::processError(QProcess::ProcessError e)
{
    QString msg;

    switch(_action){
    case monitorInit:
        msg = QString::fromUtf8("Interfaz incorrecta.");
        break;
    case monitorUp:
        msg = QString::fromUtf8("No se pudo iniciar el monitor.");
        break;
    case monitorDown:
        msg = QString::fromUtf8("No se pudo parar el monitor.");
        break;
    default:
        qDebug() << "Wrong action!";
    };

    QMessageBox::critical(this, "Error", msg);
}

void QAircrack::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString msg;

    if(exitStatus == QProcess::CrashExit)
        qDebug() << "Crash!";

    // Error
    if(exitCode != 0){
        switch(_action){
        case waiting:
            qDebug() << "Process finished with error during waiting";
            break;
        case monitorInit: // Interface info error
            ui->myMac->setText("");
            msg = QString::fromUtf8("La interfaz no es correcta.");
            break;

        case monitorUp: // Monitor start error
            msg = "No se pudo iniciar el monitor";
            break;

        default:
            qDebug() << "Wrong code!";
        }
        QMessageBox::critical(this, "Error", msg);
    }
    // Ok
    else{
        switch(_action){
        case updatingWlanList:
            _action = waiting;
            break;
        case waiting:
            qDebug() << "Process finished during waiting";
            break;
        case monitorInit: // Interface info retrieved
            _action = monitorUp;
            startMonitor();
            break;
        case monitorUp: // Monitor started
            if(ui->myMonitor->text().isEmpty()){
                QMessageBox::critical(this, "Error", "No se pudo iniciar el monitor");
            }
            else{
                qDebug() << "monitor=true";
                ui->monitorButton->setIcon(QIcon("/usr/share/pixmaps/qaircrack/monitor-on.png"));
                ui->monitorLabel->setText("Desactivar\nmonitor");
                monitor = true;
                _action = waiting;
                ui->listButton->setEnabled(true);
                ui->listLabel->setEnabled(true);
            }
            break;
        case monitorDown: // Monitor stopped
            ui->monitorButton->setIcon(QIcon("/usr/share/pixmaps/qaircrack/monitor-off.png"));
            ui->monitorLabel->setText("Activar\nmonitor");
            monitor = false;
            _action = waiting;
            break;

        default:
            qDebug() << "Wrong code!";
        }
    }

    qDebug() << "Exit code:" << exitCode;
}

QAircrack::~QAircrack()
{
    delete ui;
}

void QAircrack::toggleMonitor()
{
    if(!monitor)
        initMonitor();
    else
        stopMonitor();
}

void QAircrack::bash(const QString &s, const QString &title, const QString &geometry="")
{
    QString size=(geometry=="") ? "" : QString("--geometry ").append(geometry);
    QString command = QString("gnome-terminal %1 -t \"%2\" -x qaircrack_terminal %3 &").arg(size).arg(title).arg(s);
    qDebug() << "bash:" << command;
    ret = system( command.toUtf8().data() );
}

void QAircrack::initMonitor()
{
    _action = monitorInit;
    proc->start(QString("qaircrack_mac %1").arg(ui->myInterface->currentText()));
}

void QAircrack::startMonitor()
{
    // Kill problematic processes
    ret = system("sudo service network-manager stop");
    ret = system("sudo service avahi-daemon stop");
    ret = system("sudo killall -9 wpa_supplicant");
    ret = system("killall -9 nm-applet");

    // Stop current monitors
    ret = system("qaircrack_stopmonitors");

    // Change MAC address
   // ret = system(QString("sudo macchanger -r %1").arg(ui->myInterface->currentText()).toUtf8().data());

    // Start monitor
    ui->myMonitor->setText("");
    _action = monitorUp;
    proc->start(QString("sudo airmon-ng start %1").arg(ui->myInterface->currentText()));
}

void QAircrack::stopMonitor()
{
    // Disable controls
    ui->listButton->setEnabled(false);
    ui->listLabel->setEnabled(false);
    ui->captureButton->setEnabled(false);
    ui->captureLabel->setEnabled(false);
    ui->authButton->setEnabled(false);
    ui->authLabel->setEnabled(false);
    ui->inyectButton->setEnabled(false);
    ui->inyectLabel->setEnabled(false);
    ui->keyButton->setEnabled(false);
    ui->keyLabel->setEnabled(false);
    ui->apGroup->setEnabled(false);

    // Stop current monitors
    _action = monitorDown;
    ui->myMac->setText("");
    ui->myMonitor->setText("");
    proc->start("qaircrack_stopmonitors");

    // Resume problematic processes
    ret = system("sudo service network-manager start");
    ret = system("sudo service avahi-daemon start");
    ret = system("nm-applet --sm-disable &");
}

void QAircrack::updateWlanList()
{
    ui->myInterface->clear();
    _action = updatingWlanList;
    proc->start("qaircrack_wlans");
}

void QAircrack::list()
{
    QString command = QString("gnome-terminal --geometry 90x25 -e \"sudo airodump-ng -t OPN -t WEP %1\"").arg(ui->myMonitor->text());

    if(listing == true){
        QMessageBox::warning(this, "Aviso", QString::fromUtf8("Ya hay un terminal abierto listando las redes."));
    }
    else{
        qDebug() << command;
        listing = true;
        listingProc->start(command);
        connect(listingProc,SIGNAL(finished(int)),this,SLOT(listingFlag()));
        ui->apGroup->setEnabled(true);
        ui->captureButton->setEnabled(true);
        ui->captureLabel->setEnabled(true);
    }
}

void QAircrack::listingFlag()
{
    qDebug() << "Bajando bandera";
    listing = false;
}

void QAircrack::capture()
{
    if(listing == true){
        qDebug() << "Capturar: no!";
        listingProc->kill();
        listing = false;
    };
  //  else{
        qDebug() << "Capturar: si!";
        // Prepare directory
        if(capDir.exists())
            ret = system( QString("sudo rm -fR %1").arg(capDir.absolutePath()).toUtf8().data() );

        if(!capDir.mkdir(capDir.absolutePath()))
            QMessageBox::critical(this, "Error", "No se pudo crear el directorio para guardar las capturas.");

        // Start capturing
        _action = capturing;
        QString command = QString("sudo airodump-ng %1 -c %2 --bssid '%3' -w '%4'").arg(ui->myMonitor->text()).arg(ui->apChannel->text()).arg(ui->apMac->text()).arg(capFile);
        bash( command, "Capturar", "90x14" );
        ui->authButton->setEnabled(true);
        ui->authLabel->setEnabled(true);
   // }
}

void QAircrack::authenticate()
{
    _action = authenticating;
    QString command = QString("sudo aireplay-ng -1 0 -e '%1' -a '%2' -h '%3' %4").arg(ui->apName->text()).arg(ui->apMac->text()).arg(_mac.trimmed()).arg(ui->myMonitor->text());
    bash( command, "Autenticar" );
    ui->inyectButton->setEnabled(true);
    ui->inyectLabel->setEnabled(true);
}

void QAircrack::inyect()
{
    _action = inyecting;
    QString command = QString("sudo aireplay-ng -3 -b '%2' -h '%3' %4").arg(ui->apMac->text()).arg(_mac.trimmed()).arg(ui->myMonitor->text());
    bash( command, "Inyectar" );
    ui->keyButton->setEnabled(true);
    ui->keyLabel->setEnabled(true);
}

void QAircrack::key()
{
    _action = cracking;
    QString f = QDir::homePath().append("/Escritorio/").append(ui->apName->text());
    QString command = QString("aircrack-ng -e %1 -b %2 %3 -l %4").arg(ui->apName->text()).arg(ui->apMac->text()).arg(QString("%1*.cap").arg(capFile)).arg(f);
    bash( command, "Obtener clave" );
    qDebug() << command;
}

void QAircrack::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QAircrack::closeEvent(QCloseEvent *e)
{
    if(monitor){
        QMessageBox::warning(this, "Cerrar", QString::fromUtf8("Desactiva el monitor antes de cerrar esta aplicación."));
        e->ignore();
    }
    else
        e->accept();
}
