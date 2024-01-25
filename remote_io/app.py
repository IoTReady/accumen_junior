import gc
import json
import urequests as requests
import network
import neopixel
from machine import SoftI2C, Pin, unique_id, reset, Timer
from utime import sleep_ms, ticks_ms
from esp32_i2c_lcd import I2cLcd
from uping import ping

# DO NOT CHANGE SPACING OR QUOTES IN THIS VERSION LINE
# OTA API RELIES ON THIS.
VERSION = "1.1.8"
lan = None

is_connected = False
is_triggering = False

config = {
    "ip": "192.168.10.2",
    "gateway": "192.168.10.1",
    "netmask": "255.255.255.0",
    "base_url": "http://192.168.10.1:8000",
    "limitswitch": 36,
    "status_led_1": 2,
    "status_led_2": 12,
    "neopixel": 5,
    "num_pixels": 120,
    "pixel_delay": 50,
    "i2c_sda": 13,
    "i2c_scl": 16,
    "lcd_address": 0x27,
    "main_loop_delay_ms": 100,
    "lcd_cols": 20,
    "lcd_rows": 4,
    "connection_row": 0,
    "status_row_1": 1,
    "status_row_2": 2,
    "status_row_3": 3,
    "healthcheck_interval": 5,
}

mac_address = "".join("{:02x}".format(d) for d in unique_id()).upper()

status_led_1 = Pin(int(config.get("status_led_1")), Pin.OUT)
status_led_2 = Pin(int(config.get("status_led_2")), Pin.OUT)
neopixel_pin = Pin(int(config.get("neopixel")), Pin.OUT)
limitswitch = Pin(int(config.get("limitswitch")), Pin.IN, Pin.PULL_UP)
scl = Pin(int(config.get("i2c_scl")), Pin.OUT)
sda = Pin(int(config.get("i2c_sda")), Pin.OUT)
i2c = SoftI2C(scl=scl, sda=sda)

# LCD
lcd = I2cLcd(
    i2c,
    config.get("lcd_address"),
    int(config.get("lcd_rows")),
    int(config.get("lcd_cols")),
)
# Checkmark
lcd.custom_char(0, bytearray([0x0, 0x1, 0x3, 0x16, 0x1C, 0x8, 0x0, 0x0]))

# Neopixel array
neopixel_array = neopixel.NeoPixel(neopixel_pin, config.get("num_pixels"))

last_interrupt_time = 0
previous_trigger_ticks = 0
trigger_interval = 1000

LOG_LEVELS = {"info": "I:", "debug": "D:", "warn": "W:", "error": "E:"}


def logprint(level, text):
    prefix = LOG_LEVELS.get(level) or "D:"
    print(prefix, text)
    try:
        base_url = config.get("base_url")
        url = "{}/log".format(base_url)
        payload = {"level": level, "text": text}
        headers = {"Content-Type": "application/json", "Accept": "application/json"}
        res = requests.post(url, data=json.dumps(payload), headers=headers)
        # print("D:", res.text)
    except Exception as e:
        print("E:", str(e))


def limitswitch_isr(pin: Pin):
    global last_interrupt_time
    interrupt_time = ticks_ms()
    if interrupt_time - last_interrupt_time > 1000:
        trigger()
    last_interrupt_time = interrupt_time


limitswitch.irq(limitswitch_isr, trigger=Pin.IRQ_FALLING)


def set_status_led(led: Pin, state: bool):
    led.value(state)


def sync_tray_led():
    # Because our inputs are inverted. Low on limitswitch = closed => LED1 = OFF. High on limitswitch = open => LED1 = ON
    tray_status = limitswitch.value()
    set_status_led(status_led_1, tray_status)


def switch_off_neopixels():
    neopixel_array.fill((0, 0, 0))
    neopixel_array.write()


