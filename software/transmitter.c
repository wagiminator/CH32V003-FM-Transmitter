// ===================================================================================
// Project:   CH32V003 + KT0803 K/L FM Transmitter
// Version:   v1.0
// Year:      2023
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// Pocket Stereo FM Transmitter.
//
// References:
// -----------
// - CNLohr ch32v003fun: https://github.com/cnlohr/ch32v003fun
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-riscv64-unknown-elf, newlib) and Python3 with PyUSB
//   is installed. In addition, Linux requires access rights to WCH-LinkE programmer.
// - Connect the WCH-LinkE programmer to the PROG-header of the device.
// - Run 'make flash'.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <system.h>           // system functions
#include <gpio.h>             // GPIO functions
#include <kt0803.h>           // KT0803 functions
#include <oled_segment.h>     // OLED functions

#define PIN_SW    PA2         // KT0803 switch on/off
#define PIN_RST   PA1         // KT0803 reset (active low)
#define PIN_KEYS  PC4         // Control keys

uint8_t  display = 0;         // current display/control mode (0: frequency, 1: gain)
uint8_t  gain = 3;            // current gain (0..6)
uint16_t freq = 988;          // current frequency (in 100kHz steps, 988 means 98.8Mhz) 

// ===================================================================================
// Button Read Function
// ===================================================================================
enum { KEY_NO, KEY_UP, KEY_OK, KEY_DOWN };
uint16_t KEY_ADC[] = { 850, 590, 250, 0 };

uint8_t KEY_read(void) {
  uint8_t  ckey = 0;
  uint16_t ckeyval = ADC_read();
  while(ckeyval < KEY_ADC[ckey]) ckey++;
  return ckey;
}

// ===================================================================================
// OLED Update Function
// ===================================================================================
void OLED_update(void) {
  // Display current volume gain level
  if(display) {
    if(gain < 3) OLED_write(OLED_MINUS);
    else         OLED_write(OLED_PLUS);
    if      (gain == 3)                 OLED_printVal( 0, 4, 0);
    else if((gain == 0) || (gain == 6)) OLED_printVal(12, 4, 0);
    else if((gain == 1) || (gain == 5)) OLED_printVal( 8, 4, 0);
    else                                OLED_printVal( 4, 4, 0);
    OLED_write(OLED_SPACE);
    OLED_write(OLED_LETTER_d);
    OLED_write(OLED_LETTER_b);
  }

  // Display current transmitter frequency
  else {
    OLED_printVal(freq, 4, 1);
    OLED_write(OLED_LETTER_M);
    OLED_write(OLED_LETTER_H);
    OLED_write(OLED_LETTER_z);
  }
}

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Lokal variables
  uint8_t  i;
  uint8_t  key;
  uint8_t  keydelay;

  // Setup pins
  PIN_output(PIN_SW);
  PIN_output(PIN_RST);
  PIN_input_AN(PIN_KEYS);
  PIN_high(PIN_SW);
  PIN_high(PIN_RST);

  // Setup internal peripherals
  I2C_init();
  ADC_init();
  ADC_slow();
  ADC_input(PIN_KEYS);

  // Setup external peripherals
  OLED_init();
  OLED_clear();
  OLED_update();
  DLY_ms(500);
  KT_setFreq(freq);
  KT_setGain(gain);

  // Loop
  while(1) {
    // Read current key
    key = KEY_read();

    // Volume gain display/control mode
    if(display) {
      switch(key) {
        case KEY_UP:    if(gain < 6) KT_setGain(++gain); break;
        case KEY_DOWN:  if(gain > 0) KT_setGain(--gain); break;
        case KEY_OK:    display = 0; break;
        default:        break;
      }
      if(key) {
        OLED_update();
        while(KEY_read());
        DLY_ms(10);
      }
    }

    // Transmitter frequency display/control mode
    else {
      switch(key) {
        case KEY_UP:    freq++; break;
        case KEY_DOWN:  freq--; break;
        case KEY_OK:    display++; OLED_update(); while(KEY_read()); DLY_ms(10); break;
        default:        keydelay = 50; break;
      }
      if(freq != KT_getFreq()) {
        if(freq < KT_FREQ_MIN) freq = KT_FREQ_MAX;
        if(freq > KT_FREQ_MAX) freq = KT_FREQ_MIN;
        KT_setFreq(freq);
        OLED_update();
        i = keydelay;
        while((i--) && (KEY_read())) DLY_ms(10);
        keydelay = 5;
      }
    }
  }
}
