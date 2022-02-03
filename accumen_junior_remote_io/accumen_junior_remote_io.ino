/*
  Provides an API for:
  - Reading inputs
  - Controlling Illumination LEDs
  - Controlling outputs
  - Controlling LCD Display
*/

#include <ETH.h>
#include <WiFiClient.h>
#include <Redis.h>
#include <WebServer.h>
#include <Arduino_JSON.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

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
#define MAX_BACKOFF 300000 // 5 minutes

WiFiClient redisConn;
Redis redis(redisConn);

static bool eth_connected = false;
WebServer server(80);

IPAddress ip(192, 168, 10, 2);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnetmask(255, 255, 255, 0);
IPAddress dns(192, 168, 10, 1);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte checkMark[] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0, 0x0};

int inputs[4] = {4,17,32,36};
int outputs[4] = {2,12,14,33};

static unsigned long last_interrupt_time = 0;

unsigned long currentMillis = 0;             // stores the value of millis() in each iteration of loop()

int limitSwitchState = 0;
bool shouldTrigger = false;
bool isTriggering = false;
bool inputsChanged = false;

unsigned long previousTriggerMillis = 0; // will store last time the camera was triggered
const int triggerInterval = 500;


void initSerial()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
  }
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
  displayWaiting();
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

void displayLine(int lineNumber, int offset, String text) {
  clearLCDLine(lineNumber);
  lcd.setCursor(offset, lineNumber);
  lcd.print(text);
}

