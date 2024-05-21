#include <iostream>
#include "aht20.h"
#include <thread>

AHT20 humiditySensor;

using namespace std::literals::chrono_literals;
using namespace std;

int main()
{
    if (humiditySensor.begin() == false)
    {
        cout << "AHT20 not detected. Please check wiring. Freezing." << endl;
    }
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

        std::this_thread::sleep_for(2000ms);
    }
}