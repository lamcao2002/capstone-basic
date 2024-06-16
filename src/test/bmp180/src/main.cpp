#include <iostream>
#include <thread>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp180.h"

using namespace std::literals::chrono_literals;
using namespace std;

BMP180 bmp180Sensor((uint8_t)bmp180_ultra_low);

int main()
{
    bmp180Sensor.setup();
    bmp180Sensor.getCalParam();

    while (1)
    {
        bmp180Sensor.getUT();
        bmp180Sensor.getUP();
        long temperature1 = bmp180Sensor.getTemperature();
        long pressure = bmp180Sensor.getPressure();
        printf("Temperature1 : %2.2f\n", (float)temperature1 / 10.0);
        cout << "pressure: " << pressure << endl;

        std::this_thread::sleep_for(1000ms);
    }
    
    return 0;
}