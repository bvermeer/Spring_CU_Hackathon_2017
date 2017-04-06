#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mcp3008Spi.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_next_Button_clicked();

    void on_prev_Button_clicked();

    void poll_ADC();

private:
    Ui::MainWindow *ui;

    // Object for the MCP3008 ADC class
    mcp3008Spi *adc;

    uint8_t chan_num = 0;

    double adc_voltage = 0;
};

#endif // MAINWINDOW_H
