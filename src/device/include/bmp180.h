#ifndef __bmp180_H__
#define __bmp180_H__

#include "i2c.h"

#define BMP180_DEFAULT_ADDRESS 0x77

enum calibration
{
    bmp180_cal_ac1 = 0xAA,
    bmp180_cal_ac2 = 0xAC,
    bmp180_cal_ac3 = 0xAE,
    bmp180_cal_ac4 = 0xB0,
    bmp180_cal_ac5 = 0xB2,
    bmp180_cal_ac6 = 0xB4,
    bmp180_cal_b1 = 0xB6,
    bmp180_cal_b2 = 0xB8,
    bmp180_cal_mb = 0xBA,
    bmp180_cal_mc = 0xBC,
    bmp180_cal_md = 0xBE,
};

enum register1s
{
    bmp180_reg_cal = 0xAA,
    bmp180_control = 0xF4,           // Control register
    bmp180_get_raw_data = 0xF6,      // Get raw data MSB, LSB(0xF7), XLSB(0xF8)
    bmp180_read_temp_cmd = 0x2E,     // Read temperature control register value
    bmp180_read_pressure_cmd = 0x34, // Read pressure control register value
};

enum oss
{
    bmp180_ultra_low = 0,  // Ultra low power mode
    bmp180_standard = 1,   // Standard mode
    bmp180_high = 2,       // High-res mode
    bmp180_ultra_high = 3, // Ultra high-res mode
};

class BMP180 : public I2C
{
private:
    struct
    {
        long up;
        long ut;
    } sensorData;

public:
    int fd;
    uint8_t oss_mode;

    int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
    uint16_t ac4, ac5, ac6;

    BMP180(uint8_t mode)
    {
        oss_mode = mode;
    }

    void setup();
    void getCalParam();
    void getUT();
    void getUP();
    long computeB5();
    long getTemperature(); // temp in 0.1 C
    long getPressure();    // press in Pa
};
#endif