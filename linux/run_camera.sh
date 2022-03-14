#!/bin/bash

full_path=$(realpath $0)
dir_path=$(dirname $full_path)
$dir_path/camera.bin --device 0 --path /home/ccmsadmin/workspace/original_image
