#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <condition_variable>

#include "aht20.h"   // Include your AHT20 sensor library
#include "bmp180.h"  // Include your BMP180 sensor library
#include "ssd1306.h" // Include your SSD1306 OLED display library

struct SensorData
{
    float temperature;
    float humidity;
    long pressure;
};

std::mutex dataMutex;
std::condition_variable dataCondVar;
SensorData sensorData;
bool newDataAvailable = false;

void readSensors()
{
    AHT20 aht20;
    BMP180 bmp180((uint8_t)bmp180_ultra_low);

    // Initialize sensors
    bmp180.setup();
    bmp180.getCalParam();

    if (!aht20.begin())
    {
        std::cerr << "Failed to initialize sensors!" << std::endl;
        return;
    }

    while (true)
    {
        {
            // Read data from sensors
            std::lock_guard<std::mutex> lock(dataMutex);
            bmp180.getUT();
            bmp180.getUP();
            sensorData.temperature = (float)bmp180.getTemperature() / 10.0;
            sensorData.pressure = bmp180.getPressure();

            // If a new measurement is available
            if (aht20.available() == true)
            {
                sensorData.humidity = aht20.getHumidity();
            }

            newDataAvailable = true;
        }

        // Notify the display thread that new data is available
        dataCondVar.notify_one();

        // Sleep for a second before the next reading
        sleep(1);
    }
}

void updateDisplay()
{
    SSD1306 ssd1306;

    // Initialize display
    ssd1306.setup();
    ssd1306.ssd1306_oled_set_XY(2, 1);
    ssd1306.ssd1306_oled_write_line(0, "Temp    : ");
    ssd1306.ssd1306_oled_set_XY(2, 3);
    ssd1306.ssd1306_oled_write_line(0, "Humidity: ");
    ssd1306.ssd1306_oled_set_XY(2, 5);
    ssd1306.ssd1306_oled_write_line(0, "Pressure: ");

    while (true)
    {
        // Update display with sensor data
        std::unique_lock<std::mutex> lock(dataMutex);
        dataCondVar.wait(lock, []
                         { return newDataAvailable; });

        char array[10];
        sprintf(array, "%2.2f C", sensorData.temperature);
        ssd1306.ssd1306_oled_set_XY(60, 1);
        ssd1306.ssd1306_oled_write_line(0, array);

        sprintf(array, "%2.2f %%", sensorData.humidity);
        ssd1306.ssd1306_oled_set_XY(60, 3);
        ssd1306.ssd1306_oled_write_line(0, array);

        sprintf(array, "%ld hPa", sensorData.pressure);
        ssd1306.ssd1306_oled_set_XY(60, 5);
        ssd1306.ssd1306_oled_write_line(0, array);
        newDataAvailable = false;
    }
}

int main()
{
    std::thread sensorThread(readSensors);
    std::thread displayThread(updateDisplay);

    // Join threads to the main thread
    sensorThread.join();
    displayThread.join();

    return 0;
}
