#ifndef GPIO_H
#define GPIO_H

#include <QString>

#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "DebugDefines.h"
#include "gpio_raspberrypi3.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock


static QString GPIO_DIRECTION_INPUT("in");      // sysfs gpio config direction string (in)
static QString GPIO_DIRECTION_OUTPUT("out");    // sysfs gpio config direction string (out)

static QString GPIO_PULLUP("up"); 	// Configuration string for pullup with GPIO utility
static QString GPIO_PULLDOWN("down"); 	// Configuration string for pullup with GPIO utility
static QString GPIO_TRISTATE("tri"); 	// Configuration string for pullup with GPIO utility


typedef enum GPIO_StateName
{
    GPIO_LOW    = 0,
    GPIO_HIGH   = 1,
    GPIO_UNDEF

} GPIO_State;



class GPIO
{
    private:

    public:
        bool configureGPIO(GPIO_Pin gpionr, QString direction, QString configuration );

        bool writeGPIO(GPIO_Pin gpionr, GPIO_State value);

        bool readGPIO(GPIO_Pin gpionr, bool &value);
};

#endif // GPIO_H
