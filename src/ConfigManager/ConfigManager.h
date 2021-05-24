// Manage persistent configuration on flash drive

#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ControllerTemp/ControllerTemp.h>

// Main configuration for the esp controller
struct MainConfig
{
  String instanceName;
  int tempTarget;
  SensorConfig sensors[MAX_SENSORS];
};

// Sensor config overlay for 
struct SensorConfig
{
  String sensorUID;
  String sensorName;
};

class ConfigManager {

  public:
    static ConfigManager* getValue(String key);

  private:
    ConfigManager();
    static MainConfig _config;
};
