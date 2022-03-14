#!/bin/bash

full_path=$(realpath $0)
dir_path=$(dirname $full_path)
sudo $dir_path/barcode.bin
