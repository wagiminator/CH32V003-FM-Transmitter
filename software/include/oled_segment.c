// ===================================================================================
// SSD1306 128x32 Pixels OLED Segment Functions                               * v1.0 *
// ===================================================================================
//
// Collection of the most necessary functions for controlling an SSD1306 128x32 pixels
// I2C OLED as a simulated 8-digit 7-segment display.
//
// References:
// -----------
// - Neven Boyanov: https://github.com/tinusaur/ssd1306xled
// - Stephen Denne: https://github.com/datacute/Tiny4kOLED
// - TinyOLEDdemo: https://github.com/wagiminator/attiny13-tinyoleddemo
//
// 2022 by Stefan Wagner: https://github.com/wagiminator

#include "oled_segment.h"

// 13x32 7-segment digits font
const uint8_t OLED_FONT[] = {
  0xFC, 0x7F, 0xFF, 0x1F, 0xF9, 0x3F, 0xFE, 0x4F, 0xF3, 0x1F, 0xFC, 0x67,
  0x07, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x70,
  0x07, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0x70,
  0x07, 0x00, 0x00, 0x70, 0xF3, 0x1F, 0xFC, 0x67, 0xF9, 0x3F, 0xFE, 0x4F,
  0xFC, 0x7F, 0xFF, 0x1F, // 0
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xF0, 0x1F, 0xFC, 0x07, 0xF8, 0x3F, 0xFE, 0x0F,
  0xFC, 0x7F, 0xFF, 0x1F, // 1
  0x00, 0x00, 0xFF, 0x1F, 0x01, 0x00, 0xFE, 0x4F, 0x03, 0x80, 0xFC, 0x67,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01 ,0x70, 0xF3, 0x9F, 0x00, 0x60, 0xF9, 0x3F, 0x00, 0x40,
  0xFC, 0x7F, 0x00, 0x00, // 2
  0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x40, 0x03, 0x80, 0x00, 0x60,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0xF3, 0x9F, 0xFC, 0x67, 0xF9, 0x3F, 0xFE, 0x4F,
  0xFC, 0x7F, 0xFF, 0x1F, // 3
  0xFC, 0x7F, 0x00, 0x00, 0xF8, 0x3F, 0x00, 0x00, 0xF0, 0x9F, 0x00, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0xF0, 0x9F, 0xFC, 0x07, 0xF8, 0x3F, 0xFE, 0x0F,
  0xFC, 0x7F, 0xFF, 0x1F, // 4
  0xFC, 0x7F, 0x00, 0x00, 0xF9, 0x3F, 0x00, 0x40, 0xF3, 0x9F, 0x00, 0x60,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0x03, 0x80, 0xFC, 0x67, 0x01, 0x00, 0xFE, 0x4F,
  0x00, 0x00, 0xFF, 0x1F, // 5
  0xFC, 0x7F, 0xFF, 0x1F, 0xF9, 0x3F, 0xFE, 0x4F, 0xF3, 0x9F, 0xFC, 0x67,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0x03, 0x80, 0xFC, 0x67, 0x01, 0x00, 0xFE, 0x4F,
  0x00, 0x00, 0xFF, 0x1F, // 6
  0xFC, 0x7F, 0x00, 0x00, 0xF9, 0x3F, 0x00, 0x00, 0xF3, 0x1F, 0x00, 0x00,
  0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
  0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
  0x07, 0x00, 0x00, 0x00, 0xF3, 0x1F, 0xFC, 0x07, 0xF9, 0x3F, 0xFE, 0x0F,
  0xFC, 0x7F, 0xFF, 0x1F, // 7
  0xFC, 0x7F, 0xFF, 0x1F, 0xF9, 0x3F, 0xFE, 0x4F, 0xF3, 0x9F, 0xFC, 0x67,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0xF3, 0x9F, 0xFC, 0x67, 0xF9, 0x3F, 0xFE, 0x4F,
  0xFC, 0x7F, 0xFF, 0x1F, // 8
  0xFC, 0x7F, 0x00, 0x00, 0xF9, 0x3F, 0x00, 0x40, 0xF3, 0x9F, 0x00, 0x60,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70, 0x07, 0xC0, 0x01, 0x70,
  0x07, 0xC0, 0x01, 0x70, 0xF3, 0x9F, 0xFC, 0x67, 0xF9, 0x3F, 0xFE, 0x4F,
  0xFC, 0x7F, 0xFF, 0x1F, // 9
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x7C, 0x00, 0x00, 0x1F, 0x7C, 0x00,
  0x00, 0x1F, 0x7C, 0x00, 0x00, 0x1F, 0x7C, 0x00, 0x00, 0x1F, 0x7C, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, // : (10)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x7C,
  0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x7C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, // . (11)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, // - (12)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xC0, 0x01, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0xFC, 0x1F, 0x00,
  0x00, 0xFE, 0x3F, 0x00, 0x00, 0xFC, 0x1F, 0x00, 0x00, 0xC0, 0x01, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, // + (13)
  0xFC, 0x7F, 0xFF, 0x1F, 0xF8, 0x3F, 0xFE, 0x4F, 0xF0, 0x9F, 0xFC, 0x67,
  0x00, 0xC0, 0x01, 0x70, 0x00, 0xC0, 0x01, 0x70, 0x00, 0xC0, 0x01, 0x70,
  0x00, 0xC0, 0x01, 0x70, 0x00, 0xC0, 0x01, 0x70, 0x00, 0xC0, 0x01, 0x70,
  0x00, 0xC0, 0x01, 0x70, 0x00, 0x80, 0xFC, 0x67, 0x00, 0x00, 0xFE, 0x4F,
  0x00, 0x00, 0xFF, 0x1F, // b (14)
  0x00, 0x00, 0xFF, 0x1F, 0x00, 0x00, 0xFE, 0x4F, 0x00, 0x80, 0xFC, 0x67,
  0x00, 0xC0, 0x01, 0x70, 0x00, 0xC0, 0x01, 0x70, 0x00, 0xC0, 0x01, 0x70,
  0x00, 0xC0, 0x01, 0x70, 0x00, 0xC0, 0x01, 0x70, 0x00, 0xC0, 0x01, 0x70,
  0x00, 0xC0, 0x01, 0x70, 0xF0, 0x9F, 0xFC, 0x67, 0xF8, 0x3F, 0xFE, 0x4F,
  0xFC, 0x7F, 0xFF, 0x1F, // d (15)
  0xFC, 0x7F, 0xFF, 0x1F, 0xF9, 0x3F, 0xFE, 0x0F, 0xF3, 0x1F, 0xFC, 0x07,
  0x07, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0xE7, 0x1F, 0x00, 0x00,
  0xF7, 0x3F, 0x00, 0x00, 0xE7, 0x1F, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
  0x07, 0x00, 0x00, 0x00, 0xF3, 0x1F, 0xFC, 0x07, 0xF9, 0x3F, 0xFE, 0x0F,
  0xFC, 0x7F, 0xFF, 0x1F, // M (16)
  0xFC, 0x7F, 0xFF, 0x1F, 0xF8, 0x3F, 0xFE, 0x0F, 0xF0, 0x9F, 0xFC, 0x07,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00, 0x00, 0xC0, 0x01, 0x00,
  0x00, 0xC0, 0x01, 0x00, 0xF0, 0x9F, 0xFC, 0x07, 0xF8, 0x3F, 0xFE, 0x0F,
  0xFC, 0x7F, 0xFF, 0x1F, // H (17)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x80, 0x00, 0x60,
  0x00, 0xC0, 0x01, 0x76, 0x00, 0xC0, 0x81, 0x77, 0x00, 0xC0, 0xE1, 0x73,
  0x00, 0xC0, 0xF9, 0x71, 0x00, 0xC0, 0x7D, 0x70, 0x00, 0xC0, 0x1D, 0x70,
  0x00, 0xC0, 0x01, 0x70, 0x00, 0x80, 0x00, 0x60, 0x00, 0x00, 0x00, 0x40,
  0x00, 0x00, 0x00, 0x00  // z (18)
};

