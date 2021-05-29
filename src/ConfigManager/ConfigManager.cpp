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
      Serial.println(LOG_PREFIX"(init) An Error has occurred while mounting LittleFS");
    }
    return false;
  }
  File file = LittleFS.open(_configfile, "r");
  if(!file){
    if (_verbose) {
      Serial.println(LOG_PREFIX"(init) Failed to open file for reading");
    }
    return false;
  }

  StaticJsonDocument<MAX_CONFIG_SIZE> doc;
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println(LOG_PREFIX"(init) Read config file failed");
    return false;
  }

  _config.instanceName = doc["instanceName"] | "unnamed";
  _config.targetTemp = doc["targetTemp"] | 20;
  _config.targetSensor = doc["targetSensor"] | "";

  file.close();
  Serial.print(LOG_PREFIX"(init) Config init finished for instance: ");
  Serial.println(_config.instanceName);
  return true;
};

MainConfig ConfigManager::getConfig() {
  return _config;
}
