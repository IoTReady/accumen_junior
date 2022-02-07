/*
 * accumen_remote_io_controller.ino
 *
 *  Created on: 02.02.2022
 *
 */

#include <ETH.h>
#include <WiFiClient.h>
#include <Redis.h>
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 5
#define NUMPIXELS 60
#define DELAYVAL 50

#define LED1 2
#define LED2 12

#define LIMITSWITCH 36

#define REDIS_ADDR "192.168.10.1"
#define REDIS_PORT 6379
#define REDIS_PASSWORD ""
#define REDIS_CHANNEL "remote_io"

WiFiClient redisConn;
Redis redis(redisConn);

static bool eth_connected = false;

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte checkMark[] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0, 0x0};

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

byte mac[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

IPAddress ip(192, 168, 10, 2);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnetmask(255, 255, 255, 0);
IPAddress dns(192, 168, 10, 1);


int limitSwitchState = 0;
bool shouldTrigger = false;
bool isTriggering = false;
int currentTriggerId = 0;

static unsigned long last_interrupt_time = 0;

unsigned long currentMillis = 0;             // stores the value of millis() in each iteration of loop()
unsigned long previousHealthcheckMillis = 0; // will store last time the Healthcheck was updated
const int healthcheckInterval = 5000;

unsigned long previousTriggerMillis = 0; // will store last time the camera was triggered was updated
const int triggerInterval = 500;
unsigned long previousStatusCheckMillis = 0; // will store last time the Healthcheck was updated
const int statusCheckInterval = 2000;


String ipToString(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}


void clearLCDLine(int line)
{               
  lcd.setCursor(0,line);
  for(int n = 0; n < 20; n++) // 20 indicates symbols in line. For 2x16 LCD write - 16
  {
    lcd.print(" ");
  }
}

void displayWaiting()
{
  clearLCDLine(3);
  lcd.setCursor(1, 3);
  lcd.print("Insert Petri Dish");
}

void displayConnecting()
{
  lcd.setCursor(17, 0);
  lcd.print("<X>");
  clearLCDLine(3);
  lcd.setCursor(1, 3);
  lcd.print("Connecting to PC...");
}

void displayConnected()
{
  lcd.setCursor(17, 0);
  lcd.print("<");
  lcd.setCursor(18, 0);
  lcd.write(0);
  lcd.setCursor(19, 0);
  lcd.print(">");
}

void displayDisconnected()
{
  lcd.setCursor(17, 0);
  lcd.print("<x>");
  clearLCDLine(3);
  lcd.setCursor(0, 3);
  lcd.print("Check PC Connection ");
}

void displayCameraDisconnected()
{
  lcd.setCursor(17, 0);
  lcd.print("<x>");
  clearLCDLine(3);
  lcd.setCursor(0, 3);
  lcd.print("Camera Disconnected");
}

void displayError()
{
  clearLCDLine(3);
  lcd.setCursor(0, 3);
  lcd.print("Capture Error");
}

void displayTriggered()
{
  clearLCDLine(3);
  lcd.setCursor(0, 3);
  lcd.print("Imaging in Progress");
}

void displayCaptured()
{
  clearLCDLine(3);
  lcd.setCursor(2, 3);
  lcd.print("Imaging Complete");
}

void displayIpAddress()
{
  IPAddress ip = ETH.localIP();
  lcd.setCursor(0, 0);
  lcd.print(ip);
}

int postTrigger()
{
  currentTriggerId = currentMillis;
  char triggerIdString[20];
  itoa(currentTriggerId, triggerIdString, 10);
  JSONVar payload;
  payload["capture"] = true;
  payload["triggerId"] = triggerIdString;
  String jsonString = JSON.stringify(payload);
  Serial.print("jsonString: ");
  Serial.println(jsonString);
  int length = 100;
  char buffer[length];
  jsonString.toCharArray(buffer, length);
  return redis.publish(REDIS_CHANNEL, buffer);
}

void maybeTrigger()
{
  int limitSwitchState = digitalRead(LIMITSWITCH); // will return Closed/LOW or Open/HIGH (due to inverted inputs) 
  if (shouldTrigger && !isTriggering && !limitSwitchState && eth_connected && currentMillis - previousTriggerMillis >= triggerInterval)
  {
    isTriggering = true;
    previousTriggerMillis = currentMillis;
    // capture indicator
    digitalWrite(LED2, HIGH);
    displayTriggered();
    int statusCode = postTrigger();
    // statusCode is number of subscribers
    if (statusCode == 0)
    {
      displayDisconnected();
    } else {
      displayConnected();
    }
  }
  shouldTrigger = false;
  isTriggering = false;
}

void limitSwitchISR()
{
  // called on every change (RISING & FALLING)
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    shouldTrigger = true;
  }
  last_interrupt_time = interrupt_time;
}

String generateLog(){
  JSONVar payload;
  payload["limitSwitchState"] = limitSwitchState;
  payload["shouldTrigger"] = shouldTrigger;
  payload["isTriggering"] = isTriggering;
  payload["redisHost"] = REDIS_ADDR;
  payload["redisPort"] = REDIS_PORT;
  payload["redisChannel"] = REDIS_CHANNEL;
  payload["ip"] = ipToString(ip);
  payload["currentMillis"] = currentMillis;
  payload["previousTriggerMillis"] = previousTriggerMillis;
  
  String jsonString = JSON.stringify(payload);
  return jsonString;
}

