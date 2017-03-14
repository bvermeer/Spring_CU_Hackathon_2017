#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>

#include <stdio.h>
#include <unistd.h>
#include <string.h> /* for strncpy */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_get_ip_button_clicked()
{
    int errorNum = 0;

    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    errorNum = ioctl(fd, SIOCGIFADDR, &ifr);

    if(errorNum == 0)
    {
        char label[] = "Ethernet: ";

        /* display result */
        ui->ethernet_address_text->setText(strcat(label, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr)));
    }
    else
    {
        ui->ethernet_address_text->setText("Ethernet: ");
    }


    ::close(fd);



    int fd2;
    struct ifreq ifr2;

    fd2 = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr2.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "wlan0" */
    strncpy(ifr2.ifr_name, "wlan0", IFNAMSIZ-1);

    errorNum = ioctl(fd2, SIOCGIFADDR, &ifr2);


    if(errorNum == 0)
    {
        char wifi_label[] = "WiFi: ";

        /* display result */
        ui->wifi_address_text->setText(strcat(wifi_label, inet_ntoa(((struct sockaddr_in *)&ifr2.ifr_addr)->sin_addr)));
    }
    else
    {
        /* display result */
        ui->wifi_address_text->setText("WiFi: ");
    }

    ::close(fd2);

}
