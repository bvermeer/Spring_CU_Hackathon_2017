/**********************************************************
 * Project Name: MCP3008_ADC_Example
 *
 * Description: This is a simple example program of how
 * to read voltages from the MCP3008 ADC. The GUI allows
 * you to select the channel to read and a timer event
 * causes the ADC value to be read and displayed every
 * 100 ms.
 * *********************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the inital text value for the channel text label
    chan_num = 0;
    ui->chan_Label->setText( QString("CH: %1").arg(chan_num) );

    // Create a new mcp3008Spi object to talk to an ADC on SPI bus 1 using CS line 0. Communication speed is 1MHz, 8 bits per word, and a Vref voltage of 3.3V
    adc = new mcp3008Spi("/dev/spidev1.0", SPI_MODE_0, 1000000, 8, 3.3);

    // Poll the ADC value every 100ms
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(poll_ADC()) );
    timer->start(100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Advance the current channel number and update the channel label
void MainWindow::on_next_Button_clicked()
{
    if(chan_num == 7)
    {
        chan_num = 0;
    }
    else
    {
        chan_num++;
    }

    ui->chan_Label->setText( QString("CH: %1").arg(chan_num) );
}

// Decrement the current channel number and update the channel label
void MainWindow::on_prev_Button_clicked()
{
    if(chan_num == 0)
    {
        chan_num = 7;
    }
    else
    {
        chan_num--;
    }

    ui->chan_Label->setText( QString("CH: %1").arg(chan_num) );
}

// Read the new ADC value and update the LCD display widget
void MainWindow::poll_ADC()
{
    // Poll the voltage on the selected channel
    adc_voltage = adc->readChannelVoltage(chan_num);

    // Update the LCD display widget with the new value
    ui->lcdNumber->display(adc_voltage);
}
