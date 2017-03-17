#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "DebugDefines.h"
#include "gpio_raspberrypi3.h"
#include "hw_button_polling.h"
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myOut() << "Testing setting up and reading the GPIO";

    ui->button1_checkBox->setChecked(true);

    hw_button_polling *buttonPoller = new hw_button_polling(this);

    // Connection the button change signals to the UI symbols to change
    connect(buttonPoller, SIGNAL(button_1_Changed(bool)), ui->button1_checkBox, SLOT(setChecked(bool)) );
    connect(buttonPoller, SIGNAL(button_2_Changed(bool)), ui->button2_checkBox, SLOT(setChecked(bool)) );
    connect(buttonPoller, SIGNAL(button_3_Changed(bool)), ui->button3_checkBox, SLOT(setChecked(bool)) );
    connect(buttonPoller, SIGNAL(button_4_Changed(bool)), ui->button4_checkBox, SLOT(setChecked(bool)) );



    // Poll the HW buttons every 100ms
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), buttonPoller, SLOT(checkButtons()) );
    timer->start(100);


}

MainWindow::~MainWindow()
{
    delete ui;
}
