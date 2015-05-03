#ifndef HARDWARESETTINGS_H
#define HARDWARESETTINGS_H

#include <QDialog>

namespace Ui {
class HardwareSettings;
}

class HardwareSettings : public QDialog
{
    Q_OBJECT

public:
    explicit HardwareSettings(QWidget *parent = 0);
    ~HardwareSettings();

private:
    Ui::HardwareSettings *ui;
};

#endif // HARDWARESETTINGS_H
