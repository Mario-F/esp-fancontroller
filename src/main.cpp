/*
 * Fancontroller MAIN File
 */
#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>
#include "Credentials.h"
#include "ConfigManager/ConfigManager.h"
#include "ControllerTemp/ControllerTemp.h"
#include "ControllerFan/ControllerFan.h"

/* Set pin usage */
#define ONE_WIRE_BUS D2
#define FAN_RPM_INPUT D3
#define FAN_PWM_OUTPUT D8

/* Define Objects */
ESP8266WebServer server(80);
ControllerTemp temps(ONE_WIRE_BUS);
ControllerFan fana(FAN_RPM_INPUT, FAN_PWM_OUTPUT);

/* Define and Setup W-Lan */
const char* ssid = WIFI_SSID;  // Enter SSID here
const char* password = WIFI_PW;  //Enter Password here


/*
Webserver handles
*/
void handle_verbose() {
  boolean newVerbose = false;
  String verboseInput = server.pathArg(0);
  Serial.print("Change verbose mode with input: "); Serial.println(verboseInput);
  if(verboseInput == "yes") newVerbose = true;
  fana.setVerbose(newVerbose);
  temps.setVerbose(newVerbose);
  ConfigManager::setVerbose(newVerbose);
  server.send(200, "text/plain", "New verbose status is: '" + String(newVerbose) + "'");
}

void handle_Status() {
  DynamicJsonDocument jsonResponse(512);

  DynamicJsonDocument instanceData(128);
  instanceData["name"] = ConfigManager::getConfig().instanceName;
  instanceData["targetTemp"] = ConfigManager::getConfig().targetTemp;
  instanceData["targetSensor"] = ConfigManager::getConfig().targetSensor;
  jsonResponse["instance"] = instanceData;

  DynamicJsonDocument wifiData(128);
  wifiData["ip"] = WiFi.localIP().toString();
  wifiData["gateway"] = WiFi.gatewayIP().toString();
  wifiData["netmask"] = WiFi.subnetMask().toString();
  wifiData["signal"] = WiFi.RSSI();
  jsonResponse["wifi"] = wifiData;

  DynamicJsonDocument chipData(128);
  chipData["chipId"] = ESP.getChipId();
  chipData["flashChipId"] = ESP.getFlashChipId();
  chipData["flashChipSize"] = ESP.getFlashChipSize();
  chipData["flashChipRealSize"] = ESP.getFlashChipRealSize();
  chipData["freeHeap"] = ESP.getFreeHeap();
  jsonResponse["chip"] = chipData;

  String buf;
  serializeJson(jsonResponse, buf);
  server.send(200, "application/json", buf);
}

void handle_Sensors() {
  DynamicJsonDocument jsonResponse(1024);
  for (int i = 0; i < MAX_SENSORS; i++) {
    SensorConfig sens = ConfigManager::getConfig().sensors[i];
    DynamicJsonDocument jsonSensor(256);
    jsonSensor["slot"] = i;
    jsonSensor["sensorUID"] = sens.sensorUID;
    jsonSensor["sensorName"] = sens.sensorName;
    jsonResponse.add(jsonSensor);
  }
  String buf;
  serializeJson(jsonResponse, buf);
  server.send(200, "application/json", buf);
}

void handle_SensorsUpdate() {
  String sensorUID = server.pathArg(0);
  String sensorName = server.pathArg(1);
  String errorMessage = "";
  // Check if sensor exists
  ControllerSensor retSensor;
  if (!temps.getSensorByUID(sensorUID, &retSensor)) {
    server.send(404, "text/plain", "Sensor for update not found");
    return;
  }
  if (!ConfigManager::setSensorName(sensorUID, sensorName, &errorMessage)) {
    Serial.print("Error: "); Serial.println(errorMessage);
    server.send(507, "text/plain", errorMessage);
    return;
  }
  server.send(200, "text/plain");
}

void handle_Temps() {
  DynamicJsonDocument jsonResponse(1024);
  int sensorCount = temps.getDeviceCount();
  for (int i = 0; i < sensorCount; i++) {
    ControllerSensor sensor = temps.getSensor(i);
    DynamicJsonDocument jsonSensor(256);
    jsonSensor["type"] = "ds18b20";
    jsonSensor["uid"] = sensor.getUID();
    if (sensor.getName() != "") {
      jsonSensor["name"] = sensor.getName();
    }
    jsonSensor["temp"] = sensor.getTemp();
    jsonSensor["errors"] = sensor.getErrorCount();
    jsonResponse.add(jsonSensor);
  }
  String buf;
  serializeJson(jsonResponse, buf);
  server.send(200, "application/json", buf);
}

void handle_Speeds() {
  DynamicJsonDocument jsonResponse(512);
  DynamicJsonDocument singleFan(256);
  singleFan["type"] = "fan";
  singleFan["rpm"] = fana.getRPM();
  singleFan["speed"] = fana.getSpeed();
  singleFan["defaultSpeed"] = ConfigManager::getConfig().defaultSpeed;
  jsonResponse.add(singleFan);
  String buf;
  serializeJson(jsonResponse, buf);
  server.send(200, "application/json", buf);
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


/*
Main components
*/
void setup() {
  /* Try to counter some boot problems */
  __asm__("nop\n\t"); delay(1000); __asm__("nop\n\t");
  Serial.begin(115200); Serial.println(); Serial.println("Start setup!");

  /* Setup debugmode stuff */
  #ifdef DEBUGMODE
  fana.setVerbose(true);
  temps.setVerbose(true);
  ConfigManager::setVerbose(true);
  #endif

  /* Init the config from file */
  ConfigManager::initConfig("/config.json");
  fana.setSpeed(ConfigManager::getConfig().defaultSpeed);

  /* Connect to wifi */
  Serial.print("Connecting to WIFI "); Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" succesfully connected!");
  Serial.print("IP: "); Serial.println(WiFi.localIP());

  /* Setup temp sensors */
  temps.setSampleRate(5765);
  temps.setSampleRateAdvance(1372);
  temps.initSensors(true);

  /* Setup Webserver handling */
  server.on(UriBraces("/verbose/{}"), handle_verbose);
  server.on("/status", handle_Status);
  server.on("/temps", handle_Temps);
  server.on("/fans", handle_Speeds);
  server.on(UriBraces("/fan/{}"), []() {
    String newspeedS = server.pathArg(0);
    int newspeed = (int)newspeedS.toInt();
    fana.setSpeed(newspeed);
    // Set this also as new default speed
    ConfigManager::setDefaultSpeed(newspeed);
    Serial.println("FAN SPEED: '" + newspeedS + "'");
    server.send(200, "text/plain", "FAN SPEED UPDATED: '" + newspeedS + "'");
  });
  server.on(UriBraces("/name/{}"), []() {
    String newName = server.pathArg(0);
    ConfigManager::setInstanceName(newName);
    Serial.print("New instance Name: "); Serial.println(newName);
    server.send(200, "text/plain");
  });
  server.on("/sensors", handle_Sensors);
  server.on(UriBraces("/sensors/{}/{}"), handle_SensorsUpdate);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  fana.loop();
  temps.loop();
}
