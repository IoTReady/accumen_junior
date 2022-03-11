#!/bin/bash

full_path=$(realpath $0)
dir_path=$(dirname $full_path)
source $dir_path/venv/bin/activate
sudo $dir_path/venv/bin/python $dir_path/barcode/scan.py
