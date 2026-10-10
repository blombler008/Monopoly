# Monopoly

A custom Monopoly-inspired game project built for an ESP32-S3 development board. The system combines a TFT display, RFID readers, keypad input, SD storage, and audio playback to create a self-contained tabletop game experience.

## Overview

This project is designed around an ESP32-S3 board running Arduino/PlatformIO. It uses a touchscreen-style display and hardware input devices to simulate a Monopoly-like game flow with card scanning, board interaction, and custom UI elements.

The repository includes:

- ESP32 firmware for the game logic and hardware control
- PlatformIO configuration for building and flashing the device
- Custom board/system modules for RFID, keypad, display, and audio
- SD card support for persistent game data and assets
- Submodule-based dependencies for GUI and RFID support

## Features

- ESP32-S3 based embedded firmware
- RFID-based player/card interaction
- I2C keypad input support
- Display rendering with LVGL and TFT_eSPI
- Audio playback via ESP32-audioI2S
- SD card data access and storage
- OTA-capable release configuration
- Partition management for SPIFFS-based filesystem setup

## Hardware

The project is configured for:

- ESP32-S3-N16R8
- Arduino framework
- TFT display
- RFID readers
- I2C keypad
- SD card module
- Audio output

The main configuration is defined in `platformio.ini` and targets the `release` environment by default.

## Repository structure

```text
.
├── .gitignore
├── .gitmodules
├── compile_commands.json
├── partition_manager.py
├── platformio.ini
├── data/
├── fonts/
├── libs/
├── src/
└── README.md
```

Key directories:

- `src/` - main firmware source code
  - `audio/` - audio logic and setup
  - `components/` - reusable game/application components
  - `core/` - core system behavior
  - `gui/` - UI and display-related code
  - `helpers/` - utility/helper code
  - `includes/` - shared includes and configuration headers
  - `keypad/` - keypad handling
  - `rfid/` - RFID processing
  - `utils/` - helper and support utilities
- `libs/` - external dependencies and submodules
- `data/` - game asset/data files
- `fonts/` - font resources

## Dependencies

This project uses PlatformIO and the following main libraries:

- `bodmer/TFT_eSPI`
- `esphome/ESP32-audioI2S`
- `robtillaart/I2CKeyPad`
- `lvgl/lvgl`
- `bblanchon/ArduinoJson`

It also includes submodules:

- `libs/rfid`
- `libs/Monopoly_GUI`

## Building and running

Requirements:

- PlatformIO
- VS Code with PlatformIO extension, or PlatformIO Core CLI
- ESP32-S3 compatible development environment

From the project root, run:

```bash
pio run
```

For the default release build:

```bash
pio run -e release
```

To upload to the board:

```bash
pio run -e release -t upload
```

To monitor serial output:

```bash
pio device monitor
```

## Development notes

The firmware initializes:

- SPI interfaces
- display and GUI loop
- RFID readers
- keypad input
- SD card storage
- optional audio subsystem

The main entry point is `src/main.cpp`, where the application setup and loop are defined.

## License

This repository does not currently declare a license in the root files. If you plan to publish or share the project more broadly, consider adding an explicit license such as MIT or GPL.

## Contribution

If you are extending this project, keep the board-specific setup, pin mappings, and build configuration in sync with the PlatformIO configuration and the firmware modules.

---

This README is a practical starting point for the project and can be expanded with gameplay details, wiring diagrams, screenshots, and usage instructions as the project evolves.
