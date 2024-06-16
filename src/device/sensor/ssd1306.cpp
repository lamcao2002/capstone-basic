#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "ssd1306.h"
#include "font.h"

static uint8_t data_buf[1024];
static uint8_t max_lines = 0;
static uint8_t max_columns = 0;
static uint8_t global_x = 0;
static uint8_t global_y = 0;

void SSD1306::setup()
{
    connect(SSD1306_I2C_ADDR);
    // init oled module
    config();
    // clear display
    clearScreen();
}

uint8_t SSD1306::ssd1306_end()
{
    // return _i2c_close();
    return 0;
}

void SSD1306::ssd1306_oled_onoff(uint8_t onoff)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    if (onoff == 0)
        data_buf[1] = SSD1306_COMM_DISPLAY_OFF;
    else
        data_buf[1] = SSD1306_COMM_DISPLAY_ON;

    writeByte(data_buf, 2);
}

void SSD1306::ssd1306_oled_horizontal_flip(uint8_t flip)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    if (flip == 0)
        data_buf[1] = SSD1306_COMM_HORIZ_NORM;
    else
        data_buf[1] = SSD1306_COMM_HORIZ_FLIP;

    writeByte(data_buf, 2);
}

void SSD1306::ssd1306_oled_display_flip(uint8_t flip)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    if (flip == 0)
        data_buf[1] = SSD1306_COMM_DISP_NORM;
    else
        data_buf[1] = SSD1306_COMM_DISP_INVERSE;

    writeByte(data_buf, 2);
}

// 128x32 please use value 32
// 128x64 please use value 64
void SSD1306::ssd1306_oled_multiplex(uint8_t row)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_MULTIPLEX;
    data_buf[2] = row - 1;

    writeByte(data_buf, 3);
}

// offset range 0x00~0x3f
void SSD1306::ssd1306_oled_vert_shift(uint8_t offset)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_VERT_OFFSET;
    data_buf[2] = offset;

    writeByte(data_buf, 3);
}

// default value for clk is 0x80
void SSD1306::ssd1306_oled_set_clock(uint8_t clk)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_CLK_SET;
    data_buf[2] = clk;

    writeByte(data_buf, 3);
}

// default value for precharge is 0xf1
void SSD1306::ssd1306_oled_set_precharge(uint8_t precharge)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_PRECHARGE;
    data_buf[2] = precharge;

    writeByte(data_buf, 3);
}

// default value for deselect is 0x40
void SSD1306::ssd1306_oled_set_deselect(uint8_t voltage)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_DESELECT_LV;
    data_buf[2] = voltage;

    writeByte(data_buf, 3);
}

// default value for com pin is 0x02
void SSD1306::ssd1306_oled_set_com_pin(uint8_t value)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_COM_PIN;
    data_buf[2] = value;

    writeByte(data_buf, 3);
}

// default value use SSD1306_PAGE_MODE
void SSD1306::ssd1306_oled_set_mem_mode(uint8_t mode)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_MEMORY_MODE;
    data_buf[2] = mode;

    writeByte(data_buf, 3);
}

void SSD1306::ssd1306_oled_set_col(uint8_t start, uint8_t end)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_SET_COL_ADDR;
    data_buf[2] = start;
    data_buf[3] = end;

    writeByte(data_buf, 4);
}

void SSD1306::ssd1306_oled_set_page(uint8_t start, uint8_t end)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_SET_PAGE_ADDR;
    data_buf[2] = start;
    data_buf[3] = end;

    writeByte(data_buf, 4);
}

// default contrast value is 0x7f
void SSD1306::ssd1306_oled_set_constrast(uint8_t value)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_CONTRAST;
    data_buf[2] = value;

    writeByte(data_buf, 3);
}

void SSD1306::ssd1306_oled_scroll_onoff(uint8_t onoff)
{
    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    if (onoff == 0)
        data_buf[1] = SSD1306_COMM_DISABLE_SCROLL;
    else
        data_buf[1] = SSD1306_COMM_ENABLE_SCROLL;

    writeByte(data_buf, 2);
}

void SSD1306::ssd1306_oled_set_X(uint8_t x)
{
    if (x >= max_columns)
        return;

    global_x = x;

    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_LOW_COLUMN | (x & 0x0f);
    data_buf[2] = SSD1306_COMM_HIGH_COLUMN | ((x >> 4) & 0x0f);

    writeByte(data_buf, 3);
}

void SSD1306::ssd1306_oled_set_Y(uint8_t y)
{
    if (y >= (max_lines / 8))
        return;

    global_y = y;

    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_PAGE_NUMBER | (y & 0x0f);

    writeByte(data_buf, 2);
}

void SSD1306::ssd1306_oled_set_XY(uint8_t x, uint8_t y)
{
    if (x >= max_columns || y >= (max_lines / 8))
        return;

    global_x = x;
    global_y = y;

    data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
    data_buf[1] = SSD1306_COMM_PAGE_NUMBER | (y & 0x0f);

    data_buf[2] = SSD1306_COMM_LOW_COLUMN | (x & 0x0f);

    data_buf[3] = SSD1306_COMM_HIGH_COLUMN | ((x >> 4) & 0x0f);

    writeByte(data_buf, 4);
}

void SSD1306::ssd1306_oled_set_rotate(uint8_t degree)
{
    // only degree 0 and 180
    if (degree == 0)
    {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_HORIZ_FLIP;
        data_buf[2] = SSD1306_COMM_SCAN_REVS;

        writeByte(data_buf, 3);
    }
    else if (degree == 180)
    {
        data_buf[0] = SSD1306_COMM_CONTROL_BYTE;
        data_buf[1] = SSD1306_COMM_HORIZ_NORM;
        data_buf[2] = SSD1306_COMM_SCAN_NORM;

        writeByte(data_buf, 3);
    }
    else
        return;
}

