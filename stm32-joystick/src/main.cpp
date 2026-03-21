#include <Arduino.h>
#include <HID.h>
#include <Joystick.h>
#include <stdlib.h>

// Config
#define PIN_VRX PA0
#define PIN_VRY PA1
#define PIN_SW1 PB0
#define PIN_SW2 PB1

#define ANALOG_MAX 4096
#define ADC_DEADZONE 32
#define DEBUG_LOGGING false

enum class USBState {
  Connected,
  Disconnected,
};

Joystick_ joystick;
USBState usb_state = USBState::Connected;
HardwareSerial logging(PA10, PA9);

// fwd declare
void apply_deadzone(uint32_t &value);

void setup() {
#if DEBUG_LOGGING
  logging.begin(115200);
  logging.println("app started!");
#endif

  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  analogReadResolution(12);

  USB_Begin();
}

void loop() {
  if (!USB_Running()) {
    if (usb_state == USBState::Connected) {
#if DEBUG_LOGGING
      logging.println("USB Disconnected");
#endif
      usb_state = USBState::Disconnected;
    }

    delay(5);
    return;
  }

  if (usb_state != USBState::Connected) {
    usb_state = USBState::Connected;
#if DEBUG_LOGGING
    logging.println("USB Connected");
#endif

    // we will manually send reports at end
    // otherwise each .setXXX call would send a new report.
    joystick.begin(false);
    joystick.setYAxisRange(0, ANALOG_MAX);
    joystick.setXAxisRange(0, ANALOG_MAX);
  }

  // Poll sensors
  // buttons pull to gnd
  // and axis are inverted
  uint32_t y_axis = ANALOG_MAX - analogRead(PIN_VRY);
  uint32_t x_axis = ANALOG_MAX - analogRead(PIN_VRX);
  bool sw1_pressed = !digitalRead(PIN_SW1);
  bool sw2_pressed = !digitalRead(PIN_SW2);

  // apply deadzone
  apply_deadzone(y_axis);
  apply_deadzone(x_axis);

  // update report
  joystick.setYAxis(y_axis);
  joystick.setXAxis(x_axis);
  joystick.setButton(0, sw1_pressed);
  joystick.setButton(1, sw2_pressed);
  joystick.sendState();

#if DEBUG_LOGGING
  // log for debugging
  logging.printf("%lu, %lu, %d, %d\r\n", y_axis, x_axis, sw1_pressed,
                 sw2_pressed);
#endif

  delay(10);
}

void apply_deadzone(uint32_t &value) {
  int midpoint = ANALOG_MAX / 2;
  if (abs((int)value - midpoint) < ADC_DEADZONE) {
    value = midpoint;
  }
}
