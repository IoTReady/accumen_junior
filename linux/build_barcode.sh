#!/bin/bash
commit_hash=`git rev-parse --short=7 HEAD`
echo $commit_hash
python -m nuitka --plugin-enable=pylint-warnings --onefile --linux-onefile-icon=icon.png barcode/scan.py -o "barcode_$commit_hash.bin"
