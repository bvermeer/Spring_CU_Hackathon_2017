#include "mcp3008Spi.h"
using namespace std;
/**********************************************************
 * spiOpen() :function is called by the constructor.
 * It is responsible for opening the spidev device
 * "devspi" and then setting up the spidev interface.
 * private member variables are used to configure spidev.
 * They must be set appropriately by constructor before calling
 * this function.
 * *********************************************************/
int mcp3008Spi::spiOpen(std::string devspi){
    int statusVal = -1;
    this->spifd = open(devspi.c_str(), O_RDWR);
    if(this->spifd < 0){
        perror("could not open SPI device");
        exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_WR_MODE, &(this->mode));
    if(statusVal < 0){
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_RD_MODE, &(this->mode));
    if(statusVal < 0) {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0) {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
    if(statusVal < 0) {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));
    if(statusVal < 0) {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    statusVal = ioctl (this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));
    if(statusVal < 0) {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }
    return statusVal;
}

/***********************************************************
 * spiClose(): Responsible for closing the spidev interface.
 * Called in destructor
 * *********************************************************/

int mcp3008Spi::spiClose(){
    int statusVal = -1;
    statusVal = close(this->spifd);
        if(statusVal < 0) {
      perror("Could not close SPI device");
      exit(1);
    }
    return statusVal;
}

/********************************************************************
 * This function writes data "data" of length "length" to the spidev
 * device. Data shifted in from the spidev device is saved back into
 * "data".
 * ******************************************************************/
int mcp3008Spi::spiWriteRead( unsigned char *data, int length){

  struct spi_ioc_transfer spi[length];
  int i = 0;
  int retVal = -1;

// one spi transfer for each byte

  for (i = 0 ; i < length ; i++){

    spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
    spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
    spi[i].len           = sizeof(*(data + i)) ;
    spi[i].delay_usecs   = 0 ;
    spi[i].speed_hz      = this->speed ;
    spi[i].bits_per_word = this->bitsPerWord ;
    spi[i].cs_change = 0;
    spi[i].pad = 0;
    spi[i].tx_nbits = 0;
    spi[i].rx_nbits = 0;
}

 retVal = ioctl (this->spifd, SPI_IOC_MESSAGE(length), &spi) ;

 if(retVal < 0){
    perror("Problem transmitting spi data..ioctl");
    exit(1);
 }

return retVal;

}

/*************************************************
 * Default constructor. Set member variables to
 * default values and then call spiOpen()
 * ***********************************************/

mcp3008Spi::mcp3008Spi(){
    this->mode = SPI_MODE_0 ;
    this->bitsPerWord = 8;
    this->speed = 1000000;
    this->spifd = -1;
    this->vref  = 3.3;

    this->spiOpen(std::string("/dev/spidev1.0"));

    }

/*************************************************
 * overloaded constructor. let user set member variables to
 * and then call spiOpen()
 * ***********************************************/
mcp3008Spi::mcp3008Spi(std::string devspi, unsigned char spiMode, unsigned int spiSpeed, unsigned char spibitsPerWord, double vref){
    this->mode = spiMode ;
    this->bitsPerWord = spibitsPerWord;
    this->speed = spiSpeed;
    this->spifd = -1;
    this->vref = vref;

    this->spiOpen(devspi);

}


/*************************************************
 * This function takes in a parameter of chanNum which
 * can be a value of 0 to 7 and returns the voltage
 * of that channel. If the channel number isn't between
 * 0 and 7, a value of -1 is returned.
 * ***********************************************/
double mcp3008Spi::readChannelVoltage(uint8_t chanNum)
{
   unsigned char data[3];

   // Make sure the channel number is valid
   if( chanNum > 7 )
   {
       return -1;
   }

   // Construct the command packet to send to the ADC
   data[0] = 1;  // The first byte transmitted is the start byte
   data[1] = 0b10000000 | ((chanNum & 7) << 4);  // The second byte transmitted -> (1 & chanNum = Make a single-ended measurement on the desire channel)
   data[2] = 0;  // Third byte transmitted is a don't care

   spiWriteRead(data, sizeof(data) );

   int rawADCVal = 0;
   rawADCVal = (data[1] << 8) & 0b1100000000; // Merge data[1] and data[2] to get the raw 10-bit ADC value
   rawADCVal |= (data[2] & 0xFF);

   // Convert the raw ADC value to a voltage using the provided value for vref
   return (vref * (double)rawADCVal) / (double)(1023);
}



/**********************************************
 * Destructor: calls spiClose()
 * ********************************************/
mcp3008Spi::~mcp3008Spi(){
    this->spiClose();
}
