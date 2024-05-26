#ifndef __SSD1306_H__
#define __SSD1306_H__

#include "i2c.h"

// --- Define constants ----
// infos can be found in SSD1306 datasheet
// ----------------------------------------------
// commands
#define SSD1306_DISPLAYON 0xAF
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_MULTIPLEX_RATIO 0xA8

// registers
#define SSD1306_CONTROL_REG 0x00 // Co = 0, DC = 0
#define SSD1306_DATA_REG 0x40
// ----------------------------------------------

class SSD1306 : public I2C
{
public:
    // --- Define functions ---
    // ----------------------------------------------
    void ssd1306I2CSetup(uint8_t i2cAddr);

    // send command byte to SSD1306 via I2C
    void sendCommand(uint8_t command);
    // send data byte to SSD1306 via I2C
    void sendData(uint8_t data);

    // OLED operations
    void displayOn();    // turn on the display
    void displayOff();   // turn off the display
    void clearDisplay(); // clear the contents on display

    // Attention: The draw and draw_line functions only change the contents in local data frame
    //            To update the contents on display should function updateDisplay() be used

    /**
     * @brief draw one ascii code at determined position
     *
     * @param x: 1 to 4
     * @param y: 1 to 16
     * @param ascii: one ascii code
     */
    void draw(int x, int y, unsigned char ascii);

    /**
     * @brief draw one line of ascii codes at determined position on display
     *
     * @param x: 1 to 4
     * @param y: 1 to 16
     * @param ascii_str[16]: the number of ascii codes can not more than 16
     */
    void draw_line(int x, int y, unsigned char ascii_str[16]);

    /**
     * @brief write the frame data to SSD1306 to update display
     *
     * @param fd: file descriptor
     */
    void updateDisplay();
    // ----------------------------------------------
};

#endif