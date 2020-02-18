#include <stdlib.h>
#include "nrf_log.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_gpiote.h"
#include "cfaf.h"
#include "myconfig.h"

#define CLR_RS 		nrf_gpio_pin_clear(DISPLAY_RS_PIN)
#define SET_RS 		nrf_gpio_pin_set(DISPLAY_RS_PIN)
#define CLR_RESET 	nrf_gpio_pin_clear(DISPLAY_RESET_PIN)
#define SET_RESET 	nrf_gpio_pin_set(DISPLAY_RESET_PIN)
#define CLR_CS 		nrf_gpio_pin_clear(DISPLAY_SS_PIN) 
#define SET_CS 		nrf_gpio_pin_set(DISPLAY_SS_PIN) 
#define CLR_MOSI 	nrf_gpio_pin_clear(DISPLAY_MOSI_PIN) 
#define SET_MOSI 	nrf_gpio_pin_set(DISPLAY_MOSI_PIN) 
#define CLR_SCK 	nrf_gpio_pin_clear(DISPLAY_SCK_PIN)
#define SET_SCK 	nrf_gpio_pin_set(DISPLAY_SCK_PIN)

#define CLR_BL nrf_gpio_pin_clear(DISPLAY_BACKLIGHT)
#define SET_BL nrf_gpio_pin_set(DISPLAY_BACKLIGHT)

static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(1);  

void display_init(void){
	ret_code_t err_code;

	nrf_gpio_cfg_output(DISPLAY_RESET_PIN);
	nrf_gpio_cfg_output(DISPLAY_RS_PIN);
	nrf_gpio_cfg_output(DISPLAY_SS_PIN);
	//nrf_gpio_cfg_output(DISPLAY_BACKLIGHT);
	//SET_BL;


        //ret_code_t err_code;
	nrf_drv_gpiote_out_config_t in_config = GPIOTE_CONFIG_OUT_SIMPLE(true);
	//in_config.pull = NRF_GPIO_PIN_NOPULL;

	err_code = nrf_drv_gpiote_out_init(DISPLAY_BACKLIGHT, &in_config);
	APP_ERROR_CHECK(err_code);
        nrf_drv_gpiote_out_set(DISPLAY_BACKLIGHT);

	//Drive the ports to a reasonable starting state.
	CLR_RESET;
	CLR_RS;
	SET_CS;
	CLR_MOSI;
																
	nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	//spi_config.ss_pin   = DISPLAY_SS_PIN;
	spi_config.mosi_pin = DISPLAY_MOSI_PIN;
	//spi_config.miso_pin = cfaf->_miso;
	spi_config.sck_pin  = DISPLAY_SCK_PIN;
	spi_config.frequency = NRF_DRV_SPI_FREQ_8M;

	err_code = nrf_drv_spi_init(&spi, &spi_config, NULL, NULL);
	NRF_LOG_INFO("SPI : 0x%04X", err_code);
	APP_ERROR_CHECK(err_code);

	Initialize_LCD();

	NRF_LOG_INFO("Display initialization complete.");
}

void SPI_sendCommand(uint8_t command){
  CLR_RS;
  CLR_CS;
  uint8_t cmd = command;

  //APP_ERROR_CHECK(
		nrf_drv_spi_transfer(&spi, &cmd, 1, NULL, 0);
	//);
  SET_CS;
}


void SPI_sendData(uint16_t data){
  SET_RS;
  CLR_CS;
  uint8_t dt[2];
  dt[0] = data >> 8;
  dt[1] = data & 0x00FF;
  
  //APP_ERROR_CHECK(
            nrf_drv_spi_transfer(&spi, dt, 2, NULL, 0);
    //);
  SET_CS;
}


