//ADS1115 4 channel ADC
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>    // read/write usleep      
#include <stdlib.h>    // exit function
#include <inttypes.h>  // uint8_t, etc
#include <linux/i2c-dev.h> // I2C bus definitions
#include <sys/ioctl.h>			//Needed for I2C port
#include <math.h>
#include "/home/pi/glg/include/GlgApi.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "BSP.h"
#include <stdbool.h>

void ADC_Read(int channel_num)
{
   float My_Value;
       // open device on /dev/i2c the default on Raspberry Pi B
    if(DEBUG1 == 1)
        printf("in ADC_Read\n");
    if ((fd_adc = open("/dev/i2c-1", O_RDWR)) < 0) 
    {
        printf("Error: Couldn't open ADC device! %d\n", fd_adc);
        if (DEBUG ==True)
            printf("Failed to open ADC\n");
		exit(1);
    }
  
    delay(250);
	 if(DEBUG1 == True)
		printf("connect to ads1115\n");  // connect to ADS1115 as i2c slave
    if (ioctl(fd_adc, I2C_SLAVE, adc_address) < 0) 
    {
       printf("Error: Couldn't find device on address!\n");
       exit (1);
    }
      
    delay(250);
    AD_Buf[0] = 1; // config register is 1
    AD_Buf[2] = 0b10000000; // bits 7-0  0x85

    switch(channel_num)
    {
      case 0:
      {
         AD_Buf[1] = 0b11000011; // 0xC2 single shot
         break;
      }
      case 1:
      {
         AD_Buf[1] = 0b11010011; // 0xC2 single shot
         break;
      }
      case 2:
      {
         AD_Buf[1] = 0b11100011; // 0xC2 single shot
         break;
      }
      case 3:
      {
         AD_Buf[1] = 0b11110011; // 0xC2 single shot
         break;
      }
   }
    if (write(fd_adc, AD_Buf, 3) != 3) 
    {
       perror("Write to register 1");
       exit (1);
    }
      
    delay(250);      
    readBuf[0] = 0;
    if (write(fd_adc, readBuf, 1) != 1) 
    {
      perror("Write register select");
      exit(-1); 
   }    
    delay(250);
      
    if (read(fd_adc, readBuf, 2) != 2) 
    {
         perror("Read conversion");
         exit(-1);
    }
      
    delay(250); 
      
          // could also multiply by 256 then add readBuf[1]
 //   val = readBuf[0] << 8 | readBuf[1];
    val = readBuf[0]*256 + readBuf[1];
    if(DEBUG == True)
		printf("ADC value = %.1f\n", val); 
      
          // with +- LSB sometimes generates very low neg number.
    if (val < 0)
        val = 0;
      
    ADC_Value = (float)(val/100)/2.5;// * VPS; // convert to 0-100%  
    close(fd_adc); 
    delay(250);
    return;
}   

