// Abstract DallasTempSensor functions

#include <DallasTemperature.h>

#define LOG_PREFIX "ControllerTemp - "
#define MAX_SENSORS 8
#define DEVICE_DISCONNECTED_C -127

/*
 * Hold state of a single sensor
 */
class ControllerSensor {

  public:
    ControllerSensor();
    ControllerSensor(DeviceAddress _deviceAddress);
    String getName(void);
    void getAddress(DeviceAddress _deviceAddress);
    void setTemp(float temp);
    float getTemp(void);
    void setError(void);
    int getErrorCount(void);

  private:
    String convertAddress(DeviceAddress _deviceAddress);
    DeviceAddress deviceAddress;
    String deviceAddressString;
    float temp;
    int errors;

};

/*
 * Manage Dalls Temp activities
 */
class ControllerTemp {

  public:
    ControllerTemp();
    ControllerTemp(uint8_t _pin);
    void setVerbose(bool _flag);
    void setSampleRate(int _sampleRate);
    void setSampleRateAdvance(int _sampleRateAdvance);
    void initSensors(boolean waitForSensors);
    void resetLoop(void);
    void loop(void);
    int getDeviceCount(void);
    ControllerSensor getSensor(int _sensorIndex);

  private:
    OneWire myWire;
    uint8_t pin;
    DallasTemperature dTemp;
    ControllerSensor sensors[MAX_SENSORS];
    bool verbose;
    int deviceCount;
    unsigned long lastTempPull;
    int sampleRate;
    int sampleRateAdvance;
    bool requestSend;

};
