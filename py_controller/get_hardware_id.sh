#!/bin/bash

full_path=$(realpath $0)
dir_path=$(dirname $full_path)
source $dir_path/venv/bin/activate
$dir_path/venv/bin/python $dir_path/get_hardware_id.py $HOME/hardware_id.txt
