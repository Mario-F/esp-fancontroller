// Abstract PWM RPM Counter functions for Fancontrol

#include "ControllerFan.h"
#include "ConfigManager/ConfigManager.h"

ControllerFan* ControllerFan::cfInstance = 0;

ControllerFan::ControllerFan() {};
ControllerFan::ControllerFan(uint8_t _pinRPM, uint8_t _pinPWM) {
  pinRPM = _pinRPM;
  pinPWM = _pinPWM;
  fanRPMCounter = 0;
  fanActRPM = 0;
  fanSetSpeed = 0;
  
  pinMode(pinPWM, OUTPUT);
  analogWriteFreq(22000);
  setSpeed(100);
  
  cfInstance = this;
  attachInterrupt(digitalPinToInterrupt(pinRPM), ControllerFan::incrISR, RISING);
};

void ControllerFan::setVerbose(bool _flag) {
  verbose = _flag;
}

void ControllerFan::setSpeed(int newSpeed) {
  int minSpeed = ConfigManager::getConfig().minimumSpeed;
  fanSetSpeed = constrain(newSpeed, minSpeed, 100);
  if(verbose) {
    Serial.print(LOG_PREFIX_CF"(setSpeed) Set new Speed ");
    Serial.println(fanSetSpeed);
  }
  analogWrite(pinPWM, fanSetSpeed * 2.55);
};

int ControllerFan::getSpeed() {
  return fanSetSpeed;
};

int ControllerFan::getRPM() {
  return fanActRPM;
};

void ControllerFan::loop() {
  int timePassed = (millis() - fanRPMTimer);
  if(timePassed > 2000) {
    // Set Fanspeed
    fanActRPM = (fanRPMCounter * 30 / 2);

    if(verbose) {
      Serial.print(LOG_PREFIX_CF"(loop) FanSpeed - Time passed ");
      Serial.print(timePassed);
      Serial.print("ms, with Fanspeed: ");
      Serial.print(fanActRPM);
      Serial.println(" rpm");
    }

    // Reset loop
    fanRPMTimer = millis();
    fanRPMCounter = 0;
  }
};

ICACHE_RAM_ATTR void ControllerFan::incrISR() {
  if (cfInstance != 0) {
    cfInstance->fanRPMCounter++;
  }
};
