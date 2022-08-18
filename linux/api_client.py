import json
import requests
import getmac
from time import sleep
from os import environ

base_url = "http://192.168.10.1"
username = "B12345"
password = "Callme@11"

try:
    interface = environ.get('INTERFACE')
except:
    interface = 'eno1'

def login():
    url = f"{base_url}:9090/CCMS/auth/login"
    payload = {"username": username, "password": password}
    headers = {"Content-Type": "application/json", "Accept": "application/json"}
    res = requests.post(url, data=json.dumps(payload), headers=headers)
    print(res.text)
    return res.json()


def get_access_token():
    res = login()
    if res.get('session_exist'):
        logout()
        res = login()
    sleep(1)
    return res.get("access_token")


def get_refresh_token():
    res = login()
    if res.get('session_exist'):
        logout()
        res = login()
    sleep(1)
    return res.get("refresh_token")


def validate_image(image_path, barcode=None):
    # res = login()
    headers = {
        "Content-Type": "application/json",
        "Accept": "application/json",
        # "Authorization": f"Bearer {res.get('access_token')}",
        "Current-Role": "ROLE_ANALYST",
    }
    req = {
        "barcode": barcode,
        "imagePath": image_path,
        "hardwareType": "Junior",
        "hardwareId": getmac.get_mac_address(interface=interface).upper(),
        "userId": username,
    }
    print("validate_image req", req)
    url = "http://localhost:9099/ccms/validate/image"
    res = requests.post(url, data=json.dumps(req), headers=headers)
    print(res.text)
    return res.json()


def validate_barcode(barcode):
    url = f"{base_url}:9099/ccms/validate/barcode"
    res = login()
    headers = {
        # "Content-Type": "application/json",
        # "Accept": "application/json",
        "Authorization": f"Bearer {res.get('access_token')}",
        "Current-Role": "ROLE_ANALYST",
    }
    res = requests.post(url, data=barcode, headers=headers)
    print(res.text)
    return True


def logout():
    url = f"{base_url}:9090/CCMS/auth/logout"
    res = login()
    headers = {
        "Content-Type": "application/json",
        "Accept": "application/json",
        "Authorization": f"Bearer {res.get('access_token')}",
        "Current-Role": "ROLE_ANALYST",
    }
    res = requests.post(url, headers=headers)
    print(res.text)
    return res.json()


def refresh_token():
    url = f"{base_url}:9090/CCMS/auth/refresh"
    res = login()
    payload = {
        "grant_type": "refresh_token",
        "refresh_token": res.get('refresh_token'),
    }
    headers = {
        "Content-Type": "application/json",
        "Accept": "application/json",
        "Authorization": f"Bearer {res.get('access_token')}",
        "Current-Role": "ROLE_ANALYST",
    }
    res = requests.post(url, data=json.dumps(payload), headers=headers)
    print(res.text)
    return res.json()


if __name__ == "__main__":
    #res = logout()
    #barcode = "bar_2802_02"
    #res = validate_barcode(barcode)
    # res = refresh_token()
    #print(res)
    mac = getmac.get_mac_address(interface=interface).upper()
    print(mac)
    get_access_token()
    validate_image("/dummy/image", barcode=None)

