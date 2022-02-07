#!/bin/bash

sudo /usr/local/bin/accumen.sh
source ./venv/bin/activate
./venv/bin/python app.py --device 2 &
sudo ./venv/bin/python barcode/scan.py 