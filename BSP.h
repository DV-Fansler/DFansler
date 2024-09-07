extern float ADC_Value;
extern void ADC_Read(int channel_num);  
extern int fd_adc;
extern double val;
extern char AD_Buf[];
extern char readBuf[];
extern float myfloat;
extern int percent;
extern int adc_address;
extern int channel_number;
extern int Run_Value;

#define DEBUG True
#define DEBUG1 False


// REGISTER ADDRESSES
#define MODE1 0x00      /**< Mode Register 1 */
#define MODE2 0x01      /**< Mode Register 2 */
#define SUBADR1 0x02    /**< I2C-bus subaddress 1 */
#define SUBADR2 0x03    /**< I2C-bus subaddress 2 */
#define SUBADR3 0x04    /**< I2C-bus subaddress 3 */
#define ALLCALLADR 0x05 /**< LED All Call I2C-bus address */
#define LED0_ON_L 0x06  /**< LED0 on tick, low byte*/
#define LED0_ON_H 0x07  /**< LED0 on tick, high byte*/
#define LED0_OFF_L 0x08 /**< LED0 off tick, low byte */
#define LED0_OFF_H 0x09 /**< LED0 off tick, high byte */
// etc all 16:  LED15_OFF_H 0x45
#define ALLLED_ON_L 0xFA  /**< load all the LEDn_ON registers, low */
#define ALLLED_ON_H 0xFB  /**< load all the LEDn_ON registers, high */
#define ALLLED_OFF_L 0xFC /**< load all the LEDn_OFF registers, low */
#define ALLLED_OFF_H 0xFD /**< load all the LEDn_OFF registers,high */
#define PRESCALE    0xFE  /**< prescale_PCA9685r for PWM output frequency */
#define TESTMODE 0xFF     /**< defines the test mode to be entered */

// MODE1 bits
#define MODE1_ALLCAL 0x01  /**< respond to LED All Call I2C-bus address */
#define MODE1_SUB3 0x02    /**< respond to I2C-bus subaddress 3 */
#define MODE1_SUB2 0x04    /**< respond to I2C-bus subaddress 2 */
#define MODE1_SUB1 0x08    /**< respond to I2C-bus subaddress 1 */
#define MODE1_SLEEP 0x10   /**< Low power mode. Oscillator off */
#define MODE1_AI 0x20      /**< Auto-Increment enabled */
#define MODE1_EXTCLK 0x40  /**< Use EXTCLK pin clock */
#define MODE1_RESTART 0x80 /**< Restart enabled */
// MODE2 bits
#define MODE2_OUTNE_0 0x01 /**< Active LOW output enable input */
#define MODE2_OUTNE_1                                                          \
  0x02 /**< Active LOW output enable input - high impedience */
#define MODE2_OUTDRV 0x04 /**< totem pole structure vs open-drain */
#define MODE2_OCH 0x08    /**< Outputs change on ACK vs STOP */
#define MODE2_INVRT 0x10  /**< Output logic state inverted */

#define I2C_ADDRESS 0x40      /**< Default PCA9685 I2C Slave Address */
#define FREQUENCY_OSCILLATOR 25000000 /**< Int. osc. frequency in datasheet */

#define PRESCALE_MIN 3   /**< minimum prescale_PCA9685value */
#define PRESCALE_MAX 255 /**< maximum prescale_PCA9685_PCA9685value */


#define min(a,b) (((a) < (b)) ? (a) : (b))
void delay(int dtime);
void pca9685_init(void);
void turn_output_off( int channel);
void turn_output_on( int channel);
//void set_iic_add(void);
void set_iic_add_1(void);
/* LEDX_ON_H bits */
#define PCA9685_LED_ON 0x10
/* LEDX_OFF_H bits */
#define PCA9685_LED_OFF 0x10
extern long PWMData[];
typedef unsigned char   u8;
int i2c_init(void);
void i2c_close(void);
int i2c_write(u8 slave_addr, u8 reg, u8 data);
int i2c_read(u8 slave_addr, u8 reg, u8 *result);