// BCD conversion divider array
const uint16_t DIVIDER[] = {1, 10, 100, 1000, 10000};

// OLED initialisation sequence
const uint8_t OLED_INIT_CMD[] = {
  OLED_MULTIPLEX,   0x1F,                         // set multiplex ratio  
  OLED_CHARGEPUMP,  0x14,                         // set DC-DC enable  
  OLED_MEMORYMODE,  0x01,                         // set memory addressing mode
  OLED_COMPINS,     0x02,                         // set com pins
  OLED_PAGES, 0x00, 0x03,                         // set min and max page
  OLED_XFLIP, OLED_YFLIP,                         // flip screen
  OLED_DISPLAY_ON                                 // display on
};

// OLED init function
void OLED_init(void) {
  uint8_t i;
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_CMD_MODE);                       // set command mode
  for(i = 0; i < sizeof(OLED_INIT_CMD); i++)
    I2C_write(OLED_INIT_CMD[i]);                  // send the command bytes
  I2C_stop();                                     // stop transmission
}

// OLED set cursor to digit position
void OLED_setCursor(uint8_t digit) {
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_CMD_MODE);                       // set command mode
  I2C_write(OLED_COLUMN_LOW | 0);                 // set xpos low nibble (always zero)
  I2C_write(OLED_COLUMN_HIGH | digit);            // set xpos high nibble
  I2C_write(OLED_PAGE | 0);                       // set ypos (always zero)
  I2C_stop();                                     // stop transmission
}

