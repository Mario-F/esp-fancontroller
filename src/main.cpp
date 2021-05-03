/*
 * Fancontroller MAIN File
 */
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>
#include "Credentials.h"
#include "ControllerTemp/ControllerTemp.h"

/* Set pin usage */
#define ONE_WIRE_BUS D2
#define FAN_SPEEDIN D3
#define FAN_CONTROL D8

/* Set some variables */
#define CONFIG_WRITE_FLAG 18
const int eeAddressConfigReset = 0;
const int eeAddressConfig = 1;

/* Define Objects */
ESP8266WebServer server(80);
ControllerTemp temps(ONE_WIRE_BUS);

/* Define structs*/
struct Sensor {
  String DeviceAddress;
  String DeviceName;
};
struct Config {
  float TargetTemp;
  int DefaultFan;
  String SourceSensor;
  Sensor Sensors[MAX_SENSORS];
};
Config globalConfig;

/* Define and Setup W-Lan */
const char* ssid = WIFI_SSID;  // Enter SSID here
const char* password = WIFI_PW;  //Enter Password here


/*
Setup and define FAN FUNCTIONS
*/
int fanspeed = 1023;
int fanSpeedCounter = 0;
unsigned long fanSpeedTimer;
int actFanSpeed = 0;

void calc_fanSpeed() {
  int timePassed = (millis() - fanSpeedTimer);
  if(timePassed > 4000) {
    // Set Fanspeed
    actFanSpeed = (fanSpeedCounter * 15 / 2);
    Serial.print("FanSpeed - Time passed ");
    Serial.print(timePassed);
    Serial.print("ms, with Fanspeed: ");
    Serial.print(actFanSpeed);
    Serial.println(" rpm");

    // Reset loop
    fanSpeedTimer = millis();
    fanSpeedCounter = 0;
  }
}

ICACHE_RAM_ATTR void counter() {
  fanSpeedCounter++;
}

void set_fan(int speed) {
  Serial.print("(set_fan) Set Fanspeed: "); Serial.println(speed);
  fanspeed = speed;
  analogWriteFreq(22000);
  analogWrite(FAN_CONTROL, fanspeed);
}


/*
Webserver handles
*/
void handle_Temps() {
  String retRes = "[";
  int sensorCount = temps.getDeviceCount();
  for (int i = 0; i < sensorCount; i++) {
    ControllerSensor sensor = temps.getSensor(i);
    retRes += "{\"type\":\"ds18b20\",\"addr\":\"";
    retRes += sensor.getName();
    retRes += "\",\"temp\":";
    retRes += sensor.getTemp();
    retRes += ",\"errors\":";
    retRes += sensor.getErrorCount();
    retRes += "}";
    if (i+1 < sensorCount) retRes += ",";
  }
  retRes += "]";
  server.send(200, "application/json", retRes); 
}

void handle_Speeds() {
  Serial.print("Fanspeed is: "); Serial.print(actFanSpeed); Serial.println(" rpm.");
  String retRes = "[";
  retRes += "{\"type\":\"fan\",\"rpm\":";
  retRes += actFanSpeed;
  retRes += "}]";
  server.send(200, "application/json", retRes); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

/*
Config handling
*/
void loadConfiguration(Config &config) {
  int resetConfig = EEPROM.read(eeAddressConfigReset);

  if(resetConfig != CONFIG_WRITE_FLAG) {
    Serial.println("ConfigReset is set, use default values");
    config.TargetTemp = 25;
    config.DefaultFan = 20;
    config.SourceSensor = "first"; // use first sensor if not defined
  } else {
    Serial.println("Read config from EEPROM");
    EEPROM.get(eeAddressConfig, config);
  }

  Serial.println("Config read...:");
  Serial.print("TargetTemp: "); Serial.println(config.TargetTemp);
  Serial.print("SourceSensor: "); Serial.println(config.SourceSensor);
  Serial.print("DefaultFan: "); Serial.println(config.DefaultFan);
  for(int i = 0; i < MAX_SENSORS; i++) {
    Serial.print("Sensor -> ");
    Serial.print(config.Sensors[i].DeviceName);
    Serial.print(":");
    Serial.println(config.Sensors[i].DeviceAddress);
  }
  Serial.println("Config end.");
}

void saveConfiguration(Config &config) {
  EEPROM.put(eeAddressConfig, config);
  EEPROM.write(eeAddressConfigReset, CONFIG_WRITE_FLAG);
  Serial.println("Config saved to EEPROM!");
}

void resetConfiguration() {
  EEPROM.write(eeAddressConfigReset, 0);
  EEPROM.commit();
  Serial.println("Set config reset flag, restarting now!");
  ESP.restart();
}


/*
Main components
*/
void setup() {
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Start setup!");
  delay(1000);

  /* Initialize EEPROM */
  EEPROM.begin(512);

  /* Initalize config */
  loadConfiguration(globalConfig);

  /* Setup FAN-Pin and start 100% as failsave */
  pinMode(FAN_CONTROL, OUTPUT);
  set_fan(fanspeed);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  // Setup Fan Read speeds
  pinMode(FAN_SPEEDIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FAN_SPEEDIN), counter, RISING);
  Serial.println("Fanspeed is setup");

  // Connect and handle wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  // Init temp sensors and set in conf
  #ifdef DEBUGMODE
  temps.setVerbose(true);
  #endif
  temps.initSensors();
  temps.setSampleRate(14200);
  temps.setSampleRateAdvance(1870);
  while(temps.getDeviceCount() == 0) {
    temps.initSensors();
    delay(1000);
  }

  // Setup Webserver handling
  server.on("/temps", handle_Temps);
  server.on("/fans", handle_Speeds);
  server.on(UriBraces("/fan/{}"), []() {
    String newspeedS = server.pathArg(0);
    int newspeed = (int)newspeedS.toInt();
    set_fan(newspeed);
    Serial.println("FAN SPEED: '" + newspeedS + "'");
    server.send(200, "text/plain", "FAN SPEED UPDATED: '" + newspeedS + "'");
  });
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  calc_fanSpeed();
  temps.loop();
}
