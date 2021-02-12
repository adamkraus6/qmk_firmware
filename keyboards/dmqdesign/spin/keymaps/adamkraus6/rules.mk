# MCU name
MCU = atmega32u4

# Bootloader selection
#   Teensy       halfkay
#   Pro Micro    caterina
#   Atmel DFU    atmel-dfu
#   LUFA DFU     lufa-dfu
#   QMK DFU      qmk-dfu
#   ATmega32A    bootloadHID
#   ATmega328P   USBasp
BOOTLOADER = atmel-dfu

# Build Options
#   change yes to no to disable
#
EXTRAKEY_ENABLE    = yes     # Audio control and System control
NKRO_ENABLE        = yes     # USB Nkey Rollover
ENCODER_ENABLE     = yes     # Enable rotary encoder support
LTO_ENABLE         = yes
OLED_DRIVER_ENABLE = yes     # .91" I2C OLED

EXTRAFLAGS += -flto
