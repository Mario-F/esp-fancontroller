// Abstract DallasTempSensor functions

#include "ControllerTemp.h"
#include <OneWire.h>

/*
 * Manage Dalls Temp activities
 */
ControllerTemp::ControllerTemp() {};
ControllerTemp::ControllerTemp(uint8_t _pin) {
  // Init DallasTemp
  pin = _pin;
  myWire = OneWire(_pin);

  // defaults
  lastTempPull = millis();
  verbose = false;
  deviceCount = 0;
  sampleRate = 2750;
  sampleRateAdvance = 850;
  requestSend = false;
}

void ControllerTemp::setVerbose(bool _flag) {
  verbose = _flag;
}

void ControllerTemp::setSampleRate(int _sampleRate) {
  sampleRate = _sampleRate;
}

void ControllerTemp::setSampleRateAdvance(int _sampleRateAdvance) {
  sampleRateAdvance = _sampleRateAdvance;
}

void ControllerTemp::initSensors(boolean waitForSensors) {
  dTemp = DallasTemperature(&myWire);

  dTemp.begin();
  deviceCount = dTemp.getDeviceCount();
  while(waitForSensors && deviceCount <= 0) {
    Serial.println(LOG_PREFIX"(init) No temp sensors found try again after 1sec");
    delay(1000);
    deviceCount = dTemp.getDeviceCount();
  }
  if(verbose) {
    Serial.print(LOG_PREFIX"(init) Found "); Serial.print(deviceCount); Serial.println(" Devices.");
  }
  dTemp.setWaitForConversion(true);
  dTemp.requestTemperatures();
  for (int i = 0; i < deviceCount; i++) {
    DeviceAddress deviceAddress;
    dTemp.getAddress(deviceAddress, i);
    float temp = dTemp.getTempC(deviceAddress);
      if(verbose) {
        Serial.print(LOG_PREFIX"(Loop-Conversion) Temp ");
        Serial.print(temp);
        Serial.print(" from Device (directDeviceAddress) ");
        for(uint8_t i = 0; i < 8; i++) Serial.print(String(deviceAddress[i], HEX));
        Serial.println();
      }
    sensors[i] = ControllerSensor(deviceAddress);
    if(verbose) {
      Serial.print(LOG_PREFIX"(init) Got sensor "); Serial.println(sensors[i].getName());
    }
  }
  dTemp.setWaitForConversion(false);
  dTemp.requestTemperatures();
}

void ControllerTemp::resetLoop() {
  requestSend = false;
  lastTempPull = millis();
}

void ControllerTemp::loop() {
  int timePassed = (millis() - lastTempPull);

  // Check if temp conversion request was already send and time is right
  if ( !requestSend &&  timePassed >= ( sampleRate - sampleRateAdvance ) ) {
    if(verbose) {
      Serial.print(LOG_PREFIX"(Loop-Request) Time passed "); Serial.print(timePassed); Serial.println("ms");
    }
    requestSend = true;
    dTemp.requestTemperatures();
  }

  // Check if sample rate reached and request temp and reset loop
  if (timePassed >= sampleRate ) {
    if(verbose) {
      Serial.print(LOG_PREFIX"(Loop-Conversion) Time passed "); Serial.print(timePassed); Serial.println("ms");
    }
    for (int i = 0; i < deviceCount; i++) {
      DeviceAddress dadd;
      sensors[i].getAddress(dadd);
      float temp = dTemp.getTempC(dadd);
      if(verbose) {
        Serial.print(LOG_PREFIX"(Loop-Conversion) Temp ");
        Serial.print(temp);
        Serial.print(" from Device (directDeviceAddress) ");
        for(uint8_t i = 0; i < 8; i++) Serial.print(String(dadd[i], HEX));
        Serial.println();
      }
      if (temp != DEVICE_DISCONNECTED_C) {
        sensors[i].setTemp(temp);
      } else {
        sensors[i].setError();
      }
      if(verbose) {
        Serial.print(LOG_PREFIX"(Loop-Conversion) Convert Sensor ");
        Serial.print(sensors[i].getName() + ", Temp: ");
        Serial.print(sensors[i].getTemp());
        Serial.print(" celsius, Errors: ");
        Serial.println(sensors[i].getErrorCount());
      }
    }

    // Reset loop
    resetLoop();
  }
}

int ControllerTemp::getDeviceCount() {
  return deviceCount;
}

ControllerSensor ControllerTemp::getSensor(int _sensorIndex) {
  return sensors[_sensorIndex];
}


/*
 * Hold state of a single sensor
 */
ControllerSensor::ControllerSensor() {};
ControllerSensor::ControllerSensor(DeviceAddress _deviceAddress) {
  for(uint8_t i = 0; i < 8; i++) {
    deviceAddress[i] = _deviceAddress[i];
  }
  deviceAddressString = convertAddress(deviceAddress);
  temp = DEVICE_DISCONNECTED_C;
  errors = 0;
}

String ControllerSensor::getName() {
  return deviceAddressString;
}

void ControllerSensor::getAddress(DeviceAddress _deviceAddress) {
  for(uint8_t i = 0; i < 8; i++) {
    _deviceAddress[i] = deviceAddress[i];
  }
}

void ControllerSensor::setTemp(float _temp) {
  // Also reset errors because setTemp is only for valid values
  temp = _temp;
  errors = 0;
}

float ControllerSensor::getTemp() {
  return temp;
}

void ControllerSensor::setError() {
  errors++;
}

int ControllerSensor::getErrorCount() {
  return errors;
}

String ControllerSensor::convertAddress(DeviceAddress _deviceAddress) {
  String result = "";
  for (uint8_t i = 0; i < 8; i++)
  {
    result += String(_deviceAddress[i], HEX);
  }
  return result;
}
