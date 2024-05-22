#include <thread>
#include <iostream>

#include "aht20.h"

using namespace std::literals::chrono_literals;
using namespace std;

// //----- OPEN THE I2C BUS -----
// char *filename = (char *)"/dev/i2c-1";

/*--------------------------- Device Status ------------------------------*/
bool AHT20::begin()
{
    _deviceAddress = AHT20_DEFAULT_ADDRESS;

    isConnected(_deviceAddress);

    // Wait 40 ms after power-on before reading temp or humidity. Datasheet pg 8
    std::this_thread::sleep_for(40ms);

    // Check if the calibrated bit is set. If not, init the sensor.
    if (isCalibrated() == false)
    {
        // Send 0xBE0800
        initialize();
    }

    // Check that the cal bit has been set
    if (isCalibrated() == false)
        return false;

    // Mark all datums as fresh (not read before)
    sensorQueried.temperature = true;
    sensorQueried.humidity = true;

    return true;
}

/*------------------------ Measurement Helpers ---------------------------*/

uint8_t AHT20::getStatus()
{
    try
    {
        uint8_t buff[1];
        buff[0] = 0x71;
        writeByte(buff, 1);
        readByte(buff, 1);
        return buff[0];
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return (0);
    }
}

// Returns the state of the cal bit in the status byte
bool AHT20::isCalibrated()
{
    return (getStatus() & (1 << 3));
}

// Returns the state of the busy bit in the status byte
bool AHT20::isBusy()
{
    return (getStatus() & (1 << 7));
}

void AHT20::initialize()
{
    uint8_t buff[3];
    buff[0] = sfe_aht20_reg_initialize;
    buff[1] = 0x08;
    buff[2] = 0x00;
    writeByte(buff, 3);

    cout << "initialize" << endl;
}

void AHT20::triggerMeasurement()
{
    uint8_t buff[3];
    buff[0] = sfe_aht20_reg_measure;
    buff[1] = 0x33;
    buff[2] = 0x00;
    writeByte(buff, 3);

    cout << "triggerMeasurement" << endl;
}

// Loads the
void AHT20::readData()
{
    // Clear previous data
    sensorData.temperature = 0;
    sensorData.humidity = 0;

    uint8_t buff[6] = {0};
    readByte(buff, 6);

    uint32_t incoming = 0;
    incoming |= (uint32_t)buff[1] << (8 * 2);
    incoming |= (uint32_t)buff[2] << (8 * 1);
    uint8_t midByte = buff[3];

    incoming |= midByte;
    sensorData.humidity = incoming >> 4;

    sensorData.temperature = (uint32_t)midByte << (8 * 2);
    sensorData.temperature |= (uint32_t)buff[4] << (8 * 1);
    sensorData.temperature |= (uint32_t)buff[5] << (8 * 0);

    // Need to get rid of data in bits > 20
    sensorData.temperature = sensorData.temperature & ~(0xFFF00000);

    // Mark data as fresh
    sensorQueried.temperature = false;
    sensorQueried.humidity = false;
}

// Triggers a measurement if one has not been previously started, then returns false
// If measurement has been started, checks to see if complete.
// If not complete, returns false
// If complete, readData(), mark measurement as not started, return true
bool AHT20::available()
{
    if (measurementStarted == false)
    {
        triggerMeasurement();
        measurementStarted = true;
        return (false);
    }

    if (isBusy() == true)
    {
        return (false);
    }

    readData();
    measurementStarted = false;
    return (true);
}

/*------------------------- Make Measurements ----------------------------*/

float AHT20::getTemperature()
{
    if (sensorQueried.temperature == true)
    {
        // We've got old data so trigger new measurement
        triggerMeasurement();

        // Wait for measurement to complete
        std::this_thread::sleep_for(75ms);

        uint8_t counter = 0;
        while (isBusy())
        {
            std::this_thread::sleep_for(1ms);
            if (counter++ > 100)
                return (false); // Give up after 100ms
        }

        readData();
    }

    // From datasheet pg 8
    float tempCelsius = ((float)sensorData.temperature / 1048576) * 200 - 50;

    // Mark data as old
    sensorQueried.temperature = true;

    return tempCelsius;
}

float AHT20::getHumidity()
{
    if (sensorQueried.humidity == true)
    {
        // We've got old data so trigger new measurement
        triggerMeasurement();

        // Wait for measurement to complete
        std::this_thread::sleep_for(75ms);

        uint8_t counter = 0;
        while (isBusy())
        {
            std::this_thread::sleep_for(1ms);
            if (counter++ > 100)
                return (false); // Give up after 100ms
        }

        readData();
    }

    // From datasheet pg 8
    float relHumidity = ((float)sensorData.humidity / 1048576) * 100;

    // Mark data as old
    sensorQueried.humidity = true;

    return relHumidity;
}