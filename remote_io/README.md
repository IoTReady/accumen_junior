# Firmware For aCCumen Junior Remote IO

## Description

- Based on existing Halol POC unit
- Waits for trigger from limit switch and sends trigger via POST API to Linux
- Uses a 20x4 LCD display, e.g. Newhaven NHD-0420DZ-FSW-FBW
- Uses array of WS2812B illumination LEDs
- Uses 2 indicator LEDs
- Includes a single `config.json` file for configuration
- Includes factory reset and OTA

## Workflow
- Configure static Ethernet IP address and confirm connectivity
- Set up display, limit switch, illumination and indicator LEDs
- Check for OTA
- In an infinite loop, wait for limit switch trigger with a short (e.g. 200ms) sleep.
- Every 2s, check if Linux machine is still connected - if not, change the display.
- If limit switch triggered, enter capture workflow and change LED and display status messages accordingly