void SSD1306::config()
{
    max_lines = SSD1306_128_64_LINES;
    max_columns = SSD1306_128_64_COLUMNS;
    global_x = 0;
    global_y = 0;

    uint16_t i = 0;
    data_buf[i++] = SSD1306_COMM_CONTROL_BYTE; // command control byte
    data_buf[i++] = SSD1306_COMM_DISPLAY_OFF;  // display off
    data_buf[i++] = SSD1306_COMM_DISP_NORM;    // Set Normal Display (default)
    data_buf[i++] = SSD1306_COMM_CLK_SET;      // SETDISPLAYCLOCKDIV
    data_buf[i++] = 0x80;                      // the suggested ratio 0x80
    data_buf[i++] = SSD1306_COMM_MULTIPLEX;    // SSD1306_SETMULTIPLEX
    data_buf[i++] = max_lines - 1;             // height is 32 or 64 (always -1)
    data_buf[i++] = SSD1306_COMM_VERT_OFFSET;  // SETDISPLAYOFFSET
    data_buf[i++] = 0;                         // no offset
    data_buf[i++] = SSD1306_COMM_START_LINE;   // SETSTARTLINE
    data_buf[i++] = SSD1306_COMM_CHARGE_PUMP;  // CHARGEPUMP
    data_buf[i++] = 0x14;                      // turn on charge pump
    data_buf[i++] = SSD1306_COMM_MEMORY_MODE;  // MEMORYMODE
    data_buf[i++] = SSD1306_PAGE_MODE;         // page mode
    data_buf[i++] = SSD1306_COMM_HORIZ_FLIP;   // SEGREMAP  Mirror screen horizontally (A0)
    data_buf[i++] = SSD1306_COMM_SCAN_REVS;    // COMSCANDEC Rotate screen vertically (C0)
    data_buf[i++] = SSD1306_COMM_COM_PIN;      // HARDWARE PIN

    data_buf[i++] = 0x12;                        // for 64 lines or 48 lines
    data_buf[i++] = SSD1306_COMM_CONTRAST;       // SETCONTRAST
    data_buf[i++] = 0x7f;                        // default contract value
    data_buf[i++] = SSD1306_COMM_PRECHARGE;      // SETPRECHARGE
    data_buf[i++] = 0xf1;                        // default precharge value
    data_buf[i++] = SSD1306_COMM_DESELECT_LV;    // SETVCOMDETECT
    data_buf[i++] = 0x40;                        // default deselect value
    data_buf[i++] = SSD1306_COMM_RESUME_RAM;     // DISPLAYALLON_RESUME
    data_buf[i++] = SSD1306_COMM_DISP_NORM;      // NORMALDISPLAY
    data_buf[i++] = SSD1306_COMM_DISPLAY_ON;     // DISPLAY ON
    data_buf[i++] = SSD1306_COMM_DISABLE_SCROLL; // Stop scroll

    writeByte(data_buf, i);
}

void SSD1306::ssd1306_oled_write_line(uint8_t size, char *ptr)
{
    uint16_t i = 0;
    uint16_t index = 0;
    uint8_t *font_table = 0;
    uint8_t font_table_width = 0;

    if (ptr == 0)
        return;

    if (size == SSD1306_FONT_SMALL) // 5x7
    {
        font_table = (uint8_t *)font5x7;
        font_table_width = 5;
    }
    else if (size == SSD1306_FONT_NORMAL) // 8x8
    {
        font_table = (uint8_t *)font8x8;
        font_table_width = 8;
    }
    else
        return;

    data_buf[i++] = SSD1306_DATA_CONTROL_BYTE;

    // font table range in ascii table is from 0x20(space) to 0x7e(~)
    while (ptr[index] != 0 && i <= 1024)
    {
        if ((ptr[index] < ' ') || (ptr[index] > '~'))
            return;

        uint8_t *font_ptr = &font_table[(ptr[index] - 0x20) * font_table_width];
        uint8_t j = 0;
        for (j = 0; j < font_table_width; j++)
        {
            data_buf[i++] = font_ptr[j];
            if (i > 1024)
                return;
        }
        // insert 1 col space for small font size)
        if (size == SSD1306_FONT_SMALL)
            data_buf[i++] = 0x00;
        index++;
    }

    writeByte(data_buf, i);
}

void SSD1306::ssd1306_oled_write_string(uint8_t size, char *ptr)
{
    uint8_t rc = 0;

    if (ptr == 0)
        return;

    char *line = 0;
    char *cr = 0;
    char buf[20];

    line = ptr;
    do
    {
        memset(buf, 0, 20);
        cr = strstr(line, "\\n");
        if (cr != NULL)
        {
            strncpy(buf, line, cr - line);
        }
        else
        {
            strcpy(buf, line);
        }

        // set cursor position
        ssd1306_oled_set_XY(global_x, global_y);
        ssd1306_oled_write_line(size, buf);

        if (cr != NULL)
        {
            line = &cr[2];
            global_x = 0;
            global_y++;
            if (global_y >= (max_lines / 8))
                global_y = 0;
        }
        else
            line = NULL;

    } while (line != NULL);
}

void SSD1306::clearLine(uint8_t row)
{
    uint8_t i;
    if (row >= (max_lines / 8))
        return;

    ssd1306_oled_set_XY(0, row);
    data_buf[0] = SSD1306_DATA_CONTROL_BYTE;
    for (i = 0; i < 132; i++)
        data_buf[i + 1] = 0x00;

    return writeByte(data_buf, 1 + 132);
}

void SSD1306::clearScreen()
{
    uint8_t i;

    for (i = 0; i < (max_lines / 8); i++)
    {
        clearLine(i);
    }
}