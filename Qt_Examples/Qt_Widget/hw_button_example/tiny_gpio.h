#ifndef TINY_GPIO_H
#define TINY_GPIO_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#define GPSET0 7
#define GPSET1 8

#define GPCLR0 10
#define GPCLR1 11

#define GPLEV0 13
#define GPLEV1 14

#define GPPUD 		37
#define GPPUDCLK0 	38
#define GPPUDCLK1 	39

#define PI_BANK (gpio>>5)
#define PI_BIT 	(1<<(gpio&0x1F))

// GPIO modes
enum pin_mode {
    PI_INPUT 	= 0,
    PI_OUTPUT	= 1,
    PI_ALT0		= 4,
    PI_ALT1 	= 5,
    PI_ALT2		= 6,
    PI_ALT3 	= 7,
    PI_ALT4 	= 3,
    PI_ALT5 	= 2
};


/* Values for pull-ups/downs off, pull-down and pull-up. */
enum pi_pud {
    PI_PUD_OFF  = 0,
    PI_PUD_DOWN = 1,
    PI_PUD_UP   = 2
};



class tiny_gpio
{
   volatile uint32_t *gpioReg = (uint32_t *)MAP_FAILED;

   unsigned piModel_;
   unsigned piRev_;


public:

    // Read-only public variables for the model and revision of the Raspberry Pi
    unsigned piModel() const {return piModel_; }
    unsigned piRev() const {return piRev_; }

    tiny_gpio();

    void gpioSetMode(unsigned gpio, pin_mode mode);

    pin_mode gpioGetMode(unsigned gpio);

    void gpioSetPullUpDown(unsigned gpio, pi_pud pud);

    int gpioRead(unsigned gpio);

    void gpioWrite(unsigned gpio, unsigned level);

    void gpioTrigger(unsigned gpio, unsigned pulseLen, unsigned level);

    unsigned gpioHardwareRevision();

    int gpioInitialize();

private:
    uint32_t gpioReadBank1(void);
    uint32_t gpioReadBank2(void);

    void gpioClearBank1(uint32_t bits);
    void gpioClearBank2(uint32_t bits);

    void gpioSetBank1(uint32_t bits);
    void gpioSetBank2(uint32_t bits);

};

#endif // TINY_GPIO_H
