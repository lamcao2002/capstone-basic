// #include <iostream>
// #include <thread>
// // #include "aht20.h"
// // #include "bmp180.h"
// #include "ssd1306.h"

// #include <signal.h>
// #include <stdio.h>
// #include <stdlib.h>
// // AHT20 humiditySensor;
// // BMP180 bmp180Sensor((uint8_t)bmp180_ultra_low);
// SSD1306 oledScreen;

// using namespace std::literals::chrono_literals;
// using namespace std;

// // flag for keyboard interrupt
// static volatile int keepRunning = 1;

// // interrupt signal handler
// void intHandler(int sig)
// {
//     keepRunning = 0;
// }

// include
// device
    //include
// test 

// int main()
// {
//     // if (humiditySensor.begin() == false)
//     // {
//     //     cout << "AHT20 not detected. Please check wiring. Freezing." << endl;
//     // }

//     // bmp180Sensor.setup();
//     // bmp180Sensor.getCalParam();

//     // std::this_thread::sleep_for(2000ms);

//     // while (1)
//     // {
//     //     // If a new measurement is available
//     //     if (humiditySensor.available() == true)
//     //     {
//     //         // Get the new temperature and humidity value
//     //         float temperature = humiditySensor.getTemperature();
//     //         float humidity = humiditySensor.getHumidity();

//     //         printf("Temperature: %2.2f\n", temperature);
//     //         printf("Humidity: %2.2f\n", humidity);
//     //     }

//     //     bmp180Sensor.getUT();
//     //     bmp180Sensor.getUP();
//     //     long temperature1 = bmp180Sensor.getTemperature();
//     //     long pressure = bmp180Sensor.getPressure();
//     //     printf("Temperature1 : %2.2f\n", (float)temperature1 / 10.0);
//     //     cout << "pressure: " << pressure << endl;

//     //     std::this_thread::sleep_for(2000ms);
//     // }

//     signal(SIGINT, intHandler);
//     oledScreen.ssd1306I2CSetup(0x3C);
//     oledScreen.displayOn();
//     oledScreen.clearDisplay();
//     std::this_thread::sleep_for(500ms);
//     oledScreen.draw_line(1, 1, (unsigned char *)"hello");
//     oledScreen.updateDisplay();
//     // -- main loop --
//     // while (keepRunning)
//     // {
//     //     // turn on display
//     //     oledScreen.draw_line(2, 4, (unsigned char *)"hello");
//     //     // oledScreen.draw_line(4, 8, (unsigned char *)"world");
//     //     oledScreen.updateDisplay();
//     //     std::this_thread::sleep_for(3000ms);
//     //     // oledScreen.clearDisplay();
//     // }

//     // // --- routine when interrupt detected ---
//     // oledScreen.clearDisplay();
//     // oledScreen.draw_line(2, 1, (unsigned char *)"off in 2 sec");
//     // oledScreen.updateDisplay();
//     // std::this_thread::sleep_for(2000ms);
//     // oledScreen.displayOff();

//     return 0;
// }

#include <iostream>
// #include <stdint.h>
// #include <stdlib.h>
// #include <inttypes.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <string.h>
// #include <fcntl.h>
// #include <sys/ioctl.h>
#include <thread>

#include "ssd1306.h"
#include "bmp180.h"

BMP180 bmp180Sensor((uint8_t)bmp180_ultra_low);
SSD1306 ssd1306Sensor;

using namespace std::literals::chrono_literals;

int main(int argc, char **argv)
{
    uint8_t i2c_node_address = 1;
    int x = -1;
    int y = -1;
    char line[25] = {0};
    char msg[200] = {0};
    char oled_type[10] = {0};
    int clear_line = -1;
    int clear_all = -1;
    int orientation = -1;
    int inverted = -1;
    int display = -1;
    int font = 0;

    uint8_t rc = 0;

    // open the I2C device node
    ssd1306Sensor.setup();

    // // set display on off
    // rc += ssd1306_oled_onoff(display);

    ssd1306Sensor.ssd1306_oled_set_XY(10, 5);

    bmp180Sensor.setup();
    bmp180Sensor.getCalParam();
    ssd1306Sensor.ssd1306_oled_write_line(font, "temp: ");

    while (1)
    {
        bmp180Sensor.getUT();
        bmp180Sensor.getUP();

        float temp = (float)bmp180Sensor.getTemperature() / 10.0;
        char array[10];
        sprintf(array, "%f", temp);

        ssd1306Sensor.ssd1306_oled_set_XY(58, 5);
        ssd1306Sensor.ssd1306_oled_write_line(font, array);
        // auto l0 = [&]
        // {
        //     rc += ssd1306_oled_set_XY(58, 5);
        //     rc += ssd1306_oled_write_line(font, array);
        // };

        std::this_thread::sleep_for(1000ms);
    }

    // print text

    // close the I2C device node
    ssd1306Sensor.ssd1306_end();

    return rc;
}