void Initialize_LCD(void)
{
  //Reset the LCD controller
  CLR_RESET;
  nrf_delay_ms(1); //10µS min
  SET_RESET;
  nrf_delay_ms(150); //120mS max

  SPI_sendCommand(0x0028); // VCOM OTP
  SPI_sendData(0x0006);    // Page 55-56 of SSD2119 datasheet

  SPI_sendCommand(0x0000); // start Oscillator
  SPI_sendData(0x0001);    // Page 36 of SSD2119 datasheet

  SPI_sendCommand(0x0010); // Sleep mode
  SPI_sendData(0x0000);    // Page 49 of SSD2119 datasheet

  SPI_sendCommand(0x0001); // Driver Output Control
  //SPI_sendData(0x32EF);       // Page 36-39 of SSD2119 datasheet
  //Mirror RL, so 0,0 is in lower-left (quadrant 1)
  SPI_sendData(0x72EF); // Page 36-39 of SSD2119 datasheet

  SPI_sendCommand(0x0002); // LCD Driving Waveform Control
  SPI_sendData(0x0600);    // Page 40-42 of SSD2119 datasheet

  SPI_sendCommand(0x0003); // Power Control 1
  SPI_sendData(0x6A38);    // Page 43-44 of SSD2119 datasheet

  SPI_sendCommand(0x0011); // Entry Mode
  SPI_sendData(0x6870);    // Page 50-52 of SSD2119 datasheet

  SPI_sendCommand(0X000F); // Gate Scan Position
  SPI_sendData(0x0000);    // Page 49 of SSD2119 datasheet

  SPI_sendCommand(0X000B); // Frame Cycle Control
  SPI_sendData(0x5308);    // Page 45 of SSD2119 datasheet

  SPI_sendCommand(0x000C); // Power Control 2
  SPI_sendData(0x0003);    // Page 47 of SSD2119 datasheet

  SPI_sendCommand(0x000D); // Power Control 3
  SPI_sendData(0x000A);    // Page 48 of SSD2119 datasheet

  SPI_sendCommand(0x000E); // Power Control 4
  SPI_sendData(0x2E00);    // Page 48 of SSD2119 datasheet

  SPI_sendCommand(0x001E); // Power Control 5
  SPI_sendData(0x00B7);    // Page 55 of SSD2119 datasheet

  SPI_sendCommand(0x0025); // Frame Frequency Control
  SPI_sendData(0x8000);    // Page 53 of SSD2119 datasheet

  SPI_sendCommand(0x0026); // Analog setting
  SPI_sendData(0x3800);    // Page 54 of SSD2119 datasheet

  SPI_sendCommand(0x0027); // Critical setting to avoid pixel defect
  SPI_sendData(0x0078);    // per solomon systech, apparently undocumented.

  SPI_sendCommand(0x004E); // Ram Address Set
  SPI_sendData(0x0000);    // Page 58 of SSD2119 datasheet

  SPI_sendCommand(0x004F); // Ram Address Set
  SPI_sendData(0x0000);    // Page 58 of SSD2119 datasheet

  SPI_sendCommand(0x0012); // Sleep mode
  SPI_sendData(0x0D99);    // Page 49 of SSD2119 datasheet

  // Gamma Control (R30h to R3Bh) -- Page 56 of SSD2119 datasheet
  SPI_sendCommand(0x0030);
  SPI_sendData(0x0000);

  SPI_sendCommand(0x0031);
  SPI_sendData(0x0104);

  SPI_sendCommand(0x0032);
  SPI_sendData(0x0100);

  SPI_sendCommand(0x0033);
  SPI_sendData(0x0305);

  SPI_sendCommand(0x0034);
  SPI_sendData(0x0505);

  SPI_sendCommand(0x0035);
  SPI_sendData(0x0305);

  SPI_sendCommand(0x0036);
  SPI_sendData(0x0707);

  SPI_sendCommand(0x0037);
  SPI_sendData(0x0300);

  SPI_sendCommand(0x003A);
  SPI_sendData(0x1200);

  SPI_sendCommand(0x003B);
  SPI_sendData(0x0800);

  SPI_sendCommand(0x0007); // Display Control
  SPI_sendData(0x0033);    // Page 45 of SSD2119 datasheet

  SPI_sendCommand(0x0044); // Vertical RAM address position
  SPI_sendData(0xEF00);    // Page 57 of SSD2119 datasheet
  SPI_sendCommand(0x0045); // Horizontal RAM address position
  SPI_sendData(0x0000);    // Page 57 of SSD2119 datasheet
  SPI_sendCommand(0x0046); // Horizontal RAM address position
  SPI_sendData(0x013F);    // Page 57 of SSD2119 datasheet

  SPI_sendCommand(0x0022); // RAM data write/read

  SPI_sendCommand(0x10); // set Sleep mode
  SPI_sendData(0x0000);  //sleep OUT
  nrf_delay_ms(200);
  SPI_sendCommand(0x07); // Display Control
  SPI_sendData(0x0033);  // Display on
}


void Set_LCD_for_write_at_X_Y(uint16_t x, uint16_t y){
  SPI_sendCommand(0x4E);   // RAM address set, X
  SPI_sendData(x);         // Page 59 of SSD2119 datasheet
  SPI_sendCommand(0x4F);   // RAM address set, Y
  SPI_sendData(y);         // Page 59 of SSD2119 datasheet
  SPI_sendCommand(0x0022); // RAM data write/read
}


