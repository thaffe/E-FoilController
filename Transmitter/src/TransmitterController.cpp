#include "TransmitterController.h"

const float zeroSpeed = 65;
const float maxSpeed = 770;
const float throttleRange = maxSpeed - zeroSpeed;
#define MIN_SPEED = 0;
#define MAX_SPEED = 100;
#define DELAY 30

TransmitterController::TransmitterController()
{
}

void TransmitterController::setup()
{
    // Set all IO
    pinMode(deadMansButtonPin, INPUT_PULLUP);
    pinMode(throttlePin, INPUT);
  
    //Start BlueTooth communication
    BTSerial.begin(9600);
}

void TransmitterController::loop()
{
    if (!digitalRead(deadMansButtonPin))
    {
        float percent = (analogRead(throttlePin) - zeroSpeed) / throttleRange;
        throttleReading = min(max((1 - percent) * 256, 0), 256);
    }
    else
    {
        throttleReading = 0;
    }

    BTSerial.write(throttleReading);
    delay(50);
}

void TransmitterController::SendThrottleReading(uint8_t reading)
{
    BTSerial.write(reading);
    delay(DELAY);
}
