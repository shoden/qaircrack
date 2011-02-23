#include "qaircrack.h"
#include "ui_qaircrack.h"

QAircrack::QAircrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QAircrack)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    monitor = false;
    _action = waiting;
    proc = new QProcess();

    connect(ui->monitorButton, SIGNAL(clicked()), this, SLOT(init()));

    connect(ui->myInterfaceButton, SIGNAL(clicked()), this, SLOT(myInterfaceInfo()));
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
}

void QAircrack::readFromStdout()
{
    switch(_action){
    case wifiInfo:
        _mac = QString(proc->readAllStandardOutput().data());
        ui->myMac->setText(_mac);
        break;
    default:
        qDebug() << "Wrong action!";
    }
}

void QAircrack::readFromStdErr()
{
    cout << "Error:" << proc->readAllStandardError().data() << endl;
}

void QAircrack::processError(QProcess::ProcessError e)
{
    qDebug() << "Process error:" << e;
}

void QAircrack::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if(exitStatus == QProcess::CrashExit)
        qDebug() << "Exit code:" << exitCode;
}

void QAircrack::myInterfaceInfo()
{
    bash("iwconfig");
}

QAircrack::~QAircrack()
{
    delete ui;
}

void QAircrack::init()
{
    // Start monitor
    if(!monitor){
        if(startMonitor()){
            monitor=true;
            ui->listButton->setEnabled(true);
            ui->listLabel->setEnabled(true);
            ui->captureButton->setEnabled(true);
            ui->captureLabel->setEnabled(true);
            ui->authButton->setEnabled(true);
            ui->authLabel->setEnabled(true);
            ui->inyectButton->setEnabled(true);
            ui->inyectLabel->setEnabled(true);
            ui->keyButton->setEnabled(true);
            ui->keyLabel->setEnabled(true);

            ui->myGroup->setEnabled(true);
            //ui->apGroup->setEnabled(true);
        }
        else{
            QMessageBox::critical(this, "Monitor", "no se pudo iniciar el monitor.");
        }
    }

    // Stop monitor
    else{
        if(stopMonitor()){
            monitor=false;
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

            ui->myGroup->setEnabled(false);
            ui->apGroup->setEnabled(true);
        }
        else{
            QMessageBox::critical(this, "Monitor", "no se pudo detener el monitor.");
        }
    }
}

void QAircrack::bash(const QString &s)
{
    QString command = QString("gnome-terminal -x /home/juan/bin/helper.sh %1 &").arg(s);
    system( command.toUtf8().data() );
}

bool QAircrack::startMonitor()
{/*
    system("sudo service network-manager stop");
    system("sudo service avahi-daemon stop");
    system("sudo killall -9 wpa_supplicant");
    system("killall -9 nm-applet");
    system("sudo airmon-ng start wlan0");
*/
    _action = wifiInfo;
    proc->start("/home/juan/bin/mac");
    return true;
}

bool QAircrack::stopMonitor()
{
    system("sudo airmon-ng stop mon0");
    system("sudo service network-manager start");
    system("sudo service avahi-daemon start");
    system("sudo /sbin/wpa_supplicant -u -s");
    system("nm-applet --sm-disable");

    return true;
}

void QAircrack::list()
{
    bash("sudo airmon-ng");
}

void QAircrack::capture()
{
    QString command = QString("sudo airodump-ng -c %1 --bssid %2 -w %3").arg("11").arg("aa:bb:cc:dd:ee:ff").arg("ficherito");
    bash( command );
}

void QAircrack::authenticate()
{
    //proc.start("gnome-terminal -t AUTENTICAR -e whoami");
    system("gnome-terminal -x /home/juan/bin/helper.sh whoami &");
}

void QAircrack::inyect()
{
    //proc.start("gnome-terminal -t INYECTAR -e whoami");
}

void QAircrack::key()
{
    //proc.start("gnome-terminal -t CLAVE -e whoami");
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
