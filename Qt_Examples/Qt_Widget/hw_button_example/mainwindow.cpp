#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DebugDefines.h"
#include "gpio_raspberrypi3.h"
#include "GPIO.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myOut() << "Testing setting up and reading the GPIO";

    GPIO gpioInst;

    if(!gpioInst.configureGPIO(GPIO_Pin15, GPIO_DIRECTION_INPUT, GPIO_PULLUP)) { myErr() << "config failed..."; }


    bool pinState = false;

    for( int i = 0; i < 30; i++)
    {
        if( !gpioInst.readGPIO(GPIO_Pin15, pinState) )
        {
            myErr() << "Error reading pin!";
        }
        else
        {
            myOut() << QString("Pin value: %1").arg(pinState);
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