void Fill_LCD(uint8_t R, uint8_t G, uint8_t B){
  uint32_t i;

  Set_LCD_for_write_at_X_Y(0, 0);

  //Pre-calculate the two bytes for this color of pixel
  uint8_t first_half;
  uint8_t second_half;
  uint8_t tbuf[2];

  //The display takes two bytes (565) RRRRR GGGGGG BBBBB
  //to show one pixel.
  tbuf[0] = (R & 0xF8) | (G >> 5);
  tbuf[1] = ((G & 0xFC) << 3) | (B >> 3);
  
  // Select the LCD controller
  CLR_CS;
  // Select the LCD controller's data register
  SET_RS;

  //Fill display with a given RGB value
  for (i = 0; i < (320UL * 240UL); i++)
  {
    nrf_drv_spi_transfer(&spi, tbuf, 2, NULL, 0);
  }
  // Deselect the OLED controller
  SET_CS;
}


void Put_Pixel(uint16_t x, uint16_t y, uint8_t R, uint8_t G, uint8_t B){
  Set_LCD_for_write_at_X_Y(x, y);
  //Write the single, 16-bit pixel's worth of data
  SPI_sendData(((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3));
}


void LCD_Circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t R, uint16_t G, uint16_t B){
  uint16_t x = radius;
  uint16_t y = 0;
  int16_t radiusError = 1 - (int16_t)x;

  while (x >= y)
  {
    //11 O'Clock
    Put_Pixel(x0 - y, y0 + x, R, G, B);
    //1 O'Clock
    Put_Pixel(x0 + y, y0 + x, R, G, B);
    //10 O'Clock
    Put_Pixel(x0 - x, y0 + y, R, G, B);
    //2 O'Clock
    Put_Pixel(x0 + x, y0 + y, R, G, B);
    //8 O'Clock
    Put_Pixel(x0 - x, y0 - y, R, G, B);
    //4 O'Clock
    Put_Pixel(x0 + x, y0 - y, R, G, B);
    //7 O'Clock
    Put_Pixel(x0 - y, y0 - x, R, G, B);
    //5 O'Clock
    Put_Pixel(x0 + y, y0 - x, R, G, B);

    y++;
    if (radiusError < 0)
      radiusError += (int16_t)(2 * y + 1);
    else
    {
      x--;
      radiusError += 2 * (((int16_t)y - (int16_t)x) + 1);
    }
  }
}


void Fast_Horizontal_Line(uint16_t x0, uint16_t y, uint16_t x1, uint8_t R, uint8_t G, uint8_t B){
  uint16_t temp;
  uint8_t first_half;
  uint8_t second_half;
  uint8_t tbuf[2];

  if (x1 < x0)
  {
    mSwap(x0, x1, temp);
  }

  Set_LCD_for_write_at_X_Y(x0, y);

  // Select the LCD's data register
  SET_RS;
  // Select the LCD controller
  CLR_CS;

  //Pre-calculate the two bytes for this color of pixel
  //The display takes two bytes (565) RRRRR GGGGGG BBBBB
  //to show one pixel.
  tbuf[0] = (R & 0xF8) | (G >> 5);
  tbuf[1] = ((G & 0xFC) << 3) | (B >> 3);

  while (x0 <= x1)
  {
    //Write the single pixel's worth of data
    x0++;
    nrf_drv_spi_transfer(&spi, tbuf, 2, NULL, 0);
  }
}


void LCD_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t r, uint8_t g, uint8_t b){
  int16_t dx;
  int16_t sx;
  int16_t dy;
  int16_t sy;
  int16_t err;
  int16_t e2;

  //General case
  if (y0 != y1)
  {
    dx = abs((int16_t)x1 - (int16_t)x0);
    sx = x0 < x1 ? 1 : -1;
    dy = abs((int16_t)y1 - (int16_t)y0);
    sy = y0 < y1 ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;

    for (;;)
    {
      Put_Pixel(x0, y0, r, g, b);
      if ((x0 == x1) && (y0 == y1))
      {
        break;
      }
      e2 = err;
      if (e2 > -dx)
      {
        err -= dy;
        x0 = (uint16_t)((int16_t)x0 + sx);
      }
      if (e2 < dy)
      {
        err += dx;
        y0 = (uint16_t)((int16_t)y0 + sy);
      }
    }
  }
  else
  {
    //Optimized for LCD
    Fast_Horizontal_Line(x0, y0, x1, r, g, b);
  }
}


