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
#include "Metrics/Metrics.h"

/* Set pin usage */
#define ONE_WIRE_BUS D2
#define FAN_RPM_INPUT D3
#define FAN_PWM_OUTPUT D8

/* Define Objects */
ESP8266WebServer server(80);
ControllerTemp temps(ONE_WIRE_BUS);
ControllerFan fana(FAN_RPM_INPUT, FAN_PWM_OUTPUT);
boolean loopFanTempVerbose = false;

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
  loopFanTempVerbose = true;
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
  wifiData["ssid"] = WiFi.SSID();
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
  singleFan["minimumSpeed"] = ConfigManager::getConfig().minimumSpeed;
  jsonResponse.add(singleFan);
  String buf;
  serializeJson(jsonResponse, buf);
  server.send(200, "application/json", buf);
}

void handle_Metrics() {
  MainConfig mConfig = ConfigManager::getConfig();

  MetricsManager::resetGlobalLabels();
  MetricsManager::addGlobalLabel("instance", ConfigManager::getConfig().instanceName);
  MetricsManager::resetMetrics();

  Metric *fInfo = MetricsManager::createMetric("fancontroller_info", "gauge", "A metric with constant 1 to create labels with information about this instance");
  fInfo->value = 1;

  Metric *fEspInfo = MetricsManager::createMetric("fancontroller_esp_info", "gauge", "A metric with constant 1 with infos about the ESP");
  fEspInfo->value = 1;
  MetricsManager::addMetricLabel(fEspInfo, "chip_id", (String)ESP.getChipId());
  MetricsManager::addMetricLabel(fEspInfo, "flash_chip_id", (String)ESP.getFlashChipId());
  
  Metric *fEspFreeHeap = MetricsManager::createMetric("fancontroller_esp_free_heap", "gauge", "Show the actual free heap of the ESP");
  fEspFreeHeap->value = ESP.getFreeHeap();

  Metric *fWlan = MetricsManager::createMetric("fancontroller_wlan_info", "gauge", "A metric with constant 1 to create labels with wlan information");
  fWlan->value = 1;
  MetricsManager::addMetricLabel(fWlan, "ssid", WiFi.SSID());
  MetricsManager::addMetricLabel(fWlan, "ip", WiFi.localIP().toString());

  Metric *fWlanRssi = MetricsManager::createMetric("fancontroller_wlan_signal", "gauge", "The wlan signal strength for the connected network");
  fWlanRssi->value = (float)WiFi.RSSI();

  Metric *fRPM = MetricsManager::createMetric("fancontroller_fan_rpm", "gauge", "The fans actual RPM value read by yellow signal");
  fRPM->value = fana.getRPM();

  Metric *fSpeed = MetricsManager::createMetric("fancontroller_fan_speed", "gauge", "The desired speed in percent");
  fSpeed->value = fana.getSpeed();

  int sensorCount = temps.getDeviceCount();
  for (int i = 0; i < sensorCount; i++) {
    ControllerSensor sensor = temps.getSensor(i);
    
    Metric *fTemp = MetricsManager::createMetric("fancontroller_sensor_temp", "gauge", "The actual temp of a sensor");
    MetricsManager::addMetricLabel(fTemp, "uid", sensor.getUID());
    MetricsManager::addMetricLabel(fTemp, "name", sensor.getName());
    fTemp->value = sensor.getTemp();

    // if sensor is target create metric for target temp
    if (mConfig.targetSensor == sensor.getUID()) {
      Metric *fTargetTemp = MetricsManager::createMetric("fancontroller_sensor_target_temp", "gauge", "The target temp fan control is set to");
      MetricsManager::addMetricLabel(fTargetTemp, "uid", sensor.getUID());
      MetricsManager::addMetricLabel(fTargetTemp, "name", sensor.getName());
      fTargetTemp->value = mConfig.targetTemp;
    }

    Metric *fErrorsValue = MetricsManager::createMetric("fancontroller_sensor_errors", "counter", "Errors in a row since last succesfull result");
    MetricsManager::addMetricLabel(fErrorsValue, "uid", sensor.getUID());
    MetricsManager::addMetricLabel(fErrorsValue, "name", sensor.getName());
    fErrorsValue->value = sensor.getErrorCount();
  }

  String response = MetricsManager::getMetrics();
  server.send(200, "application/json", response);
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


/*
Handle the fan/temp regulation
*/
int loopFanTempLastExecute = millis();
int loopFanTempLoopTimer = 10000;
int loopFanTempAllowErrors = 20;
int loopFanTempUpStepsMax = 5;
int loopFanTempDownSteps = 1;
int loopFanTempUpStepsSkip = 0;
float loopFanTempLastTemp = 0;
void loopFanTemp() {
  // Check for execution time reached
  int timePassed = (millis() - loopFanTempLastExecute);
  if (timePassed > loopFanTempLoopTimer) {
    MainConfig mConfig = ConfigManager::getConfig();
    ControllerSensor retSensor;
    if (!temps.getSensorByUID(mConfig.targetSensor, &retSensor)) {
      if (loopFanTempVerbose) {
        Serial.println("(LoopFanTemp) No target sensor is set, aborting.");
      }
      loopFanTempLastExecute = millis();
      return;
    }

    // Set default to step down and validate different values to step up
    boolean stepUpNeeded = false;
    if (retSensor.getErrorCount() > loopFanTempAllowErrors) {
      stepUpNeeded = true;
    }
    float readTemp = retSensor.getTemp();
    if (readTemp > mConfig.targetTemp) {
      stepUpNeeded = true;
    }

    // Set new fanspeed
    int actFanSpeed = fana.getSpeed();
    if (stepUpNeeded) {
      if (loopFanTempUpStepsSkip <= 5 && readTemp < loopFanTempLastTemp) {
        loopFanTempUpStepsSkip++;
        if (loopFanTempVerbose) {
          Serial.println("(LoopFanTemp) Temp is decreasing, dont step up.");
        }
      } else {
        loopFanTempUpStepsSkip = 0;
        int stepUpCalculated = constrain((retSensor.getTemp() - mConfig.targetTemp) * 2, 1, loopFanTempUpStepsMax);
        fana.setSpeed(actFanSpeed + stepUpCalculated);
        if (loopFanTempVerbose) {
          Serial.print("(LoopFanTemp) Execute StepUP for fana: "); Serial.println(stepUpCalculated);
        }
      }
    } else {
      Serial.println("(LoopFanTemp) Execute StepDOWN for fana.");
      fana.setSpeed(actFanSpeed - loopFanTempDownSteps);
    }
    loopFanTempLastTemp = readTemp;
    loopFanTempLastExecute = millis();
  }
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
  loopFanTempVerbose = true;
  #endif

  /* Set fan to max speed to prevent overheat if init hangs */
  fana.setSpeed(100);

  /* Init the config from file */
  ConfigManager::initConfig("/config.json");

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

  /* Set fan to default speed */
  fana.setSpeed(ConfigManager::getConfig().defaultSpeed);

  /* Setup Webserver handling */
  server.on(UriBraces("/verbose/{}"), handle_verbose);
  server.on("/status", handle_Status);
  server.on("/temps", handle_Temps);
  server.on("/fans", handle_Speeds);
  server.on(UriBraces("/speed/default/{}"), []() {
    String newspeedS = server.pathArg(0);
    int newspeed = (int)newspeedS.toInt();
    ConfigManager::setDefaultSpeed(newspeed);
    server.send(200, "text/plain", "DEFAULT FAN SPEED UPDATED: '" + newspeedS + "'");
  });
  server.on(UriBraces("/speed/minimum/{}"), []() {
    String newspeedS = server.pathArg(0);
    int newspeed = (int)newspeedS.toInt();
    // Set this also as new default speed
    ConfigManager::setDefaultSpeed(newspeed);
    server.send(200, "text/plain", "MINIMUM FAN SPEED UPDATED: '" + newspeedS + "'");
  });
  server.on(UriBraces("/name/{}"), []() {
    String newName = server.pathArg(0);
    ConfigManager::setInstanceName(newName);
    Serial.print("New instance Name: "); Serial.println(newName);
    server.send(200, "text/plain");
  });
  server.on(UriBraces("/target/temp/{}"), []() {
    String targetTemp = server.pathArg(0);
    int targetTempInt = targetTemp.toInt();
    ConfigManager::setTargetTemp(targetTempInt);
    server.send(200, "text/plain");
  });
  server.on(UriBraces("/target/sensor/{}"), []() {
    String targetSensor = server.pathArg(0);
    // Check if sensor exists
    ControllerSensor retSensor;
    if (!temps.getSensorByUID(targetSensor, &retSensor)) {
      server.send(404, "text/plain", "Sensor for update not found");
      return;
    }
    ConfigManager::setTargetSensor(targetSensor);
    server.send(200, "text/plain");
  });
  server.on("/sensors", handle_Sensors);
  server.on(UriBraces("/sensors/{}/{}"), handle_SensorsUpdate);
  server.on("/metrics", handle_Metrics);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  fana.loop();
  temps.loop();
  loopFanTemp();
}
