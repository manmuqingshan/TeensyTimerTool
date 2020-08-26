#pragma once
#include "LaserController.h"
#include "TeensyTimerTool.h"

class SystemController
{
 public:
    SystemController() : mainTimer(TeensyTimerTool::TCK) {} // construct the main 15Hz timer, we use a TCK here
    inline void begin();
    inline void shoot();

    inline void continousMode(bool on);
    inline void setExposureDelay(unsigned delay);

 protected:
    TeensyTimerTool::PeriodicTimer mainTimer;
    LaserController lCtrl1, lCtrl2;
    unsigned exposureDelay = 300;
};

void SystemController::begin()
{
    constexpr unsigned repetitionRate = 15; // Hz

    constexpr unsigned preTrig1_pin = 1;
    constexpr unsigned trig1_pin = 2;
    constexpr unsigned preTrig2_pin = 3;
    constexpr unsigned trig2_pin = 4;
    constexpr unsigned cam_pin = 5;

    lCtrl1.begin(preTrig1_pin, trig1_pin, cam_pin);
    lCtrl2.begin(preTrig2_pin, trig2_pin, cam_pin);

    mainTimer.begin([this] { this->shoot(); }, 1E6 / repetitionRate, false); // attach callback but don't start yet
}

void SystemController::shoot()
{
    unsigned start = micros();
    lCtrl1.shoot();
    while (micros() - start < exposureDelay) { yield(); }
    lCtrl2.shoot();
}

void SystemController::continousMode(bool on)
{
    if (on)
        mainTimer.start();
    else
        mainTimer.stop();
}

void SystemController::setExposureDelay(unsigned delay)
{
    exposureDelay = max(100u, min(500u, delay)); // limit to 100-500 µs
}