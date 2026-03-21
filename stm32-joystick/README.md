# Stm32 Gamepad

Uses STM32F411's usb otg support to emulate a hid joystick.

## pins

A0/A1 -> joystick analog Y and X
B0/B1 -> digital buttons

## debug

set `#define DEBUG_LOG true`  
and use uart to usb on PA9 (this is stm32 TX, so tty's RX)

## build

```sh
pio run -t upload
```

currently uses dap link (so like a flipper in the usb dap app)