void displayCharacter(int lineNumber, int offset, int index) {
  lcd.setCursor(offset, lineNumber);
  lcd.write(index);
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

void initLEDs()
{
  int trayStatus = digitalRead(LIMITSWITCH);
  digitalWrite(LED1, !trayStatus);
  digitalWrite(LED2, LOW);
}

void success() {
  JSONVar payload;
  payload["ok"] = true;
  server.send(200, "application/json", JSON.stringify(payload));
}

void error() {
  JSONVar payload;
  payload["ok"] = false;
  server.send(404, "application/json", JSON.stringify(payload));
}


void handleNotFound() {
  error();
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

void handlePixelsOff() {
  pixelsOff();
  success();
}

void handlePixelsOn() {
  pixelsOn();
  success();
}

void inputISR() {
  // called on every change (RISING & FALLING)
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
    shouldTrigger = digitalRead(LIMITSWITCH); // will return LOW/false or HIGH/true
    // petri dish indicator - should be lit if the tray is OUT (i.e. LIMITSWITCH is LOW)
    digitalWrite(LED1, !shouldTrigger);
    inputsChanged = true;
  }
  last_interrupt_time = interrupt_time;
}

void initialiseIOs() {
  for (int i=0; i < sizeof inputs/sizeof inputs[0]; i++) {
    Serial.print("Enabling input: ");
    Serial.println(inputs[i]);
    pinMode(inputs[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(inputs[i]), inputISR, CHANGE);
    delay(100);
  }
  for (int i=0; i<sizeof outputs/sizeof outputs[0]; i++) {
    Serial.print("Enabling output: ");
    Serial.println(outputs[i]);
    pinMode(outputs[i], OUTPUT);
    delay(100);
  }
}

void handleIORead() {
  int pinNumber = 0;
  for (uint8_t i = 0; i < server.args(); i++) {
    String argName = server.argName(i);
    argName.toLowerCase();
    String argValue = server.arg(i);
    if (argName == "pin"){
      pinNumber = argValue.toInt();
    }
  }
  if (pinNumber != 0) {
    JSONVar payload;
    payload["pin"] = pinNumber;
    payload["state"] = digitalRead(pinNumber);
    return server.send(200, "application/json", JSON.stringify(payload));
  } else {
    return error();
  }
}

void handleOutputChange() {
  int pinNumber;
  int state;
  for (uint8_t i = 0; i < server.args(); i++) {
    String argName = server.argName(i);
    argName.toLowerCase();
    String argValue = server.arg(i);
    if (argName == "pin"){
      pinNumber = argValue.toInt();
    } else if (argName == "state"){
      state = argValue.toInt();
    }
  }
  for (int i=0; i <= sizeof outputs/sizeof outputs[0]; i++) {
    if (outputs[i] == pinNumber) {
      digitalWrite(pinNumber, state);
      JSONVar payload;
      payload["pin"] = pinNumber;
      payload["state"] = digitalRead(pinNumber);
      return server.send(200, "application/json", JSON.stringify(payload));
    }
  }
  return error();
}

void handleDisplayLine() {
  int lineNumber;
  int offset;
  String text;
  for (uint8_t i = 0; i < server.args(); i++) {
    String argName = server.argName(i);
    argName.toLowerCase();
    String argValue = server.arg(i);
    if (argName == "line"){
      lineNumber = argValue.toInt();
    } else if (argName == "offset"){
      offset = argValue.toInt();
    } else if (argName == "text"){
      text = argValue;
    }
  }
  displayLine(lineNumber, offset, text);
  return success();
}

void splitString(String s, String splitArr[], String delimiter, int maxLength) {
  int i = 0;
  int a = s.indexOf(delimiter);
  while (a != -1) {
    String sub = s.substring(0, a);
    splitArr[i] = sub;
    if (a+1 > s.length()-1) {
      break;
    }
    s = s.substring(a+1);
    a = s.indexOf(delimiter);
    i++;
    if (maxLength != -1 and i >= maxLength) {
        break;
    }
  }
  if (s.length() != 0) {
    splitArr[i] = s;
  }
}

void handleStoreCharacter() {
  int index;
  byte character[8];
  String splitArr[8];
  
  for (uint8_t i = 0; i < server.args(); i++) {
    String argName = server.argName(i);
    argName.toLowerCase();
    String argValue = server.arg(i);
    if (argName == "index"){
      index = argValue.toInt();
    } else if (argName == "character"){
      splitString(argValue, splitArr, "\\", 8);
      for(int i = 0; i < 8; i++){
        character[i] = splitArr[i].toInt();
        Serial.println(character[i]);
      }
    }
  }

  lcd.createChar(index, character);
  return success();
}

void handleDisplayCharacter() {
  int lineNumber;
  int offset;
  int index;
  for (uint8_t i = 0; i < server.args(); i++) {
    String argName = server.argName(i);
    argName.toLowerCase();
    String argValue = server.arg(i);
    if (argName == "line"){
      lineNumber = argValue.toInt();
    } else if (argName == "offset"){
      offset = argValue.toInt();
    } else if (argName == "index"){
      index = argValue.toInt();
    }
  }
  displayCharacter(lineNumber, offset, index);
  return success();
}


void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      {
        Serial.println("ETH Started");
        //set eth hostname here
        ETH.setHostname("esp32-ethernet");
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
        Serial.println("Starting HTTP server");
        initServer();
        displayConnected();
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
        }
        else
        {
            Serial.printf("Failed to authenticate to the Redis server! Errno: %d\n", (int)connRet);
            return;
        }
      }
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      {
        Serial.println("ETH Disconnected");
        eth_connected = false;
      }
      break;
    case SYSTEM_EVENT_ETH_STOP:
      {
        Serial.println("ETH Stopped");
        eth_connected = false;
      }
      break;
    default:
      {
      }
      break;
  }
}

void initServer() {
  // healthcheck path
  server.on("/", success);
  server.on("/illumination/off", handlePixelsOff);
  server.on("/illumination/on", handlePixelsOn);
  server.on("/input", handleIORead);
  server.on("/output", handleOutputChange);
  server.on("/display/line", handleDisplayLine);
  server.on("/display/store", handleStoreCharacter);
  server.on("/display/character", handleDisplayCharacter);
 
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void setup()
{
  initSerial();

  initPixels();
  delay(1000);
  pixelsOn();

  initialiseIOs();

  initLCD();
  initLEDs();
  
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  // Uncomment the following line if using static IP config
  ETH.config(ip, gateway, subnetmask, dns, dns);
}


void loop()
{
  currentMillis = millis();
  if (inputsChanged == true) {
    JSONVar payload;
    for (int i=0; i<sizeof inputs/sizeof inputs[0]; i++) {
      payload[String(inputs[i])] = digitalRead(inputs[i]);
      delay(5);
    }
    String jsonString = JSON.stringify(payload);
    Serial.print("Sending inputs: ");
    Serial.println(jsonString);
    int length = 100;
    char buffer[length];
    jsonString.toCharArray(buffer, length);
    Serial.println(redis.publish(REDIS_CHANNEL, buffer));
    inputsChanged = false;
  }
  delay(2);
}
