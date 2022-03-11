import gc
import urequests as requests
import network
import neopixel
from machine import idle, SoftI2C, Pin, unique_id, reset, Timer
from utime import sleep_ms, ticks_ms
from esp32_i2c_lcd import I2cLcd
from uping import ping

VERSION = "0.0.1"

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
    "num_pixels": 60,
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
    "healthcheck_interval": 30
}

mac_address = "".join("{:02x}".format(d) for d in unique_id()).upper()



status_led_1 = Pin(int(config.get("status_led_1")), Pin.OUT)
status_led_2 = Pin(int(config.get("status_led_2")), Pin.OUT)
neopixel_pin = Pin(int(config.get("neopixel")), Pin.OUT)
limitswitch = Pin(int(config.get("button")), Pin.IN)
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
neopixel_array = neopixel.Neopixel(neopixel_pin, config.get('num_pixels'))

last_interrupt_time = 0
previous_trigger_ticks = 0
trigger_interval = 1000


def limitswitch_isr(pin: Pin):
    global last_interrupt_time
    interrupt_time = ticks_ms()
    if interrupt_time - last_interrupt_time > 1000:
        trigger()
    last_interrupt_time = interrupt_time

limitswitch.irq(limitswitch_isr, trigger=Pin.IRQ_FALLING)


def set_status_led(led:Pin, state: bool):
    # Inverted logic
    led.value(not state)


def sync_tray_led():
    tray_status = limitswitch.value()
    set_status_led(status_led_1, tray_status)


def switch_off_neopixels():
    neopixel_array.fill((0,0,0))


def switch_on_neopixels():
    switch_off_neopixels()
    positions = [0,1,2,14,15,16,28,29,30,42,43,44]
    for i in positions:
        neopixel_array[i] = (255, 255, 220)
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
    lcd.putstr("Connecting to PC")
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
    #print("D: Heap Memory: ", gc.mem_free())

# End display functions


def backup_app():
    import shutil

    # save current version
    source = "/app.py"
    destination = "/app_previous.py"
    shutil.copyfile(source, destination)

def connect_lan():
    lan = network.LAN(mdc=Pin(23), mdio=Pin(18), power=Pin(12), phy_type = network.PHY_LAN8720, phy_addr=0, clock_mode=network.ETH_CLOCK_GPIO17_OUT)
    # optionally, use static config
    # lan.ifconfig(config.get('ip'), config.get('gateway'), config.get('netmask'))
    lan.active(1)

def download_ota():
    print("W: Starting OTA")
    backup_app()
    gc.collect()
    if not check_ping():
        print("I: Connecting to LAN")
        connect_lan()
    gc.collect()
    display_ota_updating()
    base_url = config.get("base_url")
    url = f"{base_url}/app.py"
    res = requests.get(url)
    if res.status_code != 200 or not res.content:
        display_ota_error()
        return False
    with open("app.py", "wb") as f:
        f.write(res.content)
    clear_status_rows()
    print("W: OTA Completed")
    return True


def check_ping():
    result = ping(config.get("gateway"), timeout=100, quiet=True, count=2)
    return result[1] > 0


def set_clock():
    from ntptime import settime

    try:
        settime()
    except Exception as e:
        print("E:", str(e))

def trigger():
    global is_triggering
    global previous_trigger_ticks
    limit_switch_state = limitswitch.value()
    if not is_triggering and not limit_switch_state and is_connected and ticks_ms() - previous_trigger_ticks >= trigger_interval:
        is_triggering = True
        set_status_led(status_led_2, True)
        display_triggered()
        url = f"{config.get('base_url')}/trigger"
        res = requests.post(url)
        if res.status_code == 200:
            sleep_ms(1000)
            display_captured()
            sleep_ms(2000)
            display_waiting()
        else:
            display_error()
        set_status_led(status_led_2, False)
        is_triggering = False
        previous_trigger_ticks = ticks_ms()


def healthcheck():
    global is_connected
    if check_ping():
        display_connected()
        is_connected = True
    else:
        display_disconnected()
        is_connected = False 
        sleep_ms(2000)
        connect_lan()
    # Post a request over HTTP
    gc.collect()

timer1 = Timer(1)
timer1.init(period=config.get('healthcheck_interval')*1000, mode=Timer.PERIODIC, callback=lambda t:healthcheck())

def start():
    try:
        count = 0
        lcd.clear()
        lcd.backlight_on()
        display_brandname()
        switch_on_neopixels()
        connect_lan()
        set_clock()
        gc.collect()
        healthcheck()
        if is_connected:
            display_waiting()
        print("D: Heap Memory: ", gc.mem_free())
        print("I: Ready!")
        print("I: App Version: ", VERSION)
        while True:
            sync_tray_led()
            if count > 10:
                gc.collect()
                print("D: Heap Memory: ", gc.mem_free())
            count += 1
            sleep_ms(int(config.get("main_loop_delay_ms")))
    except Exception as e:
        set_status_led(status_led_1, False)
        set_status_led(status_led_2, False)
        switch_off_neopixels()
        display_status(str(e))
        sleep_ms(3000)
        raise


if __name__ == "__main__":
    start()
