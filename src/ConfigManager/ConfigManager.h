// Manage persistent configuration on flash drive

#include <LittleFS.h>
#include <ArduinoJson.h>

#define LOG_PREFIX_CM "ConfigManager - "
#define MAX_CONFIG_SIZE 1024
#define MAX_SENSORS 8

// Sensor config overlay for 
struct SensorConfig
{
  String sensorUID;
  String sensorName;
};

// Main configuration for the esp controller
struct MainConfig
{
  String instanceName;
  int defaultSpeed;
  int targetTemp;
  String targetSensor;
  SensorConfig sensors[MAX_SENSORS];
};

class ConfigManager {

  public:
    static void setVerbose(boolean _flag);
    static boolean initConfig(String _file);
    static MainConfig getConfig();
    static boolean saveConfig();

    static void setInstanceName(String name);
    static void setDefaultSpeed(int speed);

  private:
    ConfigManager();
    static boolean _verbose;
    static String _configfile;
    static MainConfig _config;
};