def switch_on_neopixels(r_value=40, g_value=50, b_value=40):
    switch_off_neopixels()
    sleep_ms(500)
    # positions = [0, 1, 2, 14, 15, 16, 28, 29, 30, 42, 43, 44]
    for i in range(120):
        neopixel_array[i] = (r_value, g_value, b_value)
    neopixel_array.write()


# Display functions
def clear_lcd_line(line_num: int):
    lcd.move_to(0, line_num)
    lcd.putstr(" " * config.get("lcd_cols"))


def clear_status_rows():
    clear_lcd_line(config.get("status_row_1"))
    clear_lcd_line(config.get("status_row_2"))
    clear_lcd_line(config.get("status_row_3"))
    gc.collect()


def display_brandname():
    row = config.get("status_row_1")
    clear_lcd_line(row)
    lcd.move_to(0, row)
    lcd.putstr("aCCumen Jr")
    gc.collect()


def display_status(status):
    row = config.get("status_row_3")
    clear_lcd_line(row)
    lcd.move_to(0, row)
    lcd.putstr(status)
    gc.collect()


def display_waiting():
    row = config.get("status_row_3")
    clear_lcd_line(row)
    lcd.move_to(0, row)
    lcd.putstr("Insert Petri Dish")
    gc.collect()


def display_error():
    row = config.get("status_row_3")
    clear_lcd_line(row)
    lcd.move_to(0, row)
    lcd.putstr("Capture Error")
    gc.collect()


def display_triggered():
    row = config.get("status_row_3")
    clear_lcd_line(row)
    lcd.move_to(0, row)
    lcd.putstr("Imaging in Progress")
    gc.collect()


def display_captured():
    row = config.get("status_row_3")
    clear_lcd_line(row)
    lcd.move_to(0, row)
    lcd.putstr("Imaging Complete")
    gc.collect()


def display_ota_updating():
    clear_status_rows()
    row = config.get("status_row_3")
    lcd.move_to(0, row)
    lcd.putstr("Updating Firmware...")
    gc.collect()


def display_ota_error():
    clear_status_rows()
    row = config.get("status_row_3")
    lcd.move_to(0, row)
    lcd.putstr("Firmware Update \nFailed")
    gc.collect()


def display_connecting():
    row = config.get("status_row_3")
    clear_lcd_line(row)
    lcd.move_to(0, row)
    lcd.putstr("Connecting to PC...")
    gc.collect()


def display_connected():
    row = config.get("connection_row")
    clear_lcd_line(row)
    lcd.move_to(0, row)
    lcd.putstr(lan.ifconfig()[0])
    lcd.move_to(17, row)
    lcd.putstr("<")
    lcd.putchar(chr(0))
    lcd.putstr(">")
    gc.collect()


def display_disconnected():
    row = config.get("connection_row")
    clear_lcd_line(row)
    lcd.move_to(17, row)
    lcd.putstr("<X>")
    row = config.get("status_row_3")
    lcd.move_to(0, row)
    lcd.putstr("Check PC Connection")
    gc.collect()


# End display functions


def backup_app():
    import shutil

    # save current version
    source = "/app.py"
    destination = "/app_previous.py"
    shutil.copyfile(source, destination)


def connect_lan():
    global lan
    display_connecting()
    lan = network.LAN(
        mdc=Pin(23),
        mdio=Pin(18),
        phy_type=network.PHY_LAN8720,
        phy_addr=0,
    )
    # optionally, use static config
    # lan.ifconfig(config.get('ip'), config.get('gateway'), config.get('netmask'))
    lan.active(1)
    sleep_ms(10000)

def check_rgb():
    default_r_value = 40
    default_g_value = 50
    default_b_value = 40
    base_url = config.get("base_url")
    url = "{}/rgb".format(base_url)
    try:
        res = requests.get(url)
        logprint("debug", res.text)
        data = res.json()
        r_value = data.get("red")
        g_value = data.get("green")
        b_value = data.get("blue")
        return r_value, g_value, b_value
    except Exception as e:
        logprint("error", "Error during API call")
        return default_r_value, default_g_value, default_b_value

