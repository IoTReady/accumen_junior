#!/bin/bash
python -m nuitka --plugin-enable=pylint-warnings --plugin-enable=numpy --plugin-enable=pkg-resources --onefile --linux-onefile-icon=icon.png barcode/scan.py -o barcode.bin