void circleDemo(void){
  //Draw a cyan circle
  //LCD_Circle(160,  120,  119,0x00,0xFF,0xFF);
  //Draw a cyan circle
  LCD_Circle(160, 120, 119, 0x00, 0xFF, 0xFF);
  //Draw a white circle
  LCD_Circle(160, 120, 40, 0xFF, 0xFF, 0xFF);
  //Draw a green circle
  LCD_Circle(160 - 80, 120, 37, 0x00, 0xFF, 0x00);
  //Draw a red circle
  LCD_Circle(160 + 80, 120, 37, 0xFF, 0x00, 0x00);
  //Draw a purple circle
  LCD_Circle(160, 120 - 80, 32, 0xFF, 0x00, 0xFF);
  //Draw a orange circle
  LCD_Circle(160, 120 + 80, 28, 0xFF, 0xA5, 0x00);
}


void grayCodeDemo(void){
  //Gray code fill demo
  Fill_LCD(0x00, 0xFF, 0xFF);
  delay(200);
  Fill_LCD(0x00, 0xFF, 0xBF);
  delay(200); //transition
  Fill_LCD(0x00, 0xFF, 0x7F);
  delay(200); //transition
  Fill_LCD(0x00, 0xFF, 0x3F);
  delay(200); //transition
  Fill_LCD(0x00, 0xFF, 0x00);
  delay(200);
  Fill_LCD(0x3F, 0xFF, 0x00);
  delay(200); //transition
  Fill_LCD(0x7F, 0xFF, 0x00);
  delay(200); //transition
  Fill_LCD(0xBF, 0xFF, 0x00);
  delay(200); //transition
  Fill_LCD(0xFF, 0xFF, 0x00);
  delay(200);
  Fill_LCD(0xFF, 0xFF, 0x3F);
  delay(200); //transition
  Fill_LCD(0xFF, 0xFF, 0x7F);
  delay(200); //transition
  Fill_LCD(0xFF, 0xFF, 0xBF);
  delay(200); //transition
  Fill_LCD(0xFF, 0xFF, 0xFF);
  delay(200);
  Fill_LCD(0xFF, 0xBF, 0xFF);
  delay(200); //transition
  Fill_LCD(0xFF, 0x7F, 0xFF);
  delay(200); //transition
  Fill_LCD(0xFF, 0x3F, 0xFF);
  delay(200); //transition
  Fill_LCD(0xFF, 0x00, 0xFF);
  delay(200);
  Fill_LCD(0xFF, 0x00, 0xBF);
  delay(200); //transition
  Fill_LCD(0xFF, 0x00, 0x7F);
  delay(200); //transition
  Fill_LCD(0xFF, 0x00, 0x3F);
  delay(200); //transition
  Fill_LCD(0xFF, 0x00, 0x00);
  delay(200);
  Fill_LCD(0xBF, 0x00, 0x00);
  delay(200); //transition
  Fill_LCD(0x7F, 0x00, 0x00);
  delay(200); //transition
  Fill_LCD(0x3F, 0x00, 0x00);
  delay(200); //transition
  Fill_LCD(0x00, 0x00, 0x00);
  delay(200);
  Fill_LCD(0x00, 0x00, 0x3F);
  delay(200); //transition
  Fill_LCD(0x00, 0x00, 0x7F);
  delay(200); //transition
  Fill_LCD(0x00, 0x00, 0xBF);
  delay(200); //transition
  Fill_LCD(0x00, 0x00, 0xFF);
  delay(200);
}


void cheesyLineDemo(void){
  //Cheesy lines
  uint8_t i;
  uint16_t x;
  uint16_t y;
  uint16_t sub_x;
  uint16_t sub_y;
  uint8_t r;
  uint8_t g;
  uint8_t b;

  r = 0xff;
  g = 0x00;
  b = 0x80;
  for (x = 0; x < 320; x++)
  {
    LCD_Line(160, 120,
             x, 0,
             r++, g--, b += 2);
  }
  for (y = 0; y < 240; y++)
  {
    LCD_Line(160, 120,
             319, y,
             r++, g += 4, b += 2);
  }
  for (x = 319; 0 != x; x--)
  {
    LCD_Line(160, 120,
             x, 239,
             r -= 3, g -= 2, b -= 1);
  }
  for (y = 239; 0 != y; y--)
  {
    LCD_Line(160, 120,
             0, y,
             r + -3, g--, b++);
  }
  delay(1000);

  Fill_LCD(0x00, 0x00, 0x00);
  for (i = 2; i < 120; i += 2)
  {
    LCD_Circle(i + 2 + 40, 120, i, i << 2, 0xff - (i << 2), 0xFF);
  }
}
