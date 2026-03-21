#include <Arduino.h>
#include <HID.h>
#include <Joystick.h>

// Config
#define PIN_VRX PA0
#define PIN_VRY PA1
#define PIN_SW1 PB0
#define PIN_SW2 PB1

#define ANALOG_MAX 4096

enum class USBState {
  Connected,
  Disconnected,
};

Joystick_ joystick;
USBState usb_state = USBState::Connected;
HardwareSerial logging(PA10, PA9);

void setup() {
  logging.begin(115200);
  logging.println("app started!");

  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  analogReadResolution(12);

  USB_Begin();
  logging.println("USB setup complete");
}

void loop() {
  if (!USB_Running()) {
    if (usb_state == USBState::Connected) {
      logging.println("USB Disconnected");
      usb_state = USBState::Disconnected;
    }

    delay(5);
    return;
  }

  if (usb_state != USBState::Connected) {
    usb_state = USBState::Connected;
    logging.println("USB Connected");

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

  // update report
  joystick.setYAxis(y_axis);
  joystick.setXAxis(x_axis);
  joystick.setButton(0, sw1_pressed);
  joystick.setButton(1, sw2_pressed);
  joystick.sendState();

  // log for debugging
  logging.printf("%lu, %lu, %d, %d\r\n", y_axis, x_axis, sw1_pressed,
                 sw2_pressed);

  delay(10);
}
