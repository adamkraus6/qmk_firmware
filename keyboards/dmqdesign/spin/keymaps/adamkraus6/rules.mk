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
MOUSEKEY_ENABLE    = yes     # Mouse keys
EXTRAKEY_ENABLE    = yes     # Audio control and System control
NKRO_ENABLE        = yes     # USB Nkey Rollover
ENCODER_ENABLE     = yes     # Enable rotary encoder support

BOOTMAGIC_ENABLE   = lite   # Virtual DIP switch configuration
LTO_ENABLE         = yes
CONSOLE_ENABLE     = yes    # Console for debug

OLED_DRIVER_ENABLE = no     # .91" I2C OLED