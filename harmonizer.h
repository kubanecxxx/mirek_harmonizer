#ifndef HARMONIZER_H
#define HARMONIZER_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include "hled.h"
#include "qextserialport.h"

namespace Ui {
class Harmonizer;
}

class Harmonizer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Harmonizer(QWidget *parent = 0);
    ~Harmonizer();
    
private:
    Ui::Harmonizer *ui;
    void setupHarmonizer(const char * command);

    QPushButton * butConnect;
    QComboBox * comboPorts;
    HLed * ledka;
    QextSerialPort * comport;
    void roznout(bool roznout);

private slots:
    void butSloty(void);
    void spinSloty(int value);
    void slotConnect(void);
    void slotDisconnect(void);
    void comport_new_data(void);
};

#endif // HARMONIZER_H