bool postLog()
{
  String jsonString = generateLog();
  int length = 500;
  char buffer[length];
  jsonString.toCharArray(buffer, length);
  int statusCode = redis.publish(REDIS_CHANNEL, buffer);
  if (statusCode == 0) {
    displayDisconnected();
  } else {
    displayConnected();
  }
}

void pixelsOff()
{
  pixels.fill(pixels.Color(0, 0, 0), 0, NUMPIXELS);
  pixels.show();
}

void initPixels()
{
  pixels.begin();
  pixelsOff();
}

void pixelsOn()
{
  pixels.clear();

  int pixelArray[] = {3,4,5,14,15,16,25,26,27,36,37,38};

  for (int i = 0; i < sizeof pixelArray/sizeof pixelArray[0]; i++)
  {
    pixels.setPixelColor(pixelArray[i], pixels.Color(255, 255, 220));
    pixels.show();
    delay(DELAYVAL);
  }
}

void syncTrayLED() {
  int trayStatus = digitalRead(LIMITSWITCH);
  // Because our inputs are inverted. Low on limitswitch = closed => LED1 = OFF. High on limitswitch = open => LED1 = ON
  digitalWrite(LED1, trayStatus);
}

void syncProcessStatus() {
  if (currentTriggerId != 0 && currentMillis - previousStatusCheckMillis >= statusCheckInterval)
  {
    char triggerIdString[20];
    itoa(currentTriggerId, triggerIdString, 10);
    int triggerStatus = redis.get(triggerIdString).toInt();
    // 0 = In Progress; 1 = Done; 2 = Error    
    if (triggerStatus == 0) {
      // do nothing
    } else if (triggerStatus == 1) {
      delay(1000); // Artificial delay to account for faster frame capture once the camera settings are optimised.
      currentTriggerId = 0;
      displayCaptured();
      delay(2000);
      displayWaiting();
      digitalWrite(LED2, LOW);
    } else {
      currentTriggerId = 0;
      displayError();
      digitalWrite(LED2, LOW);
    }
    previousStatusCheckMillis = millis();
  }
}

void initLEDs()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, LOW);
  syncTrayLED();
}

void initLCD()
{
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.createChar(0, checkMark);
  lcd.setCursor(0, 1);
  lcd.print("aCCumen Jr");
  displayConnecting();
}

void initLimitSwitch()
{
  // we use the rising edge of the limit switch to trigger an ISR
  // but we trigger on all changes as we need to reflect the state on LED1
  pinMode(LIMITSWITCH, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(LIMITSWITCH), limitSwitchISR, CHANGE);
}

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      {
        Serial.println("ETH Started");
        //set eth hostname here
        ETH.setHostname("accumen-remote-io");
      }
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      {
        Serial.println("ETH Connected");
      }
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      {
        Serial.print("ETH MAC: ");
        Serial.print(ETH.macAddress());
        Serial.print(", IPv4: ");
        Serial.print(ETH.localIP());
        if (ETH.fullDuplex()) {
          Serial.print(", FULL_DUPLEX");
        }
        Serial.print(", ");
        Serial.print(ETH.linkSpeed());
        Serial.println("Mbps");
        eth_connected = true;
        displayIpAddress();
        if (!redisConn.connect(REDIS_ADDR, REDIS_PORT))
        {
            Serial.println("Failed to connect to the Redis server!");
            return;
        }
  
        Redis redis(redisConn);
        auto connRet = redis.authenticate(REDIS_PASSWORD);
        if (connRet == RedisSuccess)
        {
            Serial.println("Connected to the Redis server!");
            displayConnected();
            displayWaiting();
        }
        else
        {
            Serial.printf("Failed to authenticate to the Redis server! Errno: %d\n", (int)connRet);
            displayDisconnected();
            return;
        }
      }
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      {
        Serial.println("ETH Disconnected");
        eth_connected = false;
        displayDisconnected();
      }
      break;
    case SYSTEM_EVENT_ETH_STOP:
      {
        Serial.println("ETH Stopped");
        eth_connected = false;
        displayDisconnected();
      }
      break;
    default:
      {
      }
      break;
  }
}

void initSerial()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
  }
}

void healthcheck()
{
  if (!isTriggering && currentMillis - previousHealthcheckMillis >= healthcheckInterval)
  {
    postLog();
    previousHealthcheckMillis = millis();
  }
}

void setup()
{
  initPixels();

  // Limit Switch has to be initialised before indicator LEDs as we need to reflect status on boot.

  initLimitSwitch();

  initLEDs();

  initSerial();

  initLCD();

  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  // Uncomment the following line if using static IP config
  ETH.config(ip, gateway, subnetmask, dns, dns);

  pixelsOn();
}

void loop()
{
  currentMillis = millis();
  syncTrayLED();
  healthcheck();
  maybeTrigger();
  syncProcessStatus();
}
