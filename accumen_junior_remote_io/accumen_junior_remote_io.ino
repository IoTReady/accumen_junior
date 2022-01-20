/*
  Provides an API for:
  - Reading inputs
  - Controlling Illumination LEDs
  - Controlling outputs
  - Controlling LCD Display
*/

#include <ETH.h>
#include <Adafruit_NeoPixel.h>
#include <WebServer.h>
#include <Arduino_JSON.h>

#define NEOPIXEL_PIN 2
#define NUMPIXELS 18
#define DELAYVAL 50


static bool eth_connected = false;
WebServer server(80);

IPAddress ip(192, 168, 10, 2);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnetmask(255, 255, 255, 0);
IPAddress dns(192, 168, 10, 1);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int inputs[4] = {4,16,17,36};
// int inputs[5] = {4,6,16,17,36};
int outputs[5] = {8,9,10,11,13};

static unsigned long last_interrupt_time = 0;

unsigned long currentMillis = 0;             // stores the value of millis() in each iteration of loop()

void initSerial()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial)
  {
  }
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

  int pixelArray[NUMPIXELS] = {3,4,5,14,15,16,25,26,27,36,37,38};

  for (int i = 0; i < NUMPIXELS; i++)
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
    for (int i=0; i<sizeof inputs/sizeof inputs[0]; i++) {
      int state = digitalRead(inputs[i]);
      Serial.print("State of input ");
      Serial.print(inputs[i]);
      Serial.print(": ");
      Serial.println(state);
      delay(10);
    }
  }
  last_interrupt_time = interrupt_time;
}

void initialiseIOs() {
  for (int i=0; i<sizeof inputs/sizeof inputs[0]; i++) {
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

void handleOutputChange(int state) {
  for (uint8_t i = 0; i < server.args(); i++) {
    String argName = server.argName(i);
    argName.toLowerCase();
    String argValue = server.arg(i);
    int pinNumber;
    if (argName == "pin"){
      pinNumber = argValue.toInt();
      for (int i=0; i<=sizeof inputs/sizeof outputs[0]; i++) {
          if (outputs[i] == pinNumber) {
            digitalWrite(pinNumber, state);
            return success();
          }
      }
    }
    return error();
  } 
}

void handleOutputOn() {
  handleOutputChange(HIGH);
}

void handleOutputOff() {
  handleOutputChange(LOW);
}


void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
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
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void initServer() {
  // healthcheck path
  server.on("/", success);
  server.on("/illumination/off", handlePixelsOff);
  server.on("/illumination/on", handlePixelsOn);
  server.on("/output/on", handleOutputOn);
  server.on("/output/off", handleOutputOff);
 
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
  
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  ETH.config(ip, gateway, subnetmask, dns, dns);
}


void loop()
{
  currentMillis = millis();
  server.handleClient();
  delay(2);
}
