// ===================================================================================
// Project:   USB Rotary Encoder for CH32V003 - Mouse Wheel
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
// Turns the rotary encoder into a mouse wheel.
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
// - Connect the board via USB to your PC. It should be detected as a HID device.
// - Use the rotary encoder like the wheel on your mouse.


// ===================================================================================
// Libraries, Definitions and Macros
// ===================================================================================
#include "gpio.h"                     // GPIO functions
#include "usb_mouse.h"                // USB HID mouse functions

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
  MOUSE_init();                                           // init USB HID mouse

  // Loop
  while(1) {
    if(!PIN_read(PIN_ENC_A)) {                            // encoder turned ?
      if(PIN_read(PIN_ENC_B)) {                           // clockwise?
        MOUSE_wheel(-1);                                  // -> turn mousewheel down
      }
      else {                                              // counter-clockwise?
        MOUSE_wheel(1);                                   // -> turn mousewheel up
      }
      DLY_ms(10);                                         // debounce
      while(!PIN_read(PIN_ENC_A));                        // wait until next detent
    } 
    else {
      if(!isSwitchPressed && !PIN_read(PIN_ENC_SW)) {     // switch previously pressed?
        MOUSE_press(MOUSE_BUTTON_MIDDLE);                 // press mouse wheel button
        isSwitchPressed = 1;
      }
      else if(isSwitchPressed && PIN_read(PIN_ENC_SW)) {  // switch previously released?
        MOUSE_release(MOUSE_BUTTON_MIDDLE);               // release mouse wheel button
        isSwitchPressed = 0;                              // update switch state
      }
    }
    DLY_ms(1);                                            // slow down a little
  }
}
