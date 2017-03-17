#include "hw_button_polling.h"

hw_button_polling::hw_button_polling(QObject *parent) :
    QObject(parent)
{

    // Setup the 4 HW button GPIO lines as inputs with pull-ups

    // First initialize the GPIO class
    gpio.gpioInitialize();

    // Set GPIO 27 to a input with a pull-up
    gpio.gpioSetMode(27, PI_INPUT);
    gpio.gpioSetPullUpDown(27, PI_PUD_UP);

    // Set GPIO 23 to a input with a pull-up
    gpio.gpioSetMode(23, PI_INPUT);
    gpio.gpioSetPullUpDown(23, PI_PUD_UP);

    // Set GPIO 22 to a input with a pull-up
    gpio.gpioSetMode(22, PI_INPUT);
    gpio.gpioSetPullUpDown(22, PI_PUD_UP);

    // Set GPIO 17 to a input with a pull-up
    gpio.gpioSetMode(17, PI_INPUT);
    gpio.gpioSetPullUpDown(17, PI_PUD_UP);
}



// Fuction to read the current value of the input buttons
void hw_button_polling::checkButtons()
{
    // myOut() << "Entered the button polling loop!";

    keys[1] = gpio.gpioRead(27);
    keys[2] = gpio.gpioRead(23);
    keys[3] = gpio.gpioRead(22);
    keys[4] = gpio.gpioRead(17);


    emit button_1_Changed(!keys[1]);
    emit button_2_Changed(!keys[2]);
    emit button_3_Changed(!keys[3]);
    emit button_4_Changed(!keys[4]);

}
