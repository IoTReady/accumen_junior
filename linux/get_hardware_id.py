import sys
import getmac

if len(sys.argv) > 2:
    interface = sys.argv[2]
else:
    interface = "eno1"

hardware_id = getmac.get_mac_address(interface=interface).upper()

with open(sys.argv[1], "w") as f:
    f.write(hardware_id+"\n")
