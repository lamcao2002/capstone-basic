#include <iostream>
#include <thread>
#include "aht20.h"
#include "bmp180.h"

AHT20 humiditySensor;
BMP180 bmp180Sensor((uint8_t)bmp180_ultra_low);

using namespace std::literals::chrono_literals;
using namespace std;

int main()
{
    if (humiditySensor.begin() == false)
    {
        cout << "AHT20 not detected. Please check wiring. Freezing." << endl;
    }

    bmp180Sensor.setup();
    bmp180Sensor.getCalParam();

    std::this_thread::sleep_for(2000ms);

    while (1)
    {
        // If a new measurement is available
        if (humiditySensor.available() == true)
        {
            // Get the new temperature and humidity value
            float temperature = humiditySensor.getTemperature();
            float humidity = humiditySensor.getHumidity();

            printf("Temperature: %2.2f\n", temperature);
            printf("Humidity: %2.2f\n", humidity);
        }

        bmp180Sensor.getUT();
        bmp180Sensor.getUP();
        long temperature1 = bmp180Sensor.getTemperature();
        long pressure = bmp180Sensor.getPressure();
        printf("Temperature1 : %2.2f\n", (float)temperature1 / 10.0);
        cout << "pressure: " << pressure << endl;

        std::this_thread::sleep_for(2000ms);
    }
}