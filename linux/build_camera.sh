#!/bin/bash
commit_hash=`git rev-parse --short=7 HEAD`
echo $commit_hash
#python -m nuitka --plugin-enable=pylint-warnings --include-package=zeroconf._handlers --onefile --linux-onefile-icon=icon.png app.py -o "camera_$commit_hash.bin"
python3 -m nuitka --follow-imports app.py -o "camera_$commit_hash.bin"
