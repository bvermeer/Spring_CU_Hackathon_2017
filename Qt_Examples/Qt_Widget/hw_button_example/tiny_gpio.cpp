#include "tiny_gpio.h"

tiny_gpio::tiny_gpio()
{

}

/**
 * @brief Sets the operating mode for a GPIO pin.
 * @param gpio The GPIO pin number to set the mode on.
 * @param mode The mode to set the GPIO in to.
 */
void tiny_gpio::gpioSetMode(unsigned gpio, pin_mode mode)
{
    int reg, shift;

    reg 	= gpio/10;
    shift 	= (gpio%10) * 3;

    gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (mode<<shift);
}


/**
 * @brief Returns the currently set mode for the GPIO pin.
 * @param gpio The GPIO pin number of the pin to get the mode of.
 * @return Currently set GPIO mode for the pin.
 */
pin_mode tiny_gpio::gpioGetMode(unsigned gpio)
{
    int reg, shift;

    reg 	= gpio/10;
    shift 	= (gpio%10) * 3;

    return (pin_mode)((*(gpioReg + reg) >> shift) & 7);
}



/**
 * @brief Sets pull-up or pull-down resistor states on a pin.
 * @param gpio The GPIO pin number to change.
 * @param pud The pull-up or pull-down configuration to set.
 */
void tiny_gpio::gpioSetPullUpDown(unsigned gpio, pi_pud pud)
{
    *(gpioReg + GPPUD) = pud;

    usleep(20);

    *(gpioReg + GPPUDCLK0 + PI_BANK) = PI_BIT;

    usleep(20);

    *(gpioReg + GPPUD) = 0;

    *(gpioReg + GPPUDCLK0 + PI_BANK) = 0;
}



/**
 * @brief Read the current state of a GPIO pin.
 * @param gpio The GPIO pin to read.
 * @return The current state of the pin (1 = high, 0 = low).
 */
int tiny_gpio::gpioRead(unsigned gpio)
{
    if( (*(gpioReg + GPLEV0 + PI_BANK) & PI_BIT) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



/**
 * @brief Set the output state of a GPIO pin. The pin should
 * be configured as an output first before using this function!
 * @param gpio GPIO pin to set the output state on.
 * @param level Level to set the GPIO pin to (1 = high, 0 = low)
 */
void tiny_gpio::gpioWrite(unsigned gpio, unsigned level)
{
    if(level == 0)
    {
        *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
    }
    else
    {
        *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
    }
}



/**
 * @brief Creates a timing level transistion on a pin. The pin starts
 * at the value defined by level, then waits pulseLen microseconds, then
 * transistions to ~level.
 * @param gpio GPIO pin to create the trigger event on.
 * @param pulseLen Length of the pulse in microseconds.
 * @param level Starting level for the GPIO pin.
 */
void tiny_gpio::gpioTrigger(unsigned gpio, unsigned pulseLen, unsigned level)
{
    if(level == 0)
    {
        *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
    }
    else
    {
        *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
    }

    usleep(pulseLen);

    if(level != 0)
    {
        *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
    }
    else
    {
        *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
    }
}



/**
 * @brief Finds the Raspberry Pi model number and revision of the hardware
 */
unsigned tiny_gpio::gpioHardwareRevision()
{
   static unsigned rev = 0;

   FILE * filp;
   char buf[512];
   char term;
   int chars = 4; // Number of chars in the revision string

   if(rev != 0)
   {
       return rev;
   }

   piModel_ = 0;

   filp = fopen("/proc/cpuinfo", "r");

   if(filp != NULL)
   {
       while( fgets(buf, sizeof(buf), filp) != NULL)
       {
           if(piModel_ == 0)
           {
               if( !strncasecmp("model name", buf, 10))
               {
                   if( strstr(buf, "ARMv6") != NULL)
                   {
                       piModel_ = 1;
                       chars = 4;
                   }
                   else if( strstr(buf, "ARMv7") != NULL)
                   {
                       piModel_ = 2;
                       chars = 6;
                   }
                   else if( strstr(buf, "ARMv8") != NULL)
                   {
                       piModel_ = 2;
                       chars = 6;
                   }
               }
           }

           if( !strncasecmp("revision", buf, 8))
           {
               if( sscanf(buf+strlen(buf) - (chars+1), "%x%c", &rev, &term) == 2)
               {
                  if(term != '\n')
                  {
                      rev = 0;
                  }
               }
           }

       }

       fclose(filp);

   }

   return rev;
}




/**
 * @brief Sets up the GPIO to be used.
 * @return Returns 0 if passed, -1 if failed
 */
int tiny_gpio::gpioInitialize()
{
    int fd;

    piRev_ = gpioHardwareRevision();  // Sets piModel and piRev

    fd = open("/dev/gpiomem", O_RDWR | O_SYNC);

    if(fd < 0)
    {
        fprintf(stderr, "Failed to open /dev/gpiomem\n");
        return -1;
    }

    gpioReg = (uint32_t *)mmap(NULL, 0xB4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    close(fd);

    if(gpioReg == MAP_FAILED)
    {
        fprintf(stderr, "Bad, mmap failed\n");
        return -1;
    }

    return 0;
}



// Bit (1<<x) will be set if GPIO is high
uint32_t tiny_gpio::gpioReadBank1() {
    return (*(gpioReg + GPLEV0));
}

// Bit (1<<x) will be set if GPIO is high
uint32_t tiny_gpio::gpioReadBank2() {
    return (*(gpioReg + GPLEV1));
}

// To clear GPIO x bit or in (1<<x)
void tiny_gpio::gpioClearBank1(uint32_t bits)
{
    *(gpioReg + GPCLR0) = bits;
}

// To clear GPIO x bit or in (1<<x)
void tiny_gpio::gpioClearBank2(uint32_t bits)
{
    *(gpioReg + GPCLR1) = bits;
}

// To set GPIO x bit or in (1<<x)
void tiny_gpio::gpioSetBank1(uint32_t bits)
{
    *(gpioReg + GPSET0) = bits;
}

// To set GPIO x bit or in (1<<x)
void tiny_gpio::gpioSetBank2(uint32_t bits)
{
    *(gpioReg + GPSET1) = bits;
}
