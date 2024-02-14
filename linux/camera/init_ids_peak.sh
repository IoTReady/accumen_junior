#!/bin/bash
PEAK_PATH="$(readlink -f "$(dirname "${BASH_SOURCE[0]}")")"
export PEAK_PATH
echo PEAK_PATH:$PEAK_PYTHON
#export LD_LIBRARY_PATH="/home/iotready/code/play/ids_peak_python/ids-peak_2.6.1.0-16200_amd64/lib/"
export LD_LIBRARY_PATH="/home/iotready/code/play/ids_peak_python/ids-peak_2.6.1.0-16200_amd64/lib/"
echo LD_LIBRARY_PATH:$LD_LIBRARY_PATH
### you should see the .so files (dynamic library linking)
export GENICAM_GENTL32_PATH="$GENICAM_GENTL32_PATH:$PEAK_PATH/../lib/ids/cti"
export GENICAM_GENTL64_PATH="$GENICAM_GENTL64_PATH:$PEAK_PATH/../lib/ids/cti"


commit_hash=`git rev-parse --short=7 HEAD`
echo $commit_hash
python -m nuitka --plugin-enable=pylint-warnings --onefile --linux-onefile-icon=icon.png app.py -o "camera_$commit_hash.bin"
#python3 camera_ids_cli.py
