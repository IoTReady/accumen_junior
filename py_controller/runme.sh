#!/bin/bash

source ./venv/bin/activate
./venv/bin/python app.py &
sudo ./venv/bin/python barcode/scan.py 