// Abstract PWM RPM Counter functions for Fancontrol

#include <Arduino.h>

#define LOG_PREFIX_CF "ControllerFan - "

class ControllerFan {

  public:
    ControllerFan();
    ControllerFan(uint8_t _pinRPM, uint8_t _pinPWM);
    void setVerbose(bool _flag);
    void setSpeed(int newSpeed);
    int getSpeed();
    int getRPM();
    void loop();

  private:
    uint8_t pinRPM;
    uint8_t pinPWM;
    bool verbose;
    int fanRPMCounter;
    int fanActRPM;
    int fanSetSpeed;
    unsigned long fanRPMTimer;
    static ControllerFan* cfInstance;
    ICACHE_RAM_ATTR static void incrISR();
};
