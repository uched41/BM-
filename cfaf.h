#ifndef CFAF320_H
#define CFAF320_H

#include <stdint.h>

#define mSwap(a, b, t) \
  {                    \
    t = a;             \
    a = b;             \
    b = t;             \
  }                    \

void display_init(void);

void SPI_sendCommand(uint8_t command);

void SPI_sendData(uint16_t data);

void Initialize_LCD(void);

void Set_LCD_for_write_at_X_Y(uint16_t x, uint16_t y);

void Fill_LCD(uint8_t R, uint8_t G, uint8_t B);

void Put_Pixel(uint16_t x, uint16_t y, uint8_t R, uint8_t G, uint8_t B);

void LCD_Circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t R, uint16_t G, uint16_t B);

void Fast_Horizontal_Line(uint16_t x0, uint16_t y, uint16_t x1, uint8_t R, uint8_t G, uint8_t B);

void LCD_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t r, uint8_t g, uint8_t b);

void SPI_send_pixels(uint8_t pixel_count, uint8_t *data_ptr);

void circleDemo(void);

void grayCodeDemo(void);

void cheesyLineDemo(void);

#endif 