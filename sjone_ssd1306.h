/*
 * sjone_ssd1306.h
 *
 * CMPE146: Fall 2016
 * Samuel Palomino
 * OLED (SSD1360) Display Driver
 * Design inspired by Adafruit's SSD1306 Driver:
 * 		https://github.com/adafruit/Adafruit_SSD1306
 */

#include <stdint.h>
#include "i2c2.hpp"
#include "printf_lib.h"			//debug print
#include "utilities.h"
#include "string.h"

#define	SSD1306_I2C_ADDR_8BIT	0x78		//8 bit device address on I2C bus
#define SSD1306_I2C_ADDR_7BIT	0x3C		//7 bit device address on I2C bus

#define	SSD1306_DISPLAY_WIDTH	128
#define	SSD1306_DISPLAY_HEIGHT	64

#define	SSD1306_EXTERNALVCC		0x1
#define	SSD1306_SWITCHAPVCC		0x2

//All commands obtained from SSD1360 datasheet
//Fundamental Commands
#define	SSD1306_SET_CONTRAST				0x81
#define	SSD1306_DISPLAY_ALLON_RESUME_RAM	0xA4
#define	SSD1306_DISPLAY_ALLON_IGNORE_RAM	0xA5
#define	SSD1306_SET_DISPLAY_NORMAL			0xA6
#define	SSD1306_SET_DISPLAY_INVERT			0xA7
#define	SSD1306_SET_DISPLAY_OFF				0xAE
#define	SSD1306_SET_DISPLAY_ON				0xAF

//Addressing Settings
#define	SSD1306_SET_LOW_COLUMN			0x00
#define	SSD1306_SET_HIGH_COLUMN			0x10
#define	SSD1306_SET_MEMORY_MODE			0x20
#define	SSD1306_SET_COL_ADDR			0x21
#define	SSD1306_SET_PAGE_ADDR			0x22
#define	SSD1306_SET_PAGE_START_ADDR		0xB0

//Hardware Configurations (Panel Resolution & Layout Related)
#define	SSD1306_SET_DISPLAY_START_LINE	0x40
#define	SSD1306_SET_SEGMENT_REMAP		0xA0
#define	SSD1306_SET_MULTIPLEX_RATIO		0xA8
#define	SSD1306_SET_COM_SCAN_DIR_INC	0xC0
#define SSD1306_SET_COM_SCAN_DIR_DEC	0xC8
#define	SSD1306_SET_DISPLAY_OFFSET		0xD3
#define	SSD1306_SET_COM_PINS			0xDA

//Timing & Driving Scheme Settings
#define	SSD1306_SET_DISPLAY_CLK_DIV		0xD5
#define	SSD1306_SET_PRECHARGE_PERIOD	0xD9
#define	SSD1306_SET_VCOM_DESELECT_LEVEL	0xDB
#define	SSD1306_NOP						0xE3

#define	SSD1306_SET_CHARGE_PUMP			0x8D

class OLEDModule{
	public:
		OLEDModule();

		void initDisplay(
			uint8_t switchvcc = SSD1306_SWITCHAPVCC,	//Set voltage state
			uint8_t i2caddr = SSD1306_I2C_ADDR_8BIT		//Set i2c device address
		);

		void sendCommand(uint8_t cmd);
		void sendData(uint8_t data);

		void showDisplay();
		void clearDisplay();
		void invertDisplay(bool foo);

		void prepareDisplayAddresses();
		void showAltDisplay(uint8_t choice);
		void sendBufferToDisplay(uint8_t *buff);

		void displayString(uint8_t page, uint8_t column, char *message);

	private:
		I2C2 i2c = I2C2::getInstance();
		int8_t _i2caddr;	//device address on I2C bus
		int8_t _vccstate;	//Operating voltage state (internal or external?)
};
