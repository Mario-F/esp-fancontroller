#include <ArduinoJson.h>
#include "ConfigManager.h"

boolean ConfigManager::_verbose = false;
String ConfigManager::_configfile = "/config.json";
MainConfig ConfigManager::_config;

/*
 * Manage persistent configs
 */
ConfigManager::ConfigManager() {};

void ConfigManager::setVerbose(bool _flag) {
  _verbose = _flag;
};

boolean ConfigManager::initConfig(String _file) {
  _configfile = _file;
  if(!LittleFS.begin()){
    if (_verbose) {
      Serial.println(LOG_PREFIX_CM"(init) An Error has occurred while mounting LittleFS");
    }
    return false;
  }
  File file = LittleFS.open(_configfile, "r");
  if(!file){
    if (_verbose) {
      Serial.println(LOG_PREFIX_CM"(init) Failed to open file for reading");
    }
    return false;
  }

  StaticJsonDocument<MAX_CONFIG_SIZE> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(LOG_PREFIX_CM"(init) Read config file failed");
    return false;
  }

  _config.instanceName = doc["instanceName"] | "unnamed";
  _config.defaultSpeed = doc["defaultSpeed"] | 100;
  _config.targetTemp = doc["targetTemp"] | 20;
  _config.targetSensor = doc["targetSensor"] | "";
  if(doc["sensors"]) {
    for(int i = 0; i < MAX_SENSORS; i++) {
      _config.sensors[i].sensorUID = doc["sensors"][i]["sensorUID"] | "";
      _config.sensors[i].sensorName = doc["sensors"][i]["sensorName"] | "";
    }
  }

  file.close();
  Serial.print(LOG_PREFIX_CM"(init) Config init finished for instance: ");
  Serial.println(_config.instanceName);
  return true;
};

MainConfig ConfigManager::getConfig() {
  return _config;
};

boolean ConfigManager::saveConfig() {
  LittleFS.remove(_configfile);
  File file = LittleFS.open(_configfile, "w");
  if(!file){
    if (_verbose) {
      Serial.println(LOG_PREFIX_CM"(save) Failed to open file for writing");
    }
    return false;
  }

  StaticJsonDocument<MAX_CONFIG_SIZE> doc;

  doc["instanceName"] = _config.instanceName;
  doc["defaultSpeed"] = _config.defaultSpeed;
  doc["targetTemp"] = _config.targetTemp;
  doc["targetSensor"] = _config.targetSensor;

  // Handle the sensors array to json
  DynamicJsonDocument sensorsArray(1024);
  for (int i = 0; i < MAX_SENSORS; i++) {
    DynamicJsonDocument jsonSensor(256);
    jsonSensor["sensorUID"] = _config.sensors[i].sensorUID;
    jsonSensor["sensorName"] = _config.sensors[i].sensorName;;
    sensorsArray.add(jsonSensor);
  }
  doc["sensors"] = sensorsArray;

  if (serializeJson(doc, file) == 0) {
    if (_verbose) {
      Serial.println(LOG_PREFIX_CM"(save) Failed write to config file");
    }
    return false;
  }

  file.close();
  return true;
};

void ConfigManager::setInstanceName(String name) {
  _config.instanceName = name;
  saveConfig();
};

void ConfigManager::setTargetTemp(int targetTemp) {
  _config.targetTemp = targetTemp;
  saveConfig();
};

void ConfigManager::setTargetSensor(String targetSensor) {
  _config.targetSensor = targetSensor;
  saveConfig();
};

void ConfigManager::setDefaultSpeed(int speed) {
  _config.defaultSpeed = speed;
  saveConfig();
};

boolean ConfigManager::setSensorName(String sensorUID, String sensorName, String *error) {
  // Check for update existing sensor name
  boolean foundExistingSensor = false;
  int lastEmptySlot = -1;
  for(int i = 0; i < MAX_SENSORS; i++) {
    if (_config.sensors[i].sensorUID == sensorUID) {
      if (_verbose) {
        Serial.print(LOG_PREFIX_CM"(setSensorName) Sensor UID "); Serial.print(sensorUID); Serial.println(" found in config, updating.");
      }
      foundExistingSensor = true;
      _config.sensors[i].sensorName = sensorName;
      break;
    }
    // Remember a empty slot for assign of new sensor mapping
    if (_config.sensors[i].sensorUID == "" && _config.sensors[i].sensorName == "") {
      lastEmptySlot = i;
    }
  }
  
  // Insert new sensor mapping to config
  if (!foundExistingSensor) {
    if (lastEmptySlot == -1) {
      *error = "No empty slot for Sensor/Name mapping found please delete one first";
      return false;
    }
    _config.sensors[lastEmptySlot].sensorUID = sensorUID;
    _config.sensors[lastEmptySlot].sensorName = sensorName;
  }
  return saveConfig();
};

SensorConfig ConfigManager::getSensorByUID(String sensorUID) {
  SensorConfig retSensor;
  for(int i = 0; i < MAX_SENSORS; i++) {
    if (_config.sensors[i].sensorUID == sensorUID) {
      retSensor = _config.sensors[i];
      break;
    }
  }
  return retSensor;
};
