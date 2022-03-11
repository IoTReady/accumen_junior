#!/bin/bash

# Uncomment lines below for clean flash
esptool.py --chip esp32 --port /dev/ttyUSB0 erase_flash
esptool.py --chip esp32 --port /dev/ttyUSB0 --baud 460800 write_flash -z 0x1000 esp32-20220117-v1.18.bin
sleep 5

ampy -p /dev/ttyUSB0 put lcd_api.py
echo "Uploaded lcd_api.py"
ampy -p /dev/ttyUSB0 put esp32_i2c_lcd.py
echo "Uploaded esp32_i2c_lcd.py"
ampy -p /dev/ttyUSB0 put uping.py
echo "Uploaded uping.py"
ampy -p /dev/ttyUSB0 put shutil.py
echo "Uploaded shutil.py"

# Upload main.py
ampy -p /dev/ttyUSB0 put app.py app_factory.py
echo "Uploaded app_factory.py"
ampy -p /dev/ttyUSB0 put app.py app_previous.py
echo "Uploaded app_previous.py"
ampy -p /dev/ttyUSB0 put app.py app.py
echo "Uploaded app.py"
ampy -p /dev/ttyUSB0 put main.py
echo "Uploaded main.py"

esptool.py --chip esp32 --port /dev/ttyUSB0 read_mac
