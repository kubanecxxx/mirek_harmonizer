#include "harmonizer.h"
#include "ui_harmonizer.h"
#include <stdio.h>
#include <stdlib.h>
#include "QDebug"
#include "qextserialenumerator.h"
#include "QTimer"

Harmonizer::Harmonizer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Harmonizer)
{
    ui->setupUi(this);

    ui->butVypnout->setProperty("Command","off");
    ui->butZapnout->setProperty("Command","on");

    ui->spinBalance->setProperty("Command","volume");
    ui->spinKey->setProperty("Command","key");
    ui->spinMode->setProperty("Command","mode");
    ui->spinShift->setProperty("Command","harmony");

    connect(ui->butVypnout,SIGNAL(clicked()),this,SLOT(butSloty()));
    connect(ui->butZapnout,SIGNAL(clicked()),this,SLOT(butSloty()));

    connect(ui->spinBalance,SIGNAL(valueChanged(int)),this,SLOT(spinSloty(int)));
    connect(ui->spinKey,SIGNAL(valueChanged(int)),this,SLOT(spinSloty(int)));
    connect(ui->spinMode,SIGNAL(valueChanged(int)),this,SLOT(spinSloty(int)));
    connect(ui->spinShift,SIGNAL(valueChanged(int)),this,SLOT(spinSloty(int)));


    butConnect = new QPushButton;
    butConnect->setText(trUtf8("Připojit"));
    connect(butConnect,SIGNAL(clicked()),this,SLOT(slotConnect()));

    comboPorts = new QComboBox;
    ledka = new HLed;

    statusBar()->addPermanentWidget(comboPorts);
    statusBar()->addPermanentWidget(butConnect);
    statusBar()->addPermanentWidget(ledka);

    PortSettings nastaveni;
    nastaveni.BaudRate = BAUD115200;
    nastaveni.DataBits = DATA_8;
    nastaveni.FlowControl = FLOW_HARDWARE;
    nastaveni.Parity = PAR_NONE;
    nastaveni.StopBits = STOP_1;
    nastaveni.Timeout_Millisec = 100;

    comport = new QextSerialPort(nastaveni,QextSerialPort::EventDriven,this);
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    connect(comport,SIGNAL(readyRead()),this,SLOT(comport_new_data()));
    QStringList list;

    for (int i = 0 ; i < ports.count() ; i++)
    {

    #ifdef Q_OS_WIN
         list << ports.at(i).portName;
    #else
        list << ports.at(i).physName;

    #endif

    }

    #ifndef Q_OS_WIN
    list << "/tmp/interceptty";
    #endif

    comboPorts->addItems(list);
    roznout(false);
}

Harmonizer::~Harmonizer()
{
    comport->close();
    delete ui;
}

void Harmonizer::comport_new_data()
{
   QByteArray array = comport->readAll();
   ui->memo->appendPlainText(array);
}

void Harmonizer::butSloty()
{
    setupHarmonizer(sender()->property("Command").toByteArray().constData());
}

void Harmonizer::spinSloty(int value)
{
    const char * data = sender()->property("Command").toByteArray().constData();
    char buffer[100];

    sprintf(buffer , "%s %d", data,value);
    setupHarmonizer(buffer);
}

void Harmonizer::slotDisconnect()
{
    connect(butConnect,SIGNAL(clicked()),this,SLOT(slotConnect()));
    disconnect(butConnect,SIGNAL(clicked()),this,SLOT(slotDisconnect()));
    butConnect->setText(trUtf8("Připojit"));
    ui->memo->appendPlainText(trUtf8("Odpojeno"));

    roznout(false);
    comport->close();
}

void Harmonizer::slotConnect()
{
    comport->setPortName(comboPorts->currentText());
    comport->open(QextSerialPort::ReadWrite);

    if (comport->isOpen())
    {
        ui->memo->appendPlainText(trUtf8("Připojeno"));
        butConnect->setText("Odpojit");
        disconnect(butConnect,SIGNAL(clicked()),this,SLOT(slotConnect()));
        connect(butConnect,SIGNAL(clicked()),this,SLOT(slotDisconnect()));
        roznout(true);
    }
    else
    {
        ui->memo->appendPlainText(trUtf8("Nepovedlo se připojit!!!!!"));
    }
}

void Harmonizer::roznout(bool roznout)
{
    ledka->turnOn(roznout);
    comboPorts->setDisabled(roznout);

    ui->PrimarySizer->setEnabled(roznout);

    if (roznout)
    {
        QByteArray buffer;

        buffer = "volume ";
        buffer += QString::number(ui->spinBalance->value());
        setupHarmonizer(buffer);

        buffer = "key ";
        buffer += QString::number(ui->spinKey->value());
        setupHarmonizer(buffer);

        buffer = "harmony ";
        buffer += QString::number(ui->spinShift->value());
        setupHarmonizer(buffer);

        buffer = "mode ";
        buffer += QString::number(ui->spinMode->value());
        setupHarmonizer(buffer);
    }
}

void Harmonizer::setupHarmonizer(const char *command)
{
    if(comport->isOpen())
    {
        QByteArray buffer = "harm ";
        buffer += command;
       // ui->memo->appendPlainText(buffer);
        buffer += "\r\n";

        comport->write(buffer);
        comport->flush();
    }
    else
    {
        ui->memo->appendPlainText("Chyba!!");
    }
}
