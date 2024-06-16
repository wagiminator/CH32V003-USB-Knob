// ===================================================================================
// User Configurations
// ===================================================================================

#pragma once

// Rotary encoder in definitions
#define PIN_ENC_A           PC1       // pin connected to rotary encoder A
#define PIN_ENC_B           PC2       // pin connected to rotary encoder B
#define PIN_ENC_SW          PC4       // pin connected to rotary encoder switch

// USB pin definitions
#define USB_PORT            A         // [A,C,D] GPIO Port to use with D+ and D-
#define USB_PIN_DP          1         // [0-4] GPIO Number for USB D+ Pin
#define USB_PIN_DM          2         // [0-4] GPIO Number for USB D- Pin

// USB configuration descriptor
#define USB_MAX_POWER_mA    50        // max power in mA 

// USB device descriptor
#define USB_VENDOR_ID       0x1209    // VID
#define USB_PRODUCT_ID      0xc003    // PID
#define USB_DEVICE_VERSION  0x0100    // v1.0 (BCD-format)
#define USB_LANGUAGE        0x0409    // US English

// USB descriptor strings
#define MANUF_STR           "wagiminator"
#define PROD_STR            "USB Knob"
#define SERIAL_STR          "CH32V003HID"
