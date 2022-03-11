#!/bin/bash

sudo /usr/local/bin/accumen.sh
source ./venv/bin/activate
./venv/bin/python app.py --device 0 --path /home/ccmsadmin/workspace/original_image &
sudo ./venv/bin/python barcode/scan.py 