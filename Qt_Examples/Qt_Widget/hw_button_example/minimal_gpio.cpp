#include "minimal_gpio.h"


minimal_gpio::minimal_gpio()
{

}

void minimal_gpio::gpioSetMode(unsigned gpio, unsigned mode)
{
    int reg, shift;

    reg 	= gpio/10;
    shift 	= (gpio%10) * 3;

    gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (mode<<shift);
}


int minimal_gpio::gpioGetMode(unsigned gpio)
{
    int reg, shift;

    reg 	= gpio/10;
    shift 	= (gpio%10) * 3;

    return (*(gpioReg + reg) >> shift) & 7;
}


void minimal_gpio::gpioSetPullUpDown(unsigned gpio, unsigned pud)
{
    *(gpioReg + GPPUD) = pud;

    usleep(20);

    *(gpioReg + GPPUDCLK0 + PI_BANK) = PI_BIT;

    usleep(20);

    *(gpioReg + GPPUD) = 0;

    *(gpioReg + GPPUDCLK0 + PI_BANK) = 0;
}


int minimal_gpio::gpioRead(unsigned gpio)
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


void minimal_gpio::gpioWrite(unsigned gpio, unsigned level)
{
    if( level == 0 )
    {
        *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
    }
    else
    {
        *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
    }
}


void minimal_gpio::gpioTrigger(unsigned gpio, unsigned pulseLen, unsigned level)
{
    if( level == 0 )
    {
        *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
    }
    else
    {
        *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
    }

    usleep(pulseLen);

    if( level == 0 )
    {
        *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
    }
    else
    {
        *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
    }
}


uint32_t minimal_gpio::gpioReadBank1()
{
    return (*(gpioReg + GPLEV0));
}


uint32_t minimal_gpio::gpioReadBank2()
{
    return (*(gpioReg + GPLEV1));
}


void minimal_gpio::gpioClearBank1(uint32_t bits)
{
    *(gpioReg + GPCLR0) = bits;
}


void minimal_gpio::gpioClearBank2(uint32_t bits)
{
    *(gpioReg + GPCLR1) = bits;
}


void minimal_gpio::gpioSetBank1(uint32_t bits)
{
    *(gpioReg + GPSET0) = bits;
}


void minimal_gpio::gpioSetBank2(uint32_t bits)
{
    *(gpioReg + GPSET1) = bits;
}


unsigned minimal_gpio::gpioHardwareRevision()
{
    static unsigned rev = 0;

    FILE * filp;
    char buf[512];
    char term;
    int chars = 4;  // Number of chars in revision string

    if(rev)
    {
        return rev;
    }

    piModel = 0;

    filp = fopen("/proc/cpuinfo", "r");

    if(filp != NULL)
    {
        while( fgets(buf, sizeof(buf), filp) != NULL )
        {
            if(piModel == 0)
            {
                if( !strncasecmp("model name", buf, 10) )
                {
                    if( strstr(buf, "ARMv6") != NULL )
                    {
                        piModel = 1;
                        chars = 4;
                        piPeriphBase = 0x20000000;
                        piBusAddr = 0x40000000;
                    }
                    else if( strstr(buf, "ARMv7") != NULL )
                    {
                       piModel = 2;
                       chars = 6;
                       piPeriphBase = 0x3F000000;
                       piBusAddr = 0xC0000000;
                    }
                    else if( strstr(buf, "ARMv8") != NULL )
                    {
                        piModel = 2;
                        chars = 6;
                        piPeriphBase = 0x3F000000;
                        piBusAddr = 0xC0000000;
                    }
                }
            }

            if( !strncasecmp("revision", buf, 8) )
            {
                if( sscanf(buf+strlen(buf)-(chars+1),
                           "%x%c", &rev, &term) == 2)
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


// Returns the number of microseconds after system boot.
// Wraps around after 1 hour, 11 minutes, 35 seconds.
uint32_t minimal_gpio::gpioTick()
{
    return systReg[SYST_CLO];
}


// Map in registers
uint32_t * minimal_gpio::initMapMem(int fd, uint32_t addr, uint32_t len)
{
    return (uint32_t *) mmap(0, len,
                             PROT_READ|PROT_WRITE|PROT_EXEC,
                             MAP_SHARED|MAP_LOCKED,
                             fd, addr);
}


int minimal_gpio::gpioInitialize()
{
    int fd;

    gpioHardwareRevision();  // Sets piModel, needed for peripherals address

    fd = open("/dev/mem", O_RDWR | O_SYNC);

    if( fd < 0 )
    {
        fprintf(stderr, "This program needs root privileges. Try using sudo\n");
        return -1;
    }

    gpioReg = initMapMem(fd, GPIO_BASE, GPIO_LEN);
    systReg = initMapMem(fd, SYST_BASE, SYST_LEN);
    bscsReg = initMapMem(fd, BSCS_BASE, BSCS_LEN);

    close(fd);

    if( (gpioReg == MAP_FAILED) ||
        (systReg == MAP_FAILED) ||
        (bscsReg == MAP_FAILED))
    {
        fprintf(stderr, "Bad, mmap failed\n");
        return -1;
    }

    return 0;
}
