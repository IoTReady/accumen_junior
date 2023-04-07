#!/bin/bash
python -m nuitka --plugin-enable=pylint-warnings --onefile --linux-onefile-icon=icon.png app.py -o camera.bin
