# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino Nano project for an automated chicken watering system. The system provides fresh, filtered water year-round with two main features:
1. **Automatic water pumping** - Pumps fresh water into the bowl at configurable intervals (e.g., hourly) using a 6V water pump
2. **Winter heating** - Maintains water temperature above a threshold during cold weather using a 12V heat pad controlled via relay, with temperature monitoring via digital temperature probe

Hardware components:
- Arduino Nano (ATmega328P)
- 6V water pump (controlled at 5V)
- 12V heat pad
- 5V → 12V relay
- Digital temperature probe with controller

## Development Commands

This project uses PlatformIO for Arduino development.

### Build
```bash
pio run
```

### Upload to device
```bash
pio run --target upload
```

### Clean build files
```bash
pio run --target clean
```

### Monitor serial output
```bash
pio device monitor
```

### Run tests
```bash
pio test
```

## Code Architecture

**Target board**: Arduino Nano (ATmega328P new bootloader)
**Framework**: Arduino
**Platform**: Atmel AVR

**Main source**: `src/main.cpp` - Contains the Arduino setup() and loop() functions. Currently implements a simple LED blink pattern.

**Project structure**:
- `src/` - Main source code
- `lib/` - Project-specific libraries
- `include/` - Header files
- `test/` - Test files
- `platformio.ini` - PlatformIO configuration (board, platform, framework)

## Implementation Notes

The current code is a basic template. The full implementation needs to:
- Control water pump on timed intervals
- Read temperature sensor data every ~5 minutes
- Control relay to activate/deactivate heat pad based on temperature threshold
- Configure timing intervals and temperature thresholds (likely via constants or config)
