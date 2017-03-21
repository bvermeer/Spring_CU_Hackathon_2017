#ifndef HW_BUTTON_POLLING_H
#define HW_BUTTON_POLLING_H

#include "tiny_gpio.h"
#include <QObject>
#include <QMap>
#include "DebugDefines.h"

class hw_button_polling : public QObject
{
    Q_OBJECT

    tiny_gpio gpio;

public:
    hw_button_polling(QObject *parent = 0);

public slots:
    void checkButtons();

signals:
   void button_1_Changed(bool newValue);
   void button_2_Changed(bool newValue);
   void button_3_Changed(bool newValue);
   void button_4_Changed(bool newValue);

private:
   // Numbering convention for buttons is 1 on top, 4 on bottom
   QMap<uint, bool> keys;
};

#endif // HW_BUTTON_POLLING_H
