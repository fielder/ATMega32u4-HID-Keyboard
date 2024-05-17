#ifndef __USB_H__
#define __USB_H__

#include <stdint.h>

/* only 6 slots available in the USB report */
extern volatile uint8_t usb_keyboard_pressed_keys[6];
#define REPORT_KEY_SLOTS (sizeof(usb_keyboard_pressed_keys) / sizeof(usb_keyboard_pressed_keys[0]))
extern volatile uint8_t usb_keyboard_modifier;
extern volatile uint8_t usb_config_status;

int
usb_init ();

int
usb_send ();

#define GET_STATUS 0x00
#define CLEAR_FEATURE 0x01
#define SET_FEATURE 0x03
#define SET_ADDRESS 0x05
#define GET_DESCRIPTOR 0x06
#define GET_CONFIGURATION 0x08
#define SET_CONFIGURATION 0x09
#define GET_INTERFACE 0x0A
#define SET_INTERFACE 0x0B

#define idVendor 0x03eb  // Atmel Corp.
#define idProduct 0x2ff4  // ATMega32u4 DFU Bootloader (This isn't a real product so I don't
          // have legitimate IDs)
#define KEYBOARD_ENDPOINT_NUM 3  // The second endpoint is the HID endpoint

#define CONFIG_SIZE 34
#define HID_OFFSET 18

// HID Class-specific request codes - refer to HID Class Specification
// Chapter 7.2 - Remarks

#define GET_REPORT 0x01
#define GET_IDLE 0x02
#define GET_PROTOCOL 0x03
#define SET_REPORT 0x09
#define SET_IDLE 0x0A
#define SET_PROTOCOL 0x0B

#endif /* __USB_H__ */
