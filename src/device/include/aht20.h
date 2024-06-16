
#ifndef __aht20_H__
#define __aht20_H__

#include "i2c.h"

#define AHT20_DEFAULT_ADDRESS 0x38

enum registers
{
    sfe_aht20_reg_reset = 0xBA,
    sfe_aht20_reg_initialize = 0xBE,
    sfe_aht20_reg_measure = 0xAC,
};

class AHT20 : public I2C
{
private:
    uint8_t _deviceAddress;
    bool measurementStarted = false;

    struct
    {
        uint32_t humidity;
        uint32_t temperature;
    } sensorData;

    struct
    {
        uint8_t temperature : 1;
        uint8_t humidity : 1;
    } sensorQueried;

public:
    // int fd;

    // Device status
    bool begin();       // Sets the address of the device and opens the I2C bus
    bool available();   // Returns true if new data is available

    // Measurement helper functions
    uint8_t getStatus();       // Returns the status byte
    bool isCalibrated();       // Returns true if the cal bit is set, false otherwise
    bool isBusy();             // Returns true if the busy bit is set, false otherwise
    void initialize();         // Initialize for taking measurement
    void triggerMeasurement(); // Trigger the AHT20 to take a measurement
    void readData();           // Read and parse the 6 bytes of data into raw humidity and temp
    // bool softReset();          //Restart the sensor system without turning power off and on

    //Make measurements
    float getTemperature(); // Goes through the measurement sequence and returns temperature in degrees celcius
    float getHumidity();    // Goes through the measurement sequence and returns humidity in % RH
};
#endif