def check_ota():
    base_url = config.get("base_url")
    url = "{}/ota?version={}".format(base_url, VERSION)
    res = requests.get(url)
    logprint("debug", res.text)
    data = res.json()
    if data.get("version"):
        fpath = data["fpath"]
        download_ota(fpath)


def download_ota(fpath="/files/firmware.py"):
    logprint("warning", "Starting OTA")
    backup_app()
    gc.collect()
    if not check_ping():
        logprint("info", "Connecting to LAN")
        connect_lan()
    gc.collect()
    display_ota_updating()
    base_url = config.get("base_url")
    url = "{}{}".format(base_url, fpath)
    res = requests.get(url)
    if res.status_code != 200 or not res.content:
        display_ota_error()
        return False
    with open("app.py", "wb") as f:
        f.write(res.content)
    sleep_ms(1000)
    clear_status_rows()
    logprint("warning", "OTA Completed")
    reset()
    return True


def check_ping():
    result = ping(config.get("gateway"), timeout=100, quiet=True, count=2)
    return result[1] > 0


def trigger():
    global is_triggering
    global previous_trigger_ticks
    limit_switch_state = limitswitch.value()
    if (
        not is_triggering
        and not limit_switch_state
        and is_connected
        and ticks_ms() - previous_trigger_ticks >= trigger_interval
    ):
        logprint("debug", "Triggered: " + str(ticks_ms()))
        is_triggering = True
        sync_tray_led()
        set_status_led(status_led_2, True)
        display_triggered()
        url = config.get("base_url")
        try:
            res = requests.post(url)
            logprint("debug", "Trigger result: " + res.text)
            sleep_ms(1000)
            assert res.status_code == 200, "Capture Error"
            display_captured()
        except Exception as e:
            logprint("error", str(e))
            display_error()
        sleep_ms(2000)
        display_waiting()
        set_status_led(status_led_2, False)
        is_triggering = False
        previous_trigger_ticks = ticks_ms()


def healthcheck():
    if is_triggering:
        return False
    global is_connected
    try:
        url = config.get("base_url")
        res = requests.get(url)
        assert res.status_code == 200, "Disconnected"
        if is_connected == False:
            display_connected()
            if not is_triggering:
                display_waiting()
        is_connected = True
    except Exception as e:
        display_disconnected()
        is_connected = False
        logprint("error", "Healthcheck:" + str(e))
    gc.collect()




def start():
    print("I:", "Starting app: " + __name__)
    try:
        r_value = 40
        g_value = 50
        b_value = 40
        count = 0
        lcd.clear()
        lcd.backlight_on()
        set_status_led(status_led_2, False)
        display_brandname()
        connect_lan()
        gc.collect()
        healthcheck()
        if is_connected:
            r_value, g_value, b_value = check_rgb()
            check_ota()
            display_waiting()
        switch_on_neopixels(r_value, g_value, b_value)
        logprint("debug", "Heap Memory: " + str(gc.mem_free()))
        timer1 = Timer(1)
        timer1.init(
            period=config.get("healthcheck_interval") * 1000,
            mode=Timer.PERIODIC,
            callback=lambda t: healthcheck(),
        )
        logprint("info", "Ready!")
        logprint("info", "App Version: " + VERSION)
        while True:
            count += 1
            sync_tray_led()
            if count > 50:
                gc.collect()
                logprint("debug", "Heap Memory: " + str(gc.mem_free()))
                count = 0
            sleep_ms(int(config.get("main_loop_delay_ms")))
    except Exception as e:
        logprint("error", str(e))
        set_status_led(status_led_1, False)
        set_status_led(status_led_2, False)
        switch_off_neopixels()
        display_status(str(e))
        sleep_ms(3000)
        raise


if __name__ == "__main__":
    start()
