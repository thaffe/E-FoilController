#include "TransmitterController.h"

TransmitterController::TransmitterController() {}

void TransmitterController::setup()
{
    // Set all IO
    pinMode(deadMansButtonPin, INPUT_PULLUP);
    pinMode(throttlePin, INPUT);

    //Start BlueTooth communication
    // BTSerial.begin(9600);
    Serial.begin(9600);

    // Initi readings to zeroSpeed
    for (int i = 0; i < averageReadsCount; i++)
    {
        lastReadings[i] = throttleMin;
    }
}

void TransmitterController::loop()
{

    updateThrottleReading();

    if (readCount >= readingsPrSend)
    {
        readCount = 0;
        uint8_t reading = min(max(getAverage() * 256, 0), 256);
        // Serial.println(reading);
        Serial.write(reading);
    }

    delay(updateEveryMs);
}

void TransmitterController::updateThrottleReading()
{
    float reading = 0;
    if (!digitalRead(deadMansButtonPin))
        reading = 1 - (analogRead(throttlePin) - throttleMin) / throttleRange;

    lastReadings[readPointer] = reading;
    readPointer++;
    readCount++;
    if (readPointer >= averageReadsCount)
        readPointer = 0;
}

float TransmitterController::getAverage()
{
    float sum = 0;
    for (int i = 0; i < averageReadsCount; i++)
        sum += lastReadings[i];

    return sum / averageReadsCount;
}
