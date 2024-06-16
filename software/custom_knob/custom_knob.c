// ===================================================================================
// Project:   USB Rotary Encoder for CH32V003 - Custom Control
// Version:   v1.0
// Year:      2024
// Author:    Stefan Wagner
// Github:    https://github.com/wagiminator
// EasyEDA:   https://easyeda.com/wagiminator
// License:   http://creativecommons.org/licenses/by-sa/3.0/
// ===================================================================================
//
// Description:
// ------------
// Customize your USB Rotary Encoder Knob.
//
// References:
// -----------
// - CNLohr ch32v003fun: https://github.com/cnlohr/ch32v003fun
// - CNLohr rv003usb:    https://github.com/cnlohr/rv003usb
// - WCH Nanjing Qinheng Microelectronics: http://wch.cn
//
// Compilation Instructions:
// -------------------------
// - Make sure GCC toolchain (gcc-riscv64-unknown-elf, newlib) and Python3 with PyUSB
//   is installed. In addition, Linux requires access rights to WCH-LinkE programmer.
// - Connect the WCH-LinkE programmer to the MCU board.
// - Run 'make flash'.
//
// Operating Instructions:
// -----------------------
// - Costomize your rotary encoder knob below in this firmware.
// - Connect the board via USB to your PC. It should be detected as a HID composite
//   device with keyboard, mouse and game controller interface.
// - Turn the rotary encoder or press the encoder switch.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include <config.h>                               // user configurations
#include <system.h>                               // system functions
#include <gpio.h>                                 // GPIO functions
#include <usb_composite.h>                        // USB HID Composite

// ===================================================================================
// Macro Functions which associate Actions with Events (Customize your USB Knob here!)
// ===================================================================================
//
// The list of available USB HID functions can be found in include/usb_composite.h

// Rotary encoder example -> volume control knob
// ---------------------------------------------

// Define action(s) if encoder was rotated clockwise
static inline void ENC_CW_ACTION() {
  CON_press(CON_VOL_UP);                                  // press VOLUME UP key
}

// Define action(s) after encoder was rotated clockwise
static inline void ENC_CW_RELEASED() {
  CON_release();                                          // release VOLUME UP KEY
}

// Define action(s) if encoder was rotated counter-clockwise
static inline void ENC_CCW_ACTION() {
  CON_press(CON_VOL_DOWN);                                // press VOLUME DOWN key
}

// Define action(s) after encoder was rotated counter-clockwise
static inline void ENC_CCW_RELEASED() {
  CON_release();                                          // release VOLUME DOWN KEY
}

// Define action(s) if encoder switch was pressed
static inline void ENC_SW_PRESSED() {
  CON_press(CON_VOL_MUTE);                                // press VOLUME MUTE key
}

// Define action(s) if encoder switch was released
static inline void ENC_SW_RELEASED() {
  CON_release();                                          // release VOLUME MUTE key
}

// ===================================================================================
// Main Function
// ===================================================================================
int main(void) {
  // Variables
  uint8_t isSwitchPressed = 0;                            // state of rotary encoder switch

  // Setup
  PIN_input_PU(PIN_ENC_A);                                // set encoder pins to input pullup
  PIN_input_PU(PIN_ENC_B);
  PIN_input_PU(PIN_ENC_SW);
  HID_init();                                             // init USB HID composite device

  // Loop
  while(1) {
    if(!PIN_read(PIN_ENC_A)) {                            // encoder turned ?
      if(PIN_read(PIN_ENC_B)) {                           // clockwise?
        ENC_CW_ACTION();                                  // -> take proper action
        DLY_ms(5);                                        // -> debounce
        ENC_CW_RELEASED();                                // -> quit action
      }
      else {                                              // counter-clockwise?
        ENC_CCW_ACTION();                                 // -> take proper action
        DLY_ms(5);                                        // -> debounce
        ENC_CCW_RELEASED();                               // -> quit action
      }
      while(!PIN_read(PIN_ENC_A));                        // wait until next detent
    } 
    else {
      if(!isSwitchPressed && !PIN_read(PIN_ENC_SW)) {     // switch previously pressed?
        ENC_SW_PRESSED();                                 // -> take proper action
        isSwitchPressed = 1;                              // -> update switch state
      }
      else if(isSwitchPressed && PIN_read(PIN_ENC_SW)) {  // switch previously released?
        ENC_SW_RELEASED();                                // -> take proper action
        isSwitchPressed = 0;                              // -> update switch state
      }
    }
    DLY_ms(1);                                            // slow down a little
  }
}
