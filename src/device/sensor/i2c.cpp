#include <system_error>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "i2c.h"

void I2C::connect(uint8_t _deviceAddress)
{
    fd = open(DEVICE, O_RDWR);
    if (fd < 0)
    {
        throw std::system_error(errno,
                                std::system_category(),
                                "Failed to open RTC device");
    }
    if (ioctl(fd, I2C_SLAVE, _deviceAddress) < 0)
    {
        close(fd);
        throw std::system_error(errno,
                                std::system_category(),
                                "Failed to aquire bus address");
    }
}

void I2C::writeByte(uint8_t *buff, uint8_t len)
{
    if (write(fd, buff, len) != len)
    {
        throw std::system_error(errno,
                                std::system_category(),
                                "Write to i2c device failed");
    }
}

void I2C::readByte(uint8_t *buff, uint8_t len)
{
    if (read(fd, buff, len) != len)
    {
        throw std::system_error(errno,
                                std::system_category(),
                                "read to i2c device failed");
    }
}