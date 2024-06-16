#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>

#include "ssd1306.h"

using namespace std::literals::chrono_literals;
using namespace std;

SSD1306 ssd1306Sensor;

int main()
{
    // open the I2C device node
    ssd1306Sensor.setup();
    ssd1306Sensor.ssd1306_oled_set_XY(10, 5);
    ssd1306Sensor.ssd1306_oled_write_line(0, "hello world");

    // close the I2C device node
    ssd1306Sensor.ssd1306_end();
    return 0;
}