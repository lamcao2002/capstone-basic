#ifndef __i2c_H__
#define __i2c_H__

#define DEVICE "/dev/i2c-1"

class I2C
{
public: 
    int fd;
    void connect(uint8_t _deviceAddress);
    void writeByte(uint8_t *buf, uint8_t len);
    void readByte(uint8_t *buf, uint8_t len);
};
#endif