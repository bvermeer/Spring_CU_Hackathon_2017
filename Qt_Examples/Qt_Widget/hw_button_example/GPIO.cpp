#include "GPIO.h"

bool GPIO::configureGPIO( GPIO_Pin gpionr, QString direction, QString configuration = GPIO_TRISTATE )
{
    // Check if an export file exists. If it doesn't the sysfs gpio module isn't loaded
    QFile gpioExport("/sys/class/gpio/export");
    if(!gpioExport.exists())
    {
        myErr() << "missiong file: /sys/class/gpio/export";
        return false;
    }


    // If the value file doesn't exist, then export it
    QFile gpioDirection( QString("/sys/class/gpio/gpio%1/direction").arg(gpionr) );
    if( !gpioDirection.exists() )
    {
        if( !gpioExport.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            myErr() << gpioExport.fileName();
            return false;
        }

        QTextStream out(&gpioExport);
        out << gpionr << "\n";
        gpioExport.close();
        usleep(1000);

    }


    if( !gpioDirection.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        myErr() << gpioDirection.fileName();
        return false;
    }

    QTextStream out(&gpioDirection);
    out << direction << "\n";
    gpioDirection.close();


    // Configure pullup, pulldown, or tristate on the pin
    myOut() << QString("gpio mode %1 %2").arg(gpionr).arg(configuration).toUtf8().constData();
    int errorCode = system( QString("gpio mode %1 %2").arg(gpionr).arg(configuration).toUtf8().constData() );

    if( errorCode != 0 )
    {
        myErr() << "Error while setting the GPIO direction.";
        return false;
    }


    return true;
}


bool GPIO::writeGPIO(GPIO_Pin gpionr, GPIO_State value)
{
    if(GPIO_UNDEF == value)
    {
        return false;
    }

    // Open and write 1 or 0 to the value file
    QFile gpioFile(QString("/sys/class/gpio/gpio%1/value").arg(gpionr) );

    if( !gpioFile.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        myErr() << "FAILURE can't open gpio file gpioPath:" << gpioFile.fileName();
        return false;
    }

    QTextStream out(&gpioFile);
    out << value << "\n";
    gpioFile.close();

    return true;

}


bool GPIO::readGPIO(GPIO_Pin gpionr, bool &value)
{
    // Open the value file of the pin to read
    QFile gpioFile(QString("/sys/class/gpio/gpio%1/value").arg(gpionr) );

    if( !gpioFile.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        myErr() << "FAILURE can't open gpio file gpioPath:" << gpioFile.fileName();
        return false;
    }

    QTextStream in (&gpioFile);
    QString readValue = in.readLine();

    if( readValue.compare("1") )
    {
        value = true;
    }
    else
    {
        value = false;
    }

    return true;

}
