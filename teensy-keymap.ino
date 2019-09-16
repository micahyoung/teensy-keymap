// derived from "mouse" example of USBHost_t36 library
// simple relaying of keystrokes between a keyboard connected to the
// usb host port of a teensy 3.6 and the output of the teensy
// acting as a usb keyboard

/*
  The Teensy 3.6 has a USB host port as well as the usual USB device port
  so it can act as a USB pass through or proxy device.

  The pull request below (not mine) allows a sketch to receive the
  USB keyboard HID report when it arrives from the USB host port.
  This version does not require changes to Keyboard.

  Latest usbhost_t36 from git
  https://github.com/PaulStoffregen/USBHost_t36

  with this pull request applied
  https://github.com/PaulStoffregen/USBHost_t36/pull/18

  Hardware
    Teensy 3.6 board
    https://www.pjrc.com/store/teensy36.html
    Teensy 3.6 USB host cable
    https://www.pjrc.com/store/cable_usb_host_t36.html
*/

#include "USBHost_t36.h"  // to host a usb keyboard
#include "Keyboard.h"     // to act as a usb keyboard device

USBHost myusb;
USBHub hub1(myusb);
KeyboardController keyboard1(myusb);
USBHIDParser hid1(myusb);

/*
    Swap LeftCtrl and CapsLock. Put CapsLock in the corner where it belongs!
  Key remap possiblities
    Switchable QWERTY, Dvorak, and colemak keyboard layouts
    Hardware key macros
    Remappings and macros that work even in BIOS and recovery mode.
    Remappings and macros that work with KVMs and game consoles.
*/
void reportReader(const uint8_t report[8])
{
  uint32_t mod1 = report[0];
  // uint8_t ign1 = report[1]; //unused
  uint8_t key1 = report[2];
  uint8_t key2 = report[3];
  uint8_t key3 = report[4];
  uint8_t key4 = report[5];
  uint8_t key5 = report[6];
  uint8_t key6 = report[7];

  Serial.printf(
    "mod1: %d\n1: %d\n2: %d\n3: %d\n4: %d\n5: %d\n6: %d\n7: %d\n----\n",
    mod1,
    report[1],
    report[2],
    report[3],
    report[4],
    report[5],
    report[6],
    report[7]
  );

  //remap capslock to ctrl
  if (key1 == KEY_CAPS_LOCK) {
    key1 = 0;
    mod1 = MODIFIERKEY_CTRL; //ctrl
  }

  Keyboard.set_modifier(mod1);
  Keyboard.set_key1(key1);
  Keyboard.set_key2(key2);
  Keyboard.set_key3(key3);
  Keyboard.set_key4(key4);
  Keyboard.set_key5(key5);
  Keyboard.set_key6(key6);
  Keyboard.send_now();
}

void setup()
{
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("Serial ready");
  myusb.begin();
  keyboard1.attachReportReader(reportReader);
}

void loop() {
  myusb.Task();
}
