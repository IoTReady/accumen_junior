import sys
import getmac

hardware_id = getmac.get_mac_address().upper()

with open(sys.argv[1], "w") as f:
    f.write(hardware_id+"\n")