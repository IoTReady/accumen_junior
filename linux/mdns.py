import logging
import socket
from time import sleep
from zeroconf import IPVersion, ServiceInfo, Zeroconf
from zeroconf._utils import ipaddress

log = logging.getLogger(__name__)


def lan_ip_address():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ip = s.getsockname()[0]
    s.close()
    return ip


def init_service(host=None, port=8000):
    log.info("Registering service.")
    ip_version = IPVersion.V4Only
    desc = {"path": "/"}
    service = ServiceInfo(
        "_accumen._tcp.local.",
        "camera._accumen._tcp.local.",
        addresses=[socket.inet_aton(host or lan_ip_address())],
        port=port,
        properties=desc,
        server="accumen.local.",
    )
    zeroconf = Zeroconf(ip_version=ip_version)
    zeroconf.register_service(service)
    return zeroconf, service


if __name__ == "__main__":
    logging.basicConfig(level=logging.DEBUG)
    zeroconf, service = init_service()
    try:
        while True:
            sleep(0.1)
    except KeyboardInterrupt:
        pass
    finally:
        print("Unregistering...")
        zeroconf.unregister_service(service)
        zeroconf.close()