// OLED clear screen
void OLED_clear(void) {
  uint16_t i;
  OLED_setCursor(0);                              // set cursor to first digit
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  for(i=128*4; i; i--) I2C_write(0x00);              // clear screen
  I2C_stop();                                     // stop transmission
}

// OLED plot a single character
void OLED_plotChar(uint8_t c) {
  uint8_t i;
  uint16_t ptr = (uint16_t)c;                     // character pointer
  if(c == OLED_SPACE) {                           // SPACE?
    for(i=64; i; i--) I2C_write(0x00);            // plot space
    return;                                       // done
  }
  ptr = (ptr << 5) + (ptr << 4) + (ptr << 2);     // -> ptr = c * 13 * 4;
  for(i=52; i; i--) I2C_write(OLED_FONT[ptr++]);  // plot character
  for(i=12; i; i--) I2C_write(0x00);              // plot space between characters
}

// OLED write a single character
void OLED_write(uint8_t c) {
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  OLED_plotChar(c);                               // plot character
  I2C_stop();                                     // stop transmission
}

// OLED print value (BCD conversion by substraction method)
void OLED_printVal(uint16_t value, uint8_t digits, uint8_t decimal) {
  uint8_t leadflag = 0;                           // flag for leading spaces
  I2C_start(OLED_ADDR);                           // start transmission to OLED
  I2C_write(OLED_DAT_MODE);                       // set data mode
  while(digits--) {                               // for all digits digits
    uint8_t digitval = 0;                         // start with digit value 0
    uint16_t divider = DIVIDER[digits];           // read current divider
    while(value >= divider) {                     // if current divider fits into the value
      leadflag = 1;                               // end of leading spaces
      digitval++;                                 // increase digit value
      value -= divider;                           // decrease value by divider
    }
    if(digits == decimal) leadflag++;             // end leading spaces before decimal
    if(leadflag) OLED_plotChar(digitval);         // print the digit
    else         OLED_plotChar(OLED_SPACE);       // or print leading space
    if(decimal && (digits == decimal)) OLED_plotChar(OLED_DECIMAL);  // print decimal
  }
  I2C_stop();                                     // stop transmission
}