#include <thread>
#include <iostream>

#include "bmp180.h"
#include <cmath>

using namespace std::literals::chrono_literals;
using namespace std;

void BMP180::setup()
{
    connect(BMP180_DEFAULT_ADDRESS);
};

void BMP180::getCalParam()
{
    uint8_t buff[22] = {0};
    buff[0] = bmp180_cal_ac1;
    writeByte(buff, 1);
    readByte(buff, 22);

    ac1 = (buff[0] << 8) | buff[1];
    ac2 = (buff[2] << 8) | buff[3];
    ac3 = (buff[4] << 8) | buff[5];
    ac4 = (buff[6] << 8) | buff[7];
    ac5 = (buff[8] << 8) | buff[9];
    ac6 = (buff[10] << 8) | buff[11];
    b1 = (buff[12] << 8) | buff[13];
    b2 = (buff[14] << 8) | buff[15];
    mb = (buff[16] << 8) | buff[17];
    mc = (buff[18] << 8) | buff[19];
    md = (buff[20] << 8) | buff[21];
};

void BMP180::getUT()
{
    uint8_t buff[] = {bmp180_control, bmp180_read_temp_cmd};
    writeByte(buff, 2);

    std::this_thread::sleep_for(4.5ms);

    uint8_t buff_raw[2] = {0};
    buff_raw[0] = bmp180_get_raw_data;
    writeByte(buff_raw, 1);
    readByte(buff_raw, 2);

    sensorData.ut = (buff_raw[0] << 8) + buff_raw[1];
}

void BMP180::getUP()
{
    uint8_t buff[] = {bmp180_control, bmp180_read_pressure_cmd + (oss_mode << 6)};
    writeByte(buff, 2);

    /* Delay correctly dependant oss mode, according datasheet (see in page 21) */
    switch (oss_mode)
    {
    case bmp180_ultra_low:
        std::this_thread::sleep_for(4.5ms);
        break;

    case bmp180_standard:
        std::this_thread::sleep_for(7.5ms);
        break;

    case bmp180_high:
        std::this_thread::sleep_for(13.5ms);
        break;

    case bmp180_ultra_high:
        std::this_thread::sleep_for(25.5ms);
        break;
    }

    uint8_t buff_raw[3] = {0};
    buff_raw[0] = bmp180_get_raw_data;
    writeByte(buff_raw, 1);
    readByte(buff_raw, 3);

    sensorData.up = ((buff_raw[0] << 16) + (buff_raw[1] << 8) + buff_raw[2]) >> (8 - oss_mode);
}

/* Calculation temperature, according datasheet in page 15 */
long BMP180::computeB5()
{
    long x1 = ((sensorData.ut - ac6) * ac5) >> 15;
    long x2 = (mc << 11) / (x1 + md);
    return x1 + x2;
}

long BMP180::getTemperature()
{
    return (computeB5() + 8) >> 4;
}

long BMP180::getPressure()
{
    long b6 = computeB5() - 4000;
    long x1 = (b2 * ((b6 * b6) >> 12)) >> 11;
    long x2 = (ac2 * b6) >> 11;
    long x3 = x1 + x2;
    long b3 = (((ac1 * 4 + x3) << oss_mode) + 2) / 4;
    x1 = (ac3 * b6) >> 13;
    x2 = (b1 * ((b6 * b6) >> 12)) >> 16;
    x3 = ((x1 + x2) + 2) / 4;
    uint32_t b4 = ac4 * (uint32_t)(x3 + 32768) >> 15;
    uint32_t b7 = ((uint32_t)sensorData.up - b3) * (50000 >> oss_mode);
    long p = 0;
    if (b7 < 0x80000000)
    {
        p = (b7 * 2) / b4;
    }
    else
    {
        p = (b7 / b4) * 2;
    }
    x1 = (p >> 8) * (p >> 8);
    x1 = (x1 * 3038) >> 16;
    x2 = (-7357 * p) >> 16;
    p = p + ((x1 + x2 + 3791) >> 4);
    return p;
